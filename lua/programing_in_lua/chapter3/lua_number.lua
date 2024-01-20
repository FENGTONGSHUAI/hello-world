print(4)
print(0.4)
print(4.57e-3)

print(0.3e12)
print(5E+20)

print(type(3))
print(type(3.5))
print(type(3.0))

print(1 == 1.0)
print(-3 == -3.0)
print(0.2e3 == 200)

print(math.type(3))
print(math.type(3.0))

print(0xff)
print(0x1A3)
print(0x0.2)
print(0x1p-1)
print(0xa.bp2)

print(string.format("%a", 419))
print(string.format("%a", 0.1))

print(13 + 15)
print(13.0 + 15.0)
print(13.0 + 25)
print(-(3 * 6.0))
print(3.0/2.0)
print(3/2)
print(3//2)
print(3.0//2)
print(6//2)
print(6.0//2.0)
print(-9//2.0)
print(1.5//0.5)
x = math.pi
print(x - x % 0.01)
print(x - x%0.001)

local tolerance1 = 10
function istrunback1(angle)
    angle = angle % 360
    return (math.abs(angle - 180) < tolerance1)
end

print(istrunback1(-180))
print(-180 % 360)
print(180 % -360)
print(-180 % -360)


local tolerance2 = 0.17
function istrunback2(angle)
    angle = angle % (2*math.pi)
    return (math.abs(angle - math.pi) < tolerance2)
end

print(0.17 % (2 * math.pi))

print(math.floor(3.3))
print(math.floor(-3.3))
print(math.ceil(3.3))
print(math.ceil(-3.3))
print(math.modf(3.3))
print(math.modf(-3.3))
print(math.floor(2^70))

x = 2^52 + 1
print(string.format("%d %d", x, math.floor(x + 0.5)))

function round(x)
    local f = math.floor(x)
    if x == f then return f
    else return math.floor(x + 0.5)
    end
end

print(round(2.5))
print(round(x))
print(round(x + 0.5))

function round(x)
    local f = math.floor(x)
    if (x == f) or (x % 2.0 == 0.5) then
        return f
    else
        return math.floor(x + 0.5)
    end
end

print(round(2.5))
print(round(3.5))
print(round(-2.5))
print(round(-1.5))

print(math.maxinteger + 1 == math.mininteger)
print(math.mininteger - 1 == math.maxinteger)
print(-math.mininteger == math.mininteger)
print(math.mininteger // -1 == math.mininteger)

print(math.maxinteger)
print(0x7fffffffffffffff)
print(math.mininteger)
print(0x8000000000000000)

print(string.format("%.53f", 1 / 7 * 7)) -- 双精度浮点数: 1 位符号位; 11 位指数; 52位尾数 尾数省略了一个1 精确表示有53位
print(string.format("%.53f", 12.7 - 20 + 7.3))
print(math.maxinteger + 2)
print(math.maxinteger + 2.0)
print(math.maxinteger + 2.0 == math.maxinteger + 1.0)

print(-3 + 0.0)
print(0x7fffffffffffffff + 0.0)
print(9007199254740991 + 0.0 == 9007199254740991)
print(9007199254740992 + 0.0 == 9007199254740992)
print(9007199254740993 + 0.0 == 9007199254740993)

print(2^53) 
print(2^53 | 0)  -- 这个特性, 后面可以看一下
-- print(3.2 | 0)
-- print(2^64 | 0)
-- math.random(1, 3.5)

print(math.tointeger(-258.0))
print(math.tointeger(2^30))
print(math.tointeger(5.01))
print(math.tointeger(2^64))

function cond2int(x)
    return math.tointeger(x) or x
end

-- 3.8 兼容性可以注意一下









