------------------------------ basefunc ------------------------------
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

local function get_debug_info()
    local info = debug.getinfo(2)
    return string.format("file:%s, line:%s, func:%s", info.short_src, info.currentline, info.name)
end

------------------------------ basefunc ------------------------------

-- 12.1 函数os.time
print(os.time())
local date = 1706415099
local day2year = 365.242
local sec2hour = 60 * 60
local sec2day = sec2hour * 24
local sec2year = sec2day * day2year

print(date // sec2year + 1970)
print(date % sec2day // sec2hour)
print(date % sec2hour // 60)
print(date % 60)

print(os.time({year = 2024, month = 1, day = 28, hour = 12, min = 45, sec = 20}))
print(os.time({year = 1970, month = 1, day = 1, hour = 0}))
print(os.time({year = 1970, month = 1, day = 1, hour = 0, sec = 1}))
print(os.time({year = 1970, month = 1, day = 1}))

-- 12.2 函数os.date
print(get_str(os.date("*t", 1706415099)))
print(os.date("a %A in %B"))
print(os.date("%d/%m/%Y", 906000490))

t = 906000490
print(os.date("%Y-%m-%d", t))
print(os.date("%Y-%m-%dT%H:%M:%S", t))
print(os.date("%Y-%j", t))
print(os.date("!%c", 0))

--12.3 日期和时间处理
t = os.date("*t")
print(os.date("%Y/%m/%d", os.time(t)))
t.day = t.day + 40
print(os.date("%Y/%m/%d", os.time(t)))

t = os.date("*t")
print(t.day, t.month)
t.day = t.day - 40
print(t.day, t.month)
t = os.date("*t", os.time(t))
print(t.day, t.month)

t = os.date("*t")
print(os.date("%Y/%m/%d", os.time(t)))
t.month = t.month + 6
print(os.date("%Y/%m/%d", os.time(t)))

local t5_3 = os.time({year = 2015, month = 1, day = 12})
local t5_2 = os.time({year = 2011, month = 12, day = 16})
local d = os.difftime(t5_3, t5_2)
print(d // (24 * 3600))

myepoch = os.time({year = 2000, month = 1, day = 1, hour = 0})
now = os.time({year = 2015, month = 11, day = 20})
print(os.difftime(now, myepoch))

T = {year = 2000, month = 1, day = 1, hour = 0}
T.sec = 501336000
print(os.date("%d/%m/%Y", os.time(T)))

local x = os.clock()
local s = 0
for i = 1, 10000000 do s = s + i end
print(string.format("elapsed time: %.2f\n", os.clock() - x))