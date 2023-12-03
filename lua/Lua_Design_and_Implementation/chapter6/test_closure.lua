function test1()
    local a = 1
    function test2()
        local b = 100
        function test3()
            print(a)
            print(b)
        end
    end
    return test2
end

local fun = test1()
fun()