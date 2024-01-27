local function get_str(var)
    local result = {}
    if type(var) == "table" then
        for k, v in pairs(var) do
            local k_v_str = string.format("%s = %s", k, get_str(v))
            table.insert(result, k_v_str)
        end
        return table.concat(result, ", ")
    end
    return string.format("%q", var)
end

-- a = {p = print}
-- a.p("Hello World")
-- print = math.sin
-- a.p(print(1))
-- math.sin = a.p
-- math.sin(10, 20)

network  = {
    {name = "grauna", IP = "210.26.30.34"},
    {name = "arraial", IP = "210.26.30.23"},
    {name = "lua", IP = "210.26.23.12"},
    {name = "derain", IP = "210.26.23.20"}
}

table.sort(network, function (a, b) return (a.name > b.name) end)
print(get_str(network))

function derivative(f, delta)
    delta = delta or 1e-4
    return function (x)
        return (f(x + delta) - f(x)) / delta
    end
end

c = derivative(math.sin)
print(math.cos(5.2), c(5.2))
print(math.cos(10), c(10))

-- 9.2 非全局函数
Lib = {}
Lib.foo = function(x, y)
    return x + y
end
Lib.goo = function(x, y)
    return x - y
end

print(Lib.foo(2, 3), Lib.goo(2, 3))

local fact
fact = function(n)
    if n == 0 then return 1
    else return n * fact(n - 1)
    end
end

print(fact(5))

-- 9.3 词法界定
names = {"Peter", "Paul", "Mary"}
grades = {Mary = 10, Paul = 7, Peter = 8}
function sortbygrade(names, grades)
    table.sort(names, function (n1, n2)
        return grades[n1] > grades[n2]
    end)
end
print("before names: ", get_str(names))
sortbygrade(names, grades)
print("after names: ", get_str(names))

function newCounter()
    local count = 0
    return function ()
        count = count + 1
        return count
    end
end

c1 = newCounter()
print(c1())
print(c1())

c2 = newCounter()
print(c2())
print(c1())
print(c2())

-- 如果重新加载就会出错了
do 
    local oldSin = math.sin
    local k = math.pi / 180
    math.sin  = function(x)
        return oldSin(x * k)
    end
end

do
    local oldOpen = io.open
    local access_OK = function(filename, mode)
        return true
    end

    io.open = function(filename, mode)
        if access_OK(filename, mode) then
            return oldOpen(filename, mode)
        else
            return nil, "access denied"
        end
    end
end

-- 9.4 小试函数式编程
function disk1(x, y)
    return (x - 1.0)^2 + (y - 3.0)^2 <= 4.5^2
end

function disk(cx, cy, r) 
    return function (x, y)
        return (x - cx)^2 + (y - cy)^2 <= r^2
    end
end

function rect(left, right, bottom, up)
    return function (x, y)
        return left <= x and x <= right and
            bottom <= y and y <= up
    end
end

function complement(r)
    return function (x, y)
        return not r(x, y)
    end
end

function union(r1, r2)
    return function (x, y)
        return r1(x, y) or r2(x, y)
    end
end

function intersection(r1, r2)
    return function (x, y)
        return r1(x, y) and r2(x, y)
    end
end

function difference(r1, r2)
    return function (x, y)
        return r1(x, y) and not r2(x, y)
    end
end

function translate(r, dx, dy)
    return function (x, y)
        return r(x - dx, y - dy)
    end
end

function plot(r, M, N)
    local file=io.open("test.txt","a")
    io.output(file)-- 设置默认输出文件
    io.write("P1\n", M, " ", N, "\n")
    for i = 1, N do
        local y = (N - i * 2) / N
        for j = 1, M do
            local x = (j * 2 - M)/M
            io.write(r(x, y) and "1" or "0")
        end
        io.write("\n")
    end
    io.close()
end

c1 = disk(0, 0, 1)
plot(difference(c1, translate(c1, 0.3, 0)), 500, 500)
-- 可以在这个网址查看  https://imagetostl.com/cn/view-pbm-online
