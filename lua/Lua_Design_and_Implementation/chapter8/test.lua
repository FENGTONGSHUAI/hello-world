-- setfenv(1, {})
-- print(a)

print(_G)
print(_G["_G"])

myprint = print
myprint("1")
module("test", package.seeall)
myprint("2")

-- ../lua-5.1.4-master/lua test.lua