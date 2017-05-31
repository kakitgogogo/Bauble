local filename = "simple_io.lua"

local f = assert(io.open(filename, "r"))
local t = f:read("*all")
f:close()

print(t)

io.stdout:write("haha\n")

--[[
local tmp = io.input()
io.input("newfile")
------
io.input():close()
io.input(tmp)
]]--