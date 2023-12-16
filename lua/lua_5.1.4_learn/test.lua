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


local a = {a = 1}
local b = {}
b[a] = 1

print(get_str(b))
for k, v in pairs(b) do
    print(type(k), k)
    print(type(v), v)
end
