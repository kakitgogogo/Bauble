local counters = {}
local names = {}

local function hook()
    local f = debug.getinfo(2, "f").func
    if counters[f] == nil then
        counters[f] = 1
        names[f] = debug.getinfo(2, "Sn")
    else
        counters[f] = counters[f] + 1
    end
end

local f = assert(loadfile(arg[1]))
debug.sethook(hook, "c")
f()
debug.sethook()

function getname(func)
    local n = names[func]
    if n.what == "C" then
        return n.name
    end
    local lc = string.format("[%s]:%s", n.short_src, n.linedefined)
    if n.namewhat ~= "" then
        return string.format("%s (%s)", lc, n.name)
    else
        return ls
    end
end

for func, count in pairs(counters) do
    print(getname(func), count)
end