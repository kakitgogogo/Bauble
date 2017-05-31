s = "ASasfdsfgDSG"
print(s:lower())

s = "[rfdsf]"
print(s:sub(2, -2))

print(string.char(97))

i = 99
print(string.char(i, i + 1, i + 2))

print(string.byte("abc"))
print(string.byte("abc", 2))


print(string.format("pi = %.4f", math.pi))