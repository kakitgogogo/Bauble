function expand(s)
    return (string.gsub(s, "$(%w+)", _G))
end

name = "Lua"; status = "great"
print(expand("$name is $status"))


function expand(s)
    return (string.gsub(s, "$(%w+)", function(n)
        return tostring(_G[n])
    end))
end

print(expand("print=$print; a=$a"))


function toxml(s)
    s = string.gsub(s, "\\(%a+)(%b{})", function(tag, body)
        body = string.sub(body, 2, -2)
        body = toxml(body)
        return string.format("<%s>%s</%s>", tag, body, tag)
    end)
    return s
end

s = [[\title{the \bold{big} example}]]
print(toxml(s))