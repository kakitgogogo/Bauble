--mysin(1)

package.path = package.path..';./?'

print(package.path)

mylib = require("mylib")

t = mylib.dir("/home/kakit/bin")

for k, v in pairs(t) do print(k, v) end