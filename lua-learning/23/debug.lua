function traceback()
    for level = 1, math.huge do
        local info = debug.getinfo(level, "Sl")
        if not info then break end
        if info.what == "C" then
            print(level, "C function")
        else
            print(string.format("[%s]:%d", info.short_src, info.currentline))
        end
    end
end

traceback()

function foo(a, b)
    local x
    do local c = a - b end
    local a = 1
    while true do 
        local name, value = debug.getlocal(1, a)
        if not name then break end
        print(name, value)
        a = a + 1
    end
end

foo(10, 20)