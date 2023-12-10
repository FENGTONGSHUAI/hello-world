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
function test_lstrlibc()
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

------------------------------ lstrlib.c ------------------------------

function main()
    -- test_lmathlic()
    test_lstrlibc()
end


main()
