io.write(string.format("sin(3) = %.4f\n", math.sin(3)))

t = io.read("*all")
print(t)

t = io.read("*line")
print(t)

n = io.read("*number")
print(n)

local pat = "(%S+)%s+(%S+)%s+(%S+)%s+"
for n1, n2, n3 in string.gmatch(io.read("*all"), pat) do
    print(math.max(tonumber(n1), tonumber(n2), tonumber(n3)))
end


while true do 
    local block = io.read(2^13)
    if not block then break end
    io.write(block)
end