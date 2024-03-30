#include <stdio.h>

# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name))) \
    __attribute_copy__ (name);

void original_function() {
    printf("This is the original function.\n");
}

strong_alias(original_function, alias_function)

int main() {
    original_function();
    alias_function();
    return 0;
}

/*
gcc -E test_strong_alias.c -o test_strong_alias.i
gcc -o test_strong_alias test_strong_alias.c

文心一言的解释:
这段代码定义了一个宏_strong_alias，它用于在GCC编译器中创建符号的强别名。强别名意味着链接器会将所有对别名的引用解析为对原始符号的引用。这种技术常用于库的内部实现，以便在不改变库外部接口的情况下，能够替换或优化内部函数。

让我们一步步地解释这段代码：

c
#define _strong_alias(name, aliasname) \  
  extern __typeof (name) aliasname __attribute__ ((alias (#name))) \  
    __attribute_copy__ (name);
#define _strong_alias(name, aliasname): 定义一个宏_strong_alias，它接受两个参数：name（原始符号的名称）和aliasname（要创建的别名的名称）。

extern __typeof (name) aliasname: 这部分代码声明了一个外部符号aliasname，其类型与name相同。__typeof是GCC的一个内置运算符，用于获取name的类型。

__attribute__ ((alias (#name))): 这是一个GCC特定的函数属性，它告诉编译器aliasname是name的一个别名。#name是预处理器的字符串化操作符，它将name转换为一个字符串字面量，这样我们就可以在alias属性中使用它。

__attribute_copy__ (name);: 这个属性并不是GCC的标准属性，可能是某个特定项目或库定义的。从其名称来看，它可能是用来复制name符号的其他属性到aliasname上。这样做可以确保别名和原始符号在其他方面（如可见性、链接规则等）保持一致。需要注意的是，不是所有的项目或库都会定义这个属性，它可能是特定于某个实现或环境的。

在使用这个宏时，你可以这样写：

c
void original_function(void) {  
    // ... 函数实现 ...  
}  
  
_strong_alias(original_function, alias_function);
这将会告诉编译器和链接器，alias_function是original_function的一个别名，任何对alias_function的引用在链接时都会被解析为对original_function的引用。

请注意，由于__attribute_copy__不是GCC的标准属性，因此在使用这个宏之前，你需要确保你的编译环境支持这个属性，或者如果不需要复制属性，你可以简单地移除这部分代码。
*/