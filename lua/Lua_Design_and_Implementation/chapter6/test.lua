module("test", package.seeall)
setmetatable(test, {__index = base})
function new()
    local obj = {}
    setmetatable(obj, {__index = test})
    return obj
end