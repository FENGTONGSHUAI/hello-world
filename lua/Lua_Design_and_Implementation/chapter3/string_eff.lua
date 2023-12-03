local a = os.clock()
local s = ''
for i = 1, 300000 do
    s = s .. 'a'
end
local b = os.clock()
print(string.format("使用 .. 连接字符串, 花费时间: %s", b - a))

a = os.clock()
s = ''
local t = {}
for i = 1, 300000 do
    t[#t + 1] = 'a'
end
s = table.concat(t, '')
b = os.clock()
print(string.format("使用 table.concat 连接字符串, 花费时间: %s", b - a))


