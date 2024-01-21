io.write("sin(3) = ", math.sin(3), "\n")
io.write(string.format("sin(3) = %.4f\n", math.sin(3)))

-- for count = 1, math.huge do
--     local line = io.read("L")
--     if line == nil then break end
--     io.write(string.format("%6d ", count), line)
-- end

-- local count = 0
-- for line in io.lines() do
--     count = count + 1
--     io.write(string.format("%6d ", count), line, "\n")
-- end

-- local lines = {}

-- for line in io.lines() do
--     lines[#lines + 1] = line
-- end

-- table.sort(lines)

-- for _, l in pairs(lines) do
--     io.write(l, "\n")
-- end

-- while true do
--     local block = io.read(2^13)
--     if not block then break end
--     io.write(block)
-- end

-- 7.2 完整I/O模型
print(io.open("non-existent-file", "r"))
print(io.open("/etc/passwd", "w"))
-- local f = assert(io.open("non-existent-file", "r"))

local f = assert(io.open("lua_io.lua", "r"))
local t = f:read("a")
print(t)
f:close()

-- local temp = io.input()
-- io.input("newinput")
-- io.input():close()
-- io.input(temp)

-- for block in io.input():lines(2^13) do
--     io.write(block)
-- end

-- 7.3  其他文件操作
function fsize(file)
    local current = file:seek()
    print("fsize, current = ", current)
    local size = file:seek("end")
    file:seek("set", current)
    print("fsize, size = ", size)
    return size
end

local f = assert(io.open("lua_io.lua", "r"))
fsize(f)

-- 7.4 其他系统调用
print(os.getenv("HOME"))

function createDir(dirname)
    os.execute("mkdir" .. dirname)
end

local f = io.popen("ls .", "r")
local dir = {}
for entry in f:lines() do
    dir[#dir + 1] = entry
    print(entry)
end

local subject = "some news"
local address = "3226779785@qq.com"
local cmd = string.format("mail -s '%s' '%s'", subject, address)
local f = io.popen(cmd, "w")
f:write([[
    Nothing important to say.
    --me
]])
f:close()

-- 后面可以了解一下 LuaFileSyatem 和 luaposix 两个库