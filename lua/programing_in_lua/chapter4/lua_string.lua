a = "one string"
b = string.gsub(a, "one", "another")
print(a)
print(b)

a = "hello"
print(#a)
print(#"good bye")

print("AL0\n123\"")
print('\x41L0\10\04923"')

print("\u{3b1} \u{3b2} \u{3b3}")

page = [[
    <html>
    <head>
        <title>An HTML Page</title>
    </head>
    <body>
        <a href="http://www.lua.org">Lua</a>
    </body>
    </html>
]]

print(page)

data = "\x00\x01\x02\x03\x04\x05\x06\z
        \x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"
print(data, #data)

print(10 .. 20)

print("10" + 1)
print(tonumber("    -3 "))
print(tonumber(" 10e4 "))
print(tonumber("10e"))
print(tonumber("0x1.3p-4"))

print(tonumber("100101", 2))
print(tonumber("fff", 16))
print(tonumber("-ZZ", 36))
print(tonumber("987", 8))

print(tostring(10) == "10")
print(string.rep("abc", 3))
print(string.reverse("A Long Line!"))
print(string.lower("A Long Line!"))
print(string.upper("A Long Line!"))

s = "[in brackets]"
print(string.sub(s, 2, -2))
print(string.sub(s, 1, 1))
print(string.sub(s, -1, -1))

print(string.char(97))
i = 99; print(string.char(i, i + 1, i + 2))
print(string.byte("abc"))
print(string.byte("abc", 2))
print(string.byte("abc", -1))
print(string.byte("abc", 1, 2))

print(string.format("x = %d, y = %d", 10, 20))
print(string.format("x = %x", 200))
print(string.format("x = 0x%X", 200))
print(string.format("x = %f", 200))
tag, title = "h1", "a title"
print(string.format("<%s>%s</%s>", tag, title, tag))

print(string.format("pi = %.4f", math.pi))
d = 5; m = 11; y = 1990
print(string.format("%02d/%02d/%04d", d, m, y))

print(string.find("hello world", "wor"))
print(string.find("hello world", "war"))

print(string.gsub("hello world", "l", "."))
print(string.gsub("hello world", "ll", ".."))
print(string.gsub("hello world", "a", "."))

print(utf8.len("résumé"))
print(utf8.len("你好"), string.len("你好"))
print(utf8.char(114, 233, 115, 117, 109, 233))
print(utf8.codepoint("résumé", 6, 7))