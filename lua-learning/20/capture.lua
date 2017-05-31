pair = "name = Anna"
key, value = string.match(pair, "(%a+)%s*=%s*(%a+)")

print(key, value)

date = "Today is 17/5/1990"
d, m ,y = string.match(date, "(%d+)/(%d+)/(%d+)")
print(d, m ,y)


s = [[then he said: "it is all right"!]]
q, quotePart = string.match(s, "([\"'])(.-)%1")
print(q, quotePart)

p = "%[(=*)%[(.-)%]%1%]"
s = "a = [=[ [[something]] ]=]"
print(string.match(s, p))


print(string.gsub("hello", "%a", "%0-%0"))

print(string.gsub("hello lua", "(.*)%s(.*)", "%2 %1"))


s = [[\comment{some text}]]
s = string.gsub(s, "\\(%a+){(.-)}", "<%1>%2</%1>")
print(s)


function trim(s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

s = " asd  "
print(trim(s))

