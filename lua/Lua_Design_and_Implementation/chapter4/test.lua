-- -- 测试预填充
-- local a = os.clock()
-- for i = 1, 2000000 do
--     local a = {}
--     a[1] = 1; a[2] = 2; a[3] = 3
-- end
-- local b = os.clock()
-- print(b - a)

-- local a = os.clock()
-- for i = 1, 2000000 do
--     local a = {1, 2, 3}
--     a[1] = 1; a[2] = 2; a[3] = 3
-- end
-- local b = os.clock()
-- print(b - a)


-- 测试 # 取长度
local a = {[1] = 1, [2] = 2, 4, 5, 6}
print(#a)

for k, v in pairs(a) do
    print(string.format("k = %s, v = %s", k, v))
end
print(a[1], a[2], a[3])
print("for C test")