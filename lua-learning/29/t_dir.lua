package.path = package.path..';./?.lua'
package.cpath = package.cpath..';./?.so'

require("dir")

for fname in dir(".") do print(fname) end