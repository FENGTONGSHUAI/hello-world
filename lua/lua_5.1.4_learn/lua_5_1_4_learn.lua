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

------------------------------ lmathlib.c ------------------------------
function test_lmathlic()
    print("test lmathlib.c begin")
    print(math.pi, type(math.pi))
    print(math.huge, type(math.huge))
    print(math.fmod, type(math.fmod))
    print("test lmathlib.c end")
end
------------------------------ lmathlib.c ------------------------------

------------------------------ lstrlib.c ------------------------------


local function base_random_str(s, len)
    local letter_list = {}
    local cur_index
    local s_len = s:len()
    while(len > 0) do
        cur_index = math.random(1, s_len)
        table.insert(letter_list, s:sub(cur_index, cur_index))
        len = len - 1
    end
    return table.concat(letter_list)
end


local function get_random_str(len)
    local s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstovwxyx1234567890"
    return base_random_str(s, len)
end

local function test_str_sub()
    local s = "abcdefg"
    local len = s:len()
    print("s = ", s)
    print("s:len = ", len)
    print("s:sub(1, 1) = ", s:sub(1, 1))
    print("s:sub(1, 3) = ", s:sub(1, 3))
    print("s:sub(1, len) = ", s:sub(1, len))
    print("s:sub(1, len + 1) = ", s:sub(1, len + 1))
    print("s:sub(0, len) = ", s:sub(0, len))
    print("s:sub(-1, len) =  ", s:sub(-1, len))
    print("s:sub(2, 1) = ", s:sub(2, 1))
    print("s:sub(-2, len) = ", s:sub(-2, len))
end

local function test_str_reverse()
    local s = get_random_str(5)
    print(debug.getinfo(1).name, "s = ", s)
    local r_s = s:reverse()
    print(debug.getinfo(1).name, "r_s = ", r_s)
end

local function get_upper_letter_random_str(len)
    local s = "ABCDEFGHIJKLMNOPQRSTUVWXY"
    return base_random_str(s, len)
end

local function test_str_lower()
    local s = get_upper_letter_random_str(5)
    print(debug.getinfo(1).name, "s = ", s)
    local l_s = s:lower()
    print(debug.getinfo(1).name, "l_s = ", l_s)
end


local function get_lower_letter_random_str(len)
    local s = "abcdefghijklmnopqrstovwxyx"
    return base_random_str(s, len)
end

local function test_str_upper()
    local s = get_lower_letter_random_str(5)
    print(get_debug_info(), "s = ", s)
    local u_s = s:upper()
    print(get_debug_info(), "u_s = ", u_s)
end


local function test_str_rep()
    local s = get_random_str(5)
    print(get_debug_info(), "s = ", s)
    local three_s = s:rep(3)
    print(get_debug_info(), "three_s = ", three_s)
    three_s = s:rep(3, "; ")
    print(get_debug_info(), "three_s = ", three_s)
    local i = 10
    local three_i = string.rep(i, 3)
    print(get_debug_info(), "three_i = ", three_i)
    three_i = string.rep(i, 3, "; ")
    print(get_debug_info(), "three_i = ", three_i)
    -- local three_nil = string.rep(nil, 3)
    -- print(get_debug_info(), "three_nil = ", three_i)
    -- local t = {a = 1, b = 2}
    -- local three_t = string.rep(t, 3)
    -- print(get_debug_info(), "three_t = ", three_t)
end

local function test_str_byte()
    local s = get_random_str(10)
    print(get_debug_info(), "s = ", s)
    print(get_debug_info(), "s:byte(1) = ", s:byte(1))
    print(get_debug_info(), "s:byte(1, 10) = ", s:byte(1, 10))
    print(get_debug_info(), "s:byte(1, 9) = ", s:byte(1, 9))
    print(get_debug_info(), "s:byte(-20, 20) = ", s:byte(-20, 20))
end

local function test_str_char()
    print(get_debug_info(), "string.char()", string.char())
    print(get_debug_info(), "string.char(48)", string.char(48))
    print(get_debug_info(), "string.char(48, 49, 50, 65, 97)", string.char(48, 49, 50, 65, 97))
    print(get_debug_info(), "string.char(48, 49, 50, 0, 65, 97)", string.char(48, 49, 50, 0, 65, 97))
    print(get_debug_info(), "string.char(48, 49, 50, 255, 65, 97)", string.char(48, 49, 50, 255, 65, 97))
    print(get_debug_info(), "string.char(48, 49 ,50, 257, 65, 97)", string.char(48, 49, 50, 257, 65, 97))
end

-- 参考 https://www.jianshu.com/p/d1ea3e979ae8
local function test_str_dump()
    --自定义一个函数
    function custom_func(num1, num2)
        local ret = num1 + num2;
        print("\nnum1 = "..num1)
        print("num2 = "..num2)
        print("num1 + num2 = "..ret)
    end

    -- 将函数序列化
    local func_content = string.dump(custom_func)
    print("\nfunc_content = "..func_content)

    -- 将转化后的字符串写入文件
    local outfile = io.open("dumptest.txt", "w")
    local outnum = outfile:write(func_content)
    outfile:close()

    -- 从文件总读取内容
    local infile = io.open("dumptest.txt", "r")
    local incontent = infile:read("*a")
    infile:close()
    print("\ninput content is "..incontent)

    -- 加载函数
    local myfunc = loadstring(incontent)

    -- 执行函数
    myfunc(1, 1)

    myfunc(3, 6)

    -- 尝试输出字符串
    myfunc("7" ,".7")

    print(os.date())
end

function test_lstrlibc()
    -- test_str_sub()
    -- test_str_reverse()
    -- test_str_lower()
    -- test_str_upper()
    -- test_str_rep()
    -- test_str_byte()
    -- test_str_char()
    test_str_dump()
end

------------------------------ lstrlib.c    ------------------------------

function main()
    -- test_lmathlic()
    test_lstrlibc()
end


main()
