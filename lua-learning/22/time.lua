print(os.time{year=1970, month=1, day=1, hours=0})

t = os.date("*t", 906000490)
print(os.date("today is %A, in %B"))
print(os.date("%x", 906000490))

local x = os.clock()
local s = 0
for i=1,1000000 do s = s + i end
print(string.format("elasped time: %.2f\n", os.clock() - x))