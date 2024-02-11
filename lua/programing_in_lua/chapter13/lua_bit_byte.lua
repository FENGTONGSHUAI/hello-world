-- 13.1 位运算
print(string.format("%x", 0xff & 0xabcd))
print(string.format("%x", 0xff | 0xabcd))
print(string.format("%x", 0xaaaa ~ -1))
print(string.format("%x", ~0))

print(string.format("%x", 0xff << 12))
print(string.format("%x", 0xff >> -12))
print(string.format("%x", -1 << 80))

-- 13.2 无符号整数型
x = 3 << 62
print(x)
print(string.format("%u", x))
print(string.format("0x%X", x))

print(string.format("%u", x))
print(string.format("%u", x + 1))
print(string.format("%u", x - 1))

a = 0x7fffffffffffffff
b = 0x8000000000000000

print(a < b)
print(math.ult(a , b))

mask = 0x8000000000000000  -- 这里并没有理解这个掩码的作用, 后面看一下
c = a ~ mask
d = b ~ mask

print(c, d, c < d)

function udiv(n, d)
    if d < 0 then
        if math.ult(n, d) then return 0 
        else return 1 
        end
    end

    local q = ((n >> 1) // d) << 1
    local r = n - q * d
    if not math.ult(r, d) then
        q = q + 1 
    end
    return q
end

-- 后面的无符号整数和浮点数之间的换算没有太看懂

-- 13.3 打包和解包二进制数据
