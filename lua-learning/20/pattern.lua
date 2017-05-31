s = "hello world"

i, j = string.find(s, "world")
print(i, j)
print(s:sub(i, j))

s = "asd\nsadfsd\nasdfgdsfgdg\n"
local t = {}
local i = 0
while true do
    i = string.find(s, "\n", i+1)
    if i == nil then break end
    t[#t + 1] = i
end

for k,v in pairs(t) do print(v) end

print(string.match("hello world", "hello"))

data = "today is 1/1/1111"
d = string.match(data, "%d+/%d+/%d+")
print(d)

s = string.gsub("Lua is cute", "cute", "great")
print(s)
s = string.gsub("all lll", "l", "x")
print(s)
s = string.gsub("all lll", "l", "x", 2)
print(s)

cnt = select(2, string.gsub("all lll", "l", "x", 2))
print(cnt)


words = {}
for w in string.gmatch(s, "%a+") do
    words[#words + 1] = w
    print(w)
end

function search(modname, path)
    modname = string.gsub(modname, "%.", "/")
    for c in string.gmatch("[^;]+") do
        local fname = string.gsub(c, "?", modname)
        local f = io.open(fname)
        if f then
            f:close()
            return fname
        end
    end
    return nil
end



print(string.gsub("hello, up-down~", "%A", "."))

test = "int x; /* x */ int y; /* y */"
print(string.gsub(test, "/%*.-%*/", ""))


num = "-12asfdsfdgfjd+11"
for s in string.gmatch(num, "[+-]?%d+") do print(s) end

s = "+12423543"
if string.find(s, "^[+-]?%d+$") then print(s) end

s = "a ((asdas) asd asdas ) line"
print(s.gsub(s, "%b()", ""))