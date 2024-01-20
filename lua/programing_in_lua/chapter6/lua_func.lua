local function get_str(var)
    local result = {}
    if type(var) == "table" then
        for k, v in pairs(var) do
            local k_v_str = string.format("%s = %s", k, get_str(v))
            table.insert(result, k_v_str)
        end
        return table.concat(result, ", ")
    end
    return string.format("%s", var)
end

print(8 * 9, 9 / 8)
a = math.sin(3) + math.cos(10)
print(a)
print(os.date())
print "Hello World"
print [[a multi-line message]]
type{}

function add(a)
    local sum = 0
    for i = 1, #a do
        sum = sum + a[i]
    end
    return sum
end

function f(a, b) print(a, b) end
print(f())
print(f(3))
print(f(3, 4))
print(f(3, 4, 5))


globalCounter = 0
function incCount(n)
    n = n or 1
    globalCounter = globalCounter + n
end

-- 6.1 多返回值
s, e = string.find("hello Lua uesrs", "Lua")
print(s, e)

function maximum(a)
    local mi = 1
    local m = a[mi]
    for i = 1, #a do
        if a[i] > m then
            mi = i;
            m = a[i]
        end
    end

    return m, mi
end

print(maximum({8, 10, 23, 12, 5}))

function foo0() end
function foo1() return "a" end
function foo2() return "a", "b" end

x, y = foo2()
print(x, y)
x = foo2()
print(x)
x, y, z = 10, foo2()
print(x, y, z)

x, y = foo0()
print(x, y)
x, y = foo1()
print(x, y)
x, y, z = foo2()
print(x, y, z)

x, y = foo2(), 20
print(x, y)
x, y = foo0(), 20, 30
print(x, y)
print(foo0())
print(foo1())
print(foo2())
print(foo2(), 1)
print(foo2() .. "x")

t = {foo0()}
print(get_str(t))
t = {foo1()}
print(get_str(t))
t = {foo2()}
print(get_str(t))

t = {foo0(), foo2(), 4}
print(get_str(t))

function foo(i)
    if i == 0 then return foo0()
    elseif i == 1 then return foo1()
    elseif i == 2 then return foo2()
    end
end

print(foo(1))
print(foo(2))
print(foo(0))
print(foo(3))

print((foo0()))
print((foo1()))
print((foo2()))

-- 6.2 可变长参数函数
function add(...)
    local s = 0
    for _, v in  ipairs{...} do
        s = s + v
    end
    return s
end
print(add(1, 2, 3))
print(add(3, 4, 10, 25, 12))

local a, b = ...
print(a, b)

function id (...) return  ... end

print(id(1, 2, 3))

function foo1(...)
    print("calling foo:", ...)
    return foo(...)
end

function fwrite(fmt, ...)
    return io.write(string.format(fmt, ...))
end

fwrite("first: %s, second: %s, third: %s\n", 1, 2, 3)

function print_args(...) 
    fwrite("print_args begin ...\n")
    for k, v in  ipairs{...} do
        fwrite("k: %s, v = %s\n", k, v)
    end
    fwrite("print_args end ...\n")
end

print_args(1, 2, 3)
print_args(1, nil, 3)
print_args(1, 2, nil)

function nonils(...)
    local arg = table.pack(...)
    for i = 1, arg.n do
        if arg[i] == nil then return false end
    end
    return true
end

print(nonils(2, 3, nil))
print(nonils(2, 3))
print(nonils())
print(nonils(nil))

print(select(1, "a", "b", "c"))
print(select(2, "a", "b", "c"))
print(select(3, "a", "b", "c"))
print(select("#", "a", "b", "c"))

function add(...)
    local s = 0
    for i = 1, select("#", ...) do
        s = s + select(i, ...)
    end
    return s
end
print(add(3, 4, 10, 25, 12))

function print_args_select(...) 
    fwrite("print_args_select begin ...\n")
    for i = 1, select("#", ...) do
        fwrite("k = %s, v = %s\n", i, select(i, ...))
    end
    fwrite("print_args_select end ...\n")
end

print_args_select(1, 2, 3)
print_args_select(1, 2, nil)
print_args_select(1, nil, 3)
print_args_select(nil)

-- 6.3 函数table.unpack
print(table.unpack{10, 20, 30})
a, b = table.unpack{10, 20, 30}
print(a, b)

print(string.find("hello", "ll"))
f = string.find
a = {"hello", "ll"}
print(f(table.unpack(a)))

print(table.unpack({"Sun", "Mon", "Tue", "Wed"}, 2, 3))

function unpack(t, i, n)
    i = i or 1
    n = n or #t
    if i <= n then
        return t[i], unpack(t, i + 1, n)
    end
end

print(table.unpack({1, 2, 3}))
print(table.unpack({1, 2, nil}))
print(table.unpack({1, nil, 3}))
print(table.unpack({nil, 2, 3}))
print(table.unpack({nil}))
print(table.unpack({1, nil, nil, nil, nil, nil, 10}))

print(unpack({1, 2, 3}))
print(unpack({1, 2, nil}))
print(unpack({1, nil, 3}))
print(unpack({nil, 2, 3}))
print(unpack({nil}))
print(unpack({1, nil, nil, nil, nil, nil, 10}))

-- 6.4 正确的尾调用
function f(x) x = x + 1; return g(x) end

function foo(n)
    if n > 0 then return foo(n - 1) end
end

-- 习题6.6后面可以研究一下



