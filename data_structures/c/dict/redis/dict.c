#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <sys/time.h>
#include <ctype.h>

#include "dict.h"
#include "zmalloc.h"

// 指示字典是否启用rehash标识
static int dict_can_resize = 1;
// 强制 rehash 的比率
static unsigned int dict_force_resize_ratio = 5;

/* -------------------------- private prototypes ---------------------------- */
static int _dictExpandIfNeeded(dict *ht);
static unsigned long _dictNextPower(unsigned long size);
static int _dictKeyIndex(dict *ht, const void *key);
static int _dictInit(dict *ht, dictType *type, void *privDataPtr);

/* -------------------------- hash functions -------------------------------- */

/* Thomas Wang's 32 bit Mix Function */
// todo: 不知道这个在干啥, 有时间可以找一下资料看一下
unsigned int dictIntHashFunction(unsigned int key)
{
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return key;
}

/* Identity hash function for integer keys */
unsigned int dictIdentityHashFunction(unsigned int key)
{
    return key;
}

// todo: 这个是做什么用的?
static uint32_t dict_hash_function_seed = 5581;

void dictSetHashFunctionSeed(uint32_t seed) 
{
    dict_hash_function_seed = seed;
}

uint32_t dictGetHashFunctionSeed(void)
{
    return dict_hash_function_seed;
}

/* MurmurHash2, by Austin Appleby
 * Note - This code makes a few assumptions about how your machine behaves -
 * 1. We can read a 4-byte value from any address without crashing
 * 2. sizeof(int) == 4
 *
 * And it has a few limitations -
 *
 * 1. It will not work incrementally.
 * 2. It will not produce the same results on little-endian and big-endian
 *    machines.
 * 
 * todo: 后面有时间研究一下这个算法
 */

unsigned int dictGenHashFunction(const void *key, int len)
{
    /* 'm' and 'r' are mixing constants generated offline.
    They're not really 'magic', they just happen to work well.  */
    uint32_t seed = dict_hash_function_seed;
    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */
    uint32_t h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */
    const unsigned char *data = (const unsigned char *)key;

    while(len >= 4) {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */
    switch(len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    };

    /* Do a few final mixes of the hash to ensure the last few
     * bytes are well-incorporated. */
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return (unsigned int)h;
}

/* And a case insensitive hash function (based on djb hash) */
unsigned int dictGenHashFunction(const unsigned char *buf, int len)
{
    unsigned int hash = (unsigned int)dict_hash_function_seed;

    while (len--)
    {
        hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
    }

    return hash;
}

/* ----------------------------- API implementation ------------------------- */
/* 
重置或初始化给定哈希表的各项属性值
T = O(1)
*/
static void _dictReset(dictht *ht)
{
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

/*
创建新字典
T = O(1)
*/
dict *dictCreate(dictType *type, void *privDataPtr)
{
    dict *d = zmalloc(sizeof(*d));
    
    _dictInit(d, type, privDataPtr);

    return d;
}

/*
初始化哈希表
T =  O(1)
*/
int _dictInit(dict *d, dictType *type, void *privDataPtr)
{
    // 初始化两个哈希表的各项属性值
    // 但是暂时还不分配内容给哈希表数组
    _dictReset(&(d->ht[0]));
    _dictReset(&(d->ht[1]));

    // 设置类型特定函数
    d->type = type;

    // 设置私有数据
    d->privdata = privDataPtr;

    // 设置哈希表 rehash状态
    d->rehashidx = -1;

    // 设置字典的安全迭代器数量
    d->iterators = 0;

    return DICT_OK;
}

/*
缩小给定的字典, 使它已用节点数与字典大小之间的比率接近1:1
返回 DICT_ERR 表示字典已经在rehash, 或者dict_can_resize为假
成功创建体积更小的ht[1], 可以开始resize时, 返回DICK_OK
T = O(N)
*/

int dictResize(dict *d)
{
    int minimal;

    // 不能再关闭 rehash 或者正在 rehash 的时候调用
    if(!dict_can_resize || dictIsRehashing(d)) 
    {
        return DICT_ERR;
    }

    // 计算让比率接近 1:1 所需要的最少节点数量
    minimal = d->ht[0].used;
    if(minimal < DICT_HT_INITIAL_SIZE) 
    {
        minimal = DICT_HT_INITIAL_SIZE;
    }

    // 调整字段的大小
    // T = O(N)
    return dictExpand(d, minimal)
}

/*
创建1个新的哈希表, 并根据字典的情况, 选择以下其中一个动作来进行:
1) 如果字典的 0 号哈希表为空, 那么将新哈希表设置为 0 号哈希表
2) 如果字典的 0 号哈希表非空，那么将新哈希表设置为 1 号哈希表, 
    并打开字典的 rehash 标识，使得程序可以开始对字典进行 rehash
size 参数不够大, 或者rehash已经在进行时, 返回DICK_ERR
成功创建 0 号哈希表，或者 1 号哈希表时，返回 DICT_OK

T = O(N)
*/
int dictExpand(dict *d, unsigned long size)
{
    // 新哈希表
    dictht n;

    // 根据size参数, 计算哈希表的大小
    // T = O(1)
    unsigned long realsize = _dictNextPower(size);

    // 不能在字典正在 rehash 时进行
    // size 的值也不能小于 0 号哈希表的当前已使用节点数
    if (dictIsRehashing(d) || d->dt[0].used > size)
        return DICT_ERR;

    // 为哈希表分配空间, 并将所有指针指向NULL
    n.size = realsize;
    n.sizemask = realsize - 1;
    // T = O(N)
    // calloc会将内存初始化
    n.table = zcalloc(realsize * sizeof(dictEntry*));
    n.used = 0;

    /* 
    这是第一次初始化吗？如果是，那实际上并不是真正的rehash
    我们只是设置了第一个哈希表，以便它可以接受键。
    */
    if(d->ht[0].table == NULL) {
        d->ht[0] = n;
        return DICT_OK;
    }

    // 准备另一个哈希表用于渐进式哈希
    d->ht[1] = n;
    d->rehashidx = 0;
    rerurn DICT_OK;
}

/*
执行 N 步增量重新哈希。
如果仍有键需要从旧哈希表移动到新哈希表，则返回 1，否则返回 0。
请注意，rehash步骤包括将存储桶
（由于我们使用链表，因此可能有多个键）从旧哈希表移动到新哈希表。
T = O(N)
*/

int dictRehash(dict *d, int n)
{
    // 只可以在 rehash 进行中时执行
    if(!dictIsRehashing(d)) return 0;

    // 进行n步迁移
    // T = O(N)
    while(n--) 
    {
        dictEntry *de, *nextde;
        // 如果 0 号哈希表为空, 那么表示rehash执行完毕
        // T = o(1)
        if(d->ht[0].used == 0)
        {
            // 释放0号哈希表
            zfree(d->ht[0].table);
            // 将原来的1号哈希表设置为新的0号哈希表
            d->ht[0] = d->ht[1];
            // 重置旧的1号哈希表
            _dictReset(&(d->ht[1]));
            // 关闭rehash标识
            d->rehashidx = -1;
            // 返回0, 表示rehash已经完成
            return 0;
        }

        /*
            请注意，rehashidx 不会溢出，
            因为我们确信还有更多元素，因为 ht[0].used != 0
        */ 
        if(d->ht[0].size > (unsigned)d->rehashidx)
        {
            fprintf(stderr, "dictRehash: d->ht[0].size = %lu > d->rehashidx = %d\n", d->ht[0].size, d->rehashidx);
            fflush(stderr);
            abort();
        }

        // 略过数组中为空的索引, 找到下一个非空索引
        while(d->ht[0].table[d->rehashidx] == NULL)
        {
            d->rehashidx++;
        }

        // 指向该索引的链表表头节点
        de = d->ht[0].table[d->rehashidx];
        // 将链表中的所有节点迁移到新哈希表
        // T = O(1)
        while (de)
        {
            unsigned int h;

            // 保存下个节点的指针
            nextde = de->next;

            // 计算新哈希表的哈希值, 以及几点插入的索引位置
            h = dictHashKey(d, de->key) & d->ht[1].sizemask;

            // 插入节点到新哈希表
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;

            // 更新计数器
            d->ht[0].used--;
            d->ht[1].used++;

            // 继续处理下个节点
            de = nextde;
        }
        
        // 将刚迁移完的哈希表索引的指针设置为空
        d->ht[0].table[d->rehashidx] = NULL;
        // 更新rehash索引
        d->rehashidx++;
    }

    return 1;
}

/*
返回以毫秒为单位的unix时间戳
T = O(1)
*/
long long timeInMilliseconds(void) 
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec/1000);
}

/*
在给定毫秒数内, 以100步为单位, 对字典进行rehash
T = O(N)
*/
int dictRehashMilliseconds(dict *d, int ms)
{
    // 记录开始时间
    long long start = timeInMilliseconds();
    int rehashes = 0;

    while(dictRehash(d, 100))
    {
        rehashes += 100;
        // 如果时间已过, 跳出
        if(timeInMilliseconds() - start > ms)
        {
            break;
        }
    }

    return rehashes;
}

/*
此函数仅执行单个桶的rehash, 
并且仅在没有安全迭代器绑定到我们的哈希表时才执行。
当我们在rehash中间有迭代器时，
我们不能弄乱两个哈希表，否则某些元素可能会丢失或重复

此函数由字典中的常见查找或更新操作调用，
以便哈希表在被使用时自动从 H1 迁移到 H2。

T = O(1)
*/
static void _dictRehashStep(dict *d)
{
    if(d->iterators == 0)
    {
        dictRehash(d, 1)
    }
}

/*
添加元素到哈希表中
最坏 T = O(N), 平均 O(1)
*/
int dictAdd(dict *d, void *key, void *val)
{
    // 尝试添加键到字典, 并返回包含了这个键的新哈希节点
    // T = O(N)
    dictEntry *entry = dictAddRaw(d, key);

    // 键已存在, 添加失败
    if(!entry) return DICT_ERR;

    // 键不存在, 设置节点的值
    // T = O(1)
    dictSetVal(d, entry, val);

    // 添加成功
    return DICT_OK;
}

/*
此函数添加条目，但不设置值，而是将 dictEntry 结构返回给用户，
这将确保按照用户的意愿填充值字段
该函数也直接暴露给用户API，
主要是为了在哈希值中存储非指针而调用，例如：
entry = dictAddRaw(dict,mykey);
if (entry != NULL) dictSetSignedIntegerVal(entry,1000);

返回值:
如果key已经存在, 返回NULL
如果key正常添加, 返回节点, 供调用者使用
T = O(N)
*/

dictEntry *dictAddRaw(dict *d, void *key)
{
    int index;
    dictEntry *entry;
    dictht *ht;

    // 如果在rehash过程中, 则进行单步rehash
    if(dictIsRehashing(d)) _dictRehashStep(d);

    // 计算键在哈希表中的索引值
    // 如果值为-1, 说明键已存在
    if(index = _dictKeyIndex(d, key) == -1)
        return NULL
    
    // 分配内存, 并且插入新的entry
    ht = dictIsRehashing(d) ? &(d->ht[1]) : &(d->ht[0]);
    // 为新节点分配空间
    // 分配之后不用初始化, 因为后面会把所有的数据都设置一下
    entry = zmalloc(sizeof(*entry));
    // 将新节插入到链表表头
    entry->next = ht->table[index];
    ht->table[index] = entry;
    // 更新哈希表已使用节点数量
    ht->used++;

    // 设置新节点的键
    dictSetKey(d, entry, key);

    return entry;
}

/*
将给定的键值对添加到字典中, 如果键已经存在, 则删除旧的
如果键是从头添加的，则返回 1；
如果已经存在具有该键的元素并且 dictReplace() 仅执行了值更新操作，则返回 0。
T = O(N)
*/

int dictReplace(dict *d, void *key, void *val)
{
    dictEntry *entry, auxentry;

    // 尝试添加键值, 如果key不存在, 添加会执行成功
    if(dictAdd(d, key, val) == DICT_OK) return 1;

    // 运行到这里, 说明key存在, 获取entry
    entry = dictFind(d, key);

    /*
    设置新值并释放旧值。请注意，按此顺序执行此操作很重要，
    因为该值可能与前一个值完全相同。在这种情况下，
    想想引用计数，您需要增加（设置），然后减少（释放），而不是相反。
    
    这里的实现不能调用这个函数来设置与之前key一样的val的, 因为free的
    时候并没有管引用计数
    */

    // 先保存原有的值的指针
    auxentry = *entry;
    // 设置新的值
    dictSetVal(d, entry, val);
    // 释放旧的值
    dictFreeVal(d, &auxentry);

    return 0;
}

/*
dictAddRaw() 根据给定 key 释放存在，执行以下动作：

1) key 已经存在，返回包含该 key 的字典节点
2) key 不存在，那么将 key 添加到字典

不论发生以上的哪一种情况，
dictAddRaw() 都总是返回包含给定 key 的字典节点。

T = O(N)
*/

dictEntry *dictReplaceRaw(dict *d, void *key)
{
    // 使用key在字典中查找节点
    dictEntry *entry = dictFind(d, key);
    // 如果节点找到了直接返回节点, 否则添加并返回一个新节点
    return entry ? entry : dictAddRaw(d, key);
}


/*
查找并删除给定键的节点
T = O(1)
*/

static int dictGenericDelete(dict *d, const void *key, int nofree)
{
    unsigned int h, idx;
    dictEntry *he, *prevHe;
    int table;

    // 哈希表为空
    if(d->ht[0].size == 0) return DICT_ERR;

    // 进行单步rehash
    if(dictIsRehashing(d)) _dictRehashStep(d);

    // 计算哈希值
    h = dictHashKey(d, key);

    // 遍历哈希表
    for(table = 0; table <= 1; table++) {
        // 计算索引值
        idx = h & ht[table].sizemask;
        // 指向该索引上的链表
        he = d->ht[table].table[idx];
        prevHe = NULL;

        // 遍历链表上的所有节点
        while(he) 
        {
            if(dictCompareKeys(d, key, he->key))
            {
                // 从链表中删除
                if(prevHe)
                {
                    prevHe->next = he->next;
                } 
                else 
                {
                    d->ht[table].table[idx] = he->next;
                }

                if(!nofree)
                {
                    dictFreeKey(d, he);
                    dictFreeVal(d, he);
                }

                // 释放节点本身
                zfree(he);

                // 更新已使用节点数量
                d->ht[table].used--;

                // 返回已找到信号
                return DICT_OK;
            }

            prevHe = he;
            he = he->next;
        }

        // 如果不是在rehash过程中, d->ht[1].table = NULL, 
        // 不用找
        if(!dictIsRehashing(d)) break;
    }

    // 没找到
    return  DICT_ERR;
}

/*
删除给定的键, 且释放键和值
T = O(1)
*/

int dictDelete(dict *ht, const void *key) 
{
    return dictGenericDelete(ht, key, 0);
}

/*
删除给定的键, 但不释放键和值
T = O(1)
*/

int dictDeleteNoFree(dict *ht, const void *key) 
{
    return dictGenericDelete(ht, key, 1);
}

/*
销毁整个字典
T = O(N)
*/

int _dictClear(dict *d, dictht *ht, void(callback)(void *))
{
    unsigned long i;

    // 释放所有成员
    for(i = 0; i < ht->size && ht->used > 0; i++)
    {
        dictEntry *he, *nextHe;

        if(callback && (i & 65535) == 0) callback(d->privdata);

        // 跳过空索引
        if((he = ht->table[i]) == NULL) continue;

        // 遍历整个链表
        while(he) 
        {
            nextHe = he->next;
            // 删除键
            dictFreeKey(d, he);
            // 删除值
            dictFreeVal(d, he);
            // 释放节点
            zfree(he);

            // 更新已使用节点计数
            ht->used--;

            // 处理下个节点
            he = nextHe;
        }
    }

    // 释放哈希表结构
    zfree(ht->table);

    // 重置哈希表属性
    _dictReset(ht);

    return DICT_OK;
}

/*
删除并释放整个字典
T = O(N)
*/

void dictRelease(dict *d)
{
    // 删除并清空两个哈希表
    _dictClear(d, &(d->ht[0]), NULL);
    _dictClear(d, &(d->ht[1]), NULL);

    // 释放节点结构
    zfree(d);
}

/*
返回字典中包含键key的节点

T = O(1)
*/
dictEntry *dictFind(dict *d, const void *key)
{
    dictEntry *he;
    unsigned int h, idx, table;

    // 哈希表为空
    if (d->ht[0].size == 0) return NULL; 

    if(dictIsRehashing(d)) _dictRehashStep(d);

    // 计算键的哈希值
    h = dictHashKey(d, key);
    // 在字典的哈希表中查找这个键
    for(table = 0; table <= 1; table ++) 
    {
        // 计算索引值
        idx = h & ht[table].sizemask;
        // 遍历给定索引上的链表的所有节点, 查找Key
        he = d->ht[table].table[idx];
        while(he) 
        {
            if(dictCompareKeys(d, key, he->key))
            {
                return he;
            }

            he = he->next;
        }

        // 如果不是在rehash过程中, d->ht[1].table = NULL, 
        // 不用找
        if(!dictIsRehashing(d)) 
        {
            return NULL;
        }
    }

    // 进行到这里, 说明两个哈希表都没有找到
    return NULL;
}

/*
获取给定键节点的值

T = O(1)
*/

void *dictFetchValue(dict *d, const void *key)
{
    dictEntry *he;

    he = dictFind(d, key);

    return he ? dictGetVal(he) : NULL;
}

/*
指纹是一个 64 位数字，表示给定时间的字典状态，
它只是几个字典属性的异或运算。初始化不安全迭代器时，
我们会获取字典指纹，并在释放迭代器时再次检查指纹。
如果两个指纹不同，则意味着迭代器的用户在迭代时对字典执行了禁止的操作。
todo: 这里的算法后面有时间可以研究一下
*/

long long dictFingerprint(dict *d)
{
    long long integers[6], hash = 0;
    int j;

    integers[0] = (long) d->ht[0].table;
    integers[1] = d->ht[0].size;
    integers[2] = d->ht[0].used;
    integers[3] = (long) d->ht[1].table;
    integers[4] = d->ht[1].size;
    integers[5] = d->ht[1].used;

    /*
    我们通过对每个整数与前一个整数hash值相加的和进行hash
    来对 N 个整数进行哈希处理。也即：
        result = hash(hash(hash(int1)+int2)+int3) ...
    这样，不同顺序的同一组整数将（可能）哈希为不同的数字。
    */
    for (j = 0; j < 6; j++) {
        hash += integers[j];
        /* For the hashing step we use Tomas Wang's 64 bit integer hash. */
        hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
        hash = hash ^ (hash >> 24);
        hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
        hash = hash ^ (hash >> 14);
        hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
        hash = hash ^ (hash >> 28);
        hash = hash + (hash << 31);
    }
    return hash;
}

/*
创建并返回给定字典的不安全迭代器
T = O(1)

todo: 什么叫不安全呢?
*/
dictIterator *dictGetIterator(dict *d)
{
    dictIterator *iter = zmalloc(sizeof(*iter));

    iter->d = d;
    iter->table = 0;
    iter->index = -1;
    iter->safe = 0;
    iter->entry = NULL;
    iter->nextEntry = NULL;

    return iter;
}

/*
创建并返回给定节点的安全迭代器
T = O(1)
*/
dictIterator *dictGetSafeIterator(dict *d)
{
    dictIterator *i = dictGetIterator(d);

    // 设置安全迭代器标识
    i->safe = 1;

    return i;
}

/*
返回迭代器指向的当前节点
*/

dictEntry *dictNext(dictIterator *iter)
{
    while(1)
    {
        if(iter->entry == NULL)
        {
            // 指向被迭代的哈希表
            dictht *ht = &(iter->d->ht[iter->table]);
            
            // 初次迭代时执行
            if(iter->index == -1 && iter->table == 0)
            {
                // 如果是安全迭代器, 那么更新安全迭代器计数
                if(iter->safe)
                {
                    iter->d->iterators++;
                }
                // 如果是不安全迭代器, 那么计算指纹
                else
                {
                    iter->fingerprint = dictFingerprint(iter->d);
                }
            }
            // 更新索引
            iter->index++;

            // 如果迭代器的当前索引大于当前被迭代的哈希表的大小
            // 那么说明这个哈希表已经迭代完毕
            // todo: 这里转成signed类型比较会不会有问题呢？
            if(iter->index >= (signed) ht->size)
            {
                // 如果正在rehash的话, 那么说明1号哈希表也正在使用中
                // 那么继续对1号哈希表进行迭代
                if(dictIsRehashing(iter->d) && iter->table == 0)
                {
                    iter->table++;
                    iter->index = 0;
                    ht = &(iter->d->ht[1]);
                }
                // 如果没有rehash, 那么说明迭代已经完成了 
                else
                {
                    break;
                }
            }

            // 如果进行到这里, 说明这个哈希表并未迭代完
            // 更新节点指针, 指向下个索引链表的表头节点
            iter->entry = ht->table[iter->index];
        }
        else
        {
            // 执行到这里, 说明程序正在迭代某个链表
            // 将节点指针指向链表的下个节点
            iter->entry = iter->nextEntry;
        }

        // 如果当前节点不为空，那么也记录下该节点的下个节点
        // 因为安全迭代器有可能会将迭代器返回的当前节点删除
        if(iter->entry)
        {
            iter->nextEntry = iter->entry->next;
            return iter->entry;
        }
    }

    // 迭代完毕
    return NULL;
}

/*
释放给定字典的迭代器
T = O(1)
*/
void dictReleaseIterator(dictIterator *iter)
{
    if(!(iter->index == -1 && iter->table == 0))
    {
        // 释放安全迭代器, 安全迭代器计数器减1
        if(iter->safe)
        {
            iter->d->iterators--;
        }
        // 释放不安全迭代器, 验指纹是否有变化
        else
        {
            if(iter->fingerprint == dictFingerprint(iter->d))
            {
                fprintf(stderr, "dictReleaseIterator: iter->fingerprint = %lld > dictFingerprint(iter->d) = %lld\n",
                    iter->fingerprint, dictFingerprint(iter->d));
                fflush(stderr);
                abort();
            }
        }
    }
    zfree(iter);
}

/*
随机返回字典中的任意一个节点
可用于实现随机化算法
T = O(N)
*/
dictEntry *dictGetRandonKey(dict *d)
{
    dictEntry *he, *orighe;
    unsigned int h;
    int listlen, listele;

    // 字典为空
    if(dictSize(d) == 0) return NULL;

    // 进行单步hash
    if(dictIsRehashing(d)) _dictRehashStep(d);

    // 如果正在rehash, 那么将1号哈希表也作为随机查找的目标
    if(dictIsRehashing(d))
    {
        do
        {
            h = random() % (d->ht[0].size + d->ht[1].size);
            he = (h >= d->ht[0].size) ? (d->ht[1].table[h - d->ht[0].size]) :
                                        (d->ht[0].table[h]);
        } while (he == NULL);
    }
    else
    {
        do 
        {
            h = random() & (d->ht[0].sizemask);
            he = d->ht[0].table[h];
        } while(he == NULL);
    }

    // 目前he已经指向了一个非空的节点链表
    // 程序将从这个链表随机返回一个节点
    listlen = 0;
    orighe = he;
    // 计算节点数量
    while (he)
    {
        he = he->next;
        listlen++;
    }
    // 获取随机节点索引
    listele = random() % listlen;
    he = orighe;
    // 查找节点
    while(listele)
    {
        he = he->next;
        listele--;
    }
    
    // 返回随机节点
    return he;
}

/*
这是 dictGetRandomKey() 的一个版本，它经过修改，以便
通过在哈希表的随机位置跳转并线性扫描条目来返回多个条目。
返回的指向哈希表条目的指针存储在指向 dictEntry 
指针数组的“des”中。该数组必须至少有“count”个元素的空间，
这是我们传递给函数的参数，用于告知我们需要多少个随机元素。
该函数返回存储在“des”中的项目数，
如果哈希表中的元素少于“count”，则该数字可能小于“count”。
请注意，当您需要返回项目的良好分布时，
此函数不适用，但仅当您需要“抽样”给定数量的连续元素
以运行某种算法或生成统计数据时才适用。
但是，该函数在生成 N 个元素时比 dictGetRandomKey() 
快得多，并且保证元素不重复。
*/
int dictGetRandonKeys(dict *d, dictEntry **des, int count)
{
    // 内部哈希表id, 0或者1
    int j;
    int stored = 0;

}