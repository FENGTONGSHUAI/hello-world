days = {"Sunday", "Monday", "Tuesday", "Wednesday", 
    "Thursday", "Friday", "Staturday"}
print(days[4])

w = {x = 0, y = 0, label = "console"}
x = {math.sin(0), math.sin(1), math.sin(2)}
w[1] = "another field"
x.f = w
print(w["x"])
print(w[1])
print(x.f[1])
w.x = nil

polyline = {color = "blue",
            thickness = 2,
            npoints = 4,
            {x = 0, y = 0},
            {x = -10, y = 0},
            {x = -10, y = 1},
            {x = 0, y = 1},
            }

print(polyline[2].x)
print(polyline[4].y)

opnames = {
    ["+"] = "add",
    ["-"] = "sub",
    ["*"] = "mil",
    ["/"] = "div"
}

i = 20; s = "-"
a = {[i+0] = s, [i+1] = s..s, [i+2] = s..s..s}
print(opnames[s])
print(a[22])

a = {}
for i = 1, 10 do
    -- a[i] = io.read()
    a[i] = i
end

for i = 1, #a do
    print(a[i])
end

print(a[#a])
a[#a] = nil
a[#a + 1] = v

t =  {10, print, x =12, k = "hi"}
for k, v in pairs(t) do
    print(k, v)
end

t = {10, print, 12, "hi"}
for k, v in ipairs(t) do
    print(k, v)
end

for k = 1, #t do
    print(k, t[k])
end

t = {}
for i = 1, 10 do
    table.insert(t, i)
end
print(#t)

table.move(t, 1, #t, 2)
print(#t)
for k = 1, #t do
    print(k, t[k])
end
table.remove(t, 1)

table.move(t, 2, #t, 1)
print(#t)
for k = 1, #t do
    print(k, t[k])
end
t[#t] = nil

b = {}
table.move(t, 1, #t, 1, b)
print(#b)
for k = 1, #b do
    print(k, b[k])
end

