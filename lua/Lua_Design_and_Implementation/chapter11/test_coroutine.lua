function foo(a)
    print("foo", a)
    return coroutine.yield(2 * a)
end

co = coroutine.create(function (a, b)
    print("co_body", a, b)
    local r = foo(a + 1)
    print("co-body", r)
    local r, s = coroutine.yield(a + b, a - b)
    print("co-body", r, s)
    return b, "end"
end)

print("main", coroutine.resume(co, 1, 10))
print("------")
print("main", coroutine.resume(co, r))
print("------")
print("main", coroutine.resume(co, "x", "y"))
print("------")
print("main", coroutine.resume(co, "x", "y"))

-- ../lua-5.1.4-master/lua test_coroutine.lua