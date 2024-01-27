-- 8.1 局部变量和代码块
x = 10
local i = 1

while i <= x do
    local x = i * 2
    print(x)
    i = i + 1
end

if i > 20 then
    local x 
    x = 20
    print(x + 2)
else
    print(x)
end

print(x)

local x1, x2
do  
    local a, b, c, d = 1, 10, 1, 1
    local a2 = 2 * a
    local d = (b^2 - 4 * a * c)^(1/2)
    x1 = (-b + d)/a2
    x2 = (-b - d)/a2
end
print(x1, x2)

local a, b = 1, 10
if a < b then
    print(a)
    local a
    print(a)
end
print(a, b)

-- 8.2 控制结构
local x = 2
local sqr = x / 2
repeat
    sqr = (sqr + x/sqr) / 2
    local error = math.abs(sqr^2 - x)
until error < x / 1000

for i = 1, math.huge do
    if(0.3*i^3 - 20*i^2 - 500 >= 0) then
        print(i)
        break
    end
end

for i = 1, 10 do print(i) end
print(i)

for j = 1, 10 do print(i) end
print(j)

local function test_func()
    do return end
    local a, b = 1, 2
    return a + b
end

-- ::s1:: do
--     local c = io.read(1)
--     if c == '0' then goto s2
--     elseif c == nil then print 'ok'; return 
--     else goto s1
--     end
-- end

-- ::s2:: do
--     local c = io.read(1)
--     if c == '0' then goto s1
--     elseif c == nil then print 'not ok'; return
--     else goto s2
--     end
-- end

-- goto s1

goto room1 

::room1:: do
    local move = io.read()
    if move == "south" then goto room3
    elseif move == "east" then goto room2
    else
        print("invalid move")
        goto room1
    end
end

::room2:: do
    local move = io.read()
    if move == "south" then goto room4
    elseif move == "wast" then goto room1
    else
        print("invalid move")
        goto room2
    end
end


::room3:: do
    local move = io.read()
    if move == "north" then goto room1
    elseif move == "east" then goto room4
    else
        print("invalid move")
        goto room3
    end
end

::room4:: do
    print("Congratulations, you won!")
end