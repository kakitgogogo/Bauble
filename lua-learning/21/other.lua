function fsize(filename)
    local file = io.open(filename, "r")
    local current = file:seek()
    local size = file:seek("end")
    file:seek("set", current)
    return size
end

print(fsize("improve.lua"))

f = io.tmpfile()
f:flush()