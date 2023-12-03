module("base", package,.seeall)
function new()
    local obj = {}
    setmetatable(obj, {__index = base})
    return obj
end


