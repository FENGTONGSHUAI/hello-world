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

s = "hello world"
i, j = string.find(s, "hello")
print(i, j)
print(string.sub(s, i, j))
print(string.find(s, "world"))
i, j = string.find(s, "l")
print(i, j)
print(string.find(s, "lll"))

print(string.match("hello world", "hello"))

date = "Today is 17/7/1990"
d = string.match(date, "%d+/%d+/%d+")
print(d)

s = string.gsub("Lua is cute", "cute", "great")
print(s)
s = string.gsub("all lii", "l", "x")
print(s)
s = string.gsub("Lua is great", "Sol", "Sun")
print(s)
s = string.gsub("all lii", "l", "x", 1)
print(s)
s = string.gsub("all lii", "l", "x", 2)
print(s)

s = "some string"
words = {}
for w in string.gmatch(s, "%a+") do
    words[#words + 1] = w
end

print(get_str(words))

s = "Deadline is 30/05/1999, firm"
date = "%d%d/%d%d/%d%d%d%d"
print(string.match(s, date))

print(string.gsub("hello, up-down!", "%A", "."))
print(string.gsub("one, and two; and three", "%a+", "word"))

print(string.match("the number 1298 is even", "%d+"))

test = "int x; /* x */; int y; /* y */"
print(string.gsub(test, "/%*.*%*/", ""))
test = "int x; /* x */; int y; /* y */"
print(string.gsub(test, "/%*.-%*/", ""))

s = "a (conclosed (in) parentheses) line"
print(string.gsub(s, "%b()", "1"))

s = "the anthem is the theme"
print(string.gsub(s, "%f[%w]the%f[%W]", "one"))

pair = "name = Anna"
key, value = string.match(pair, "(%a+)%s*=%s*(%a+)")
print(key, value)

date = "Today is 17/7/1990"
d, m, y = string.match(date, "(%d+)/(%d+)/(%d+)")
print(d, m, y)


date2 = "Today is 17/7/1990  17/12/2023"
print(string.gmatch(date2, "(%d+)/(%d+)/(%d+)"))
for d, m, y in string.gmatch(date2, "(%d+)/(%d+)/(%d+)") do
    print(d, m, y)
end

s = [[then he said: "it's all right"]]
q, quotedPart = string.match(s, "([\"'])(.-)%1")
print(quotedPart)
print(q)

p = "%[(=*)%[(.-)%]%1%]"
s = "a = [=[[[ something ]] ]==] ]=]; print(a)"
print(string.match(s, p))

print(string.gsub("hello Lua!", "%a", "%0-%0"))
print(string.gsub("hello Lua!", "(.)(.)", "%2%1"))

s = [[the \quote{task} is to \em{change} that.]]
s = string.gsub(s, "\\(%a+){(.-)}", "<%1>%2</%1>")
print(s)

function trim(s)
    s = string.gsub(s, "^%s*(.-)%s*$", "%1")
    return s
end

s = "   11111    "
print(trim(s))

function expand(s)
    return (string.gsub(s, "$(%w+)", _G))
end

name = "Lua"; status = "great"
print(expand("$name is $status, isn't it?"))
print(expand("$othername is $status, isn't it?"))

function expand2(s)
    return (string.gsub(s, "$(%w+)", function (n)
        return tostring(_G[n])
    end))
end

print(expand2("print = $print; a = $a"))

function toxml(s)
    s = string.gsub(s, "\\(%a+)(%b{})", function (tag, body)
        body = string.sub(body, 2, -2)
        body = toxml(body)
        return string.format("<%s>%s</%s>", tag, body, tag)
    end)
    return s
end

print(toxml("\\title{The \\bold{big} example}"))

function unescape(s)
    s = string.gsub(s, "+", " ")
    s = string.gsub(s, "%%(%x%x)", function (h)
        return string.char(tonumber(h, 16))
    end)
    return s
end

print(unescape("a%2Bb+%3D+c"))

cgi = {}
function decode(s)
    for name, value in string.gmatch(s, "([^&=]+)=([^&=]+)") do
        name = unescape(name)
        value = unescape(value)
        cgi[name] = value
    end    
end

function escape(s)
    s = string.gsub(s, "[&=+%%%c]", function (c)
        return string.format("%%%02X", string.byte(c))
    end)
    s = string.gsub(s, " ", "+")
    return s
end

function encode(t)
    local b = {}
    for k, v in pairs(t) do
        b[#b + 1] = (escape(k) .. "=" .. escape(v))
    end
    return table.concat(b, "&")
end

t = {name = "al", query = "a+b = c", q = "yes or no"}
print(encode(t))

print(string.match("hello", "()ll()"))
print(string.find("hello", "ll"))

function expandTabs(s, tab)
    tab = tab or 8
    local corr = 0
    s = string.gsub(s, "()\t",function (p)
        local sp = tab - (p - 1 + corr) % tab
        corr = corr - 1 + sp
        return string.rep(" ", sp)
    end)
    return s
end

function unexpandTabs(s, tab)
    tab = tab or 8
    s = expandTabs(s, tab)
    local pat = string.rep(".", tab)
    s = string.gsub(s, pat, "%0\1")
    s = string.gsub(s, " +\1", "\t")
    s = string.gsub(s, "\1", "")
    return s
end

a = "111111  11111111 1111111 11111111"
b = "1\t11\t11111111\t111111111\t"
print(string.gsub(expandTabs(b), "\t", "\\t"))
print(string.gsub(unexpandTabs(a), "\t", "\\t"))

test = [[char s[] = "a /* here"; /* a tricky string */]]
print(string.gsub(test, "/%*.-%*/", "<COMMENT>"))

i, j = string.find(";$%  **#$hello13", "%a*")
print(i, j)

pattern = string.rep("[^\n]", 70) .. "+"

function nocase(s)
    s = string.gsub(s, "%a", function (c)
        return "[" .. string.lower(c) .. string.upper(c) .. "]"
    end)
    return s
end

print(nocase("Hi there!"))

function code(s)
    return (string.gsub(s, "\\(.)", function (x)
        return string.format("\\%03d", string.byte(x))        
    end))
end

function decode(s)
    return (string.gsub(s, "\\(%d%d%d)", function (d)
        return "\\" .. string.char(tonumber(d))
    end))
end

s = [[follows a typical string: "This is \"great\"!".]]
s = code(s)
print(s)
s = string.gsub(s, '".-"', string.upper)
print(s)
s = decode(s)
print(s)




