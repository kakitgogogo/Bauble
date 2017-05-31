function getvarvalue(name)
    local value, found
    for i = 1, math.huge do
        local n, v = debug.getlocal(2, i)
        if not n then break end
        if n == name then
            value = v
            found = true
        end
    end
    if found then return value end

    local func = debug.getinfo(2, "f").func
    for i = 1, math.huge do
        local n, v = debug.getupvalue(func, i)
        if not n then break end
        if n == name then return v end
    end

    return getfenv(func)[name]
end

i = 0
print(getvarvalue("i"))


co = coroutine.create(function()
    local x = 10
    coroutine.yield()
    error("some error")
end)

coroutine.resume(co)
coroutine.resume(co)
print(debug.getlocal(co, 1, 1))
print(debug.traceback(co))