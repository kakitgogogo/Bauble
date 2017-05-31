package.path = package.path..';./?.lua'
package.cpath = package.cpath..';./?.so'

array = require("array")

a = array.new(1000)
print(a)
print(array.size(a))

for i = 1, 1000 do 
	array.set(a, i, i%5==0)
end

print(array.get(a, 10))

print(a:get(10))

--array.get(io.stdin, 100)


--[[
local metaarray = getmetatable(array.new(1))
metaarray.__index = metaarray
metaarray.set = array.set
metaarray.get = array.get
metaarray.size = array.size
]]--




array = require("array2")

a = array.new(1000)
print(a)

print(#a)

print(a[10])

a[10] = false

print(a[10])