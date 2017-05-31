s = [[follow a typical string: "This is \"great\"!".]]

print(s)

function code(s)
    return (string.gsub(s, "\\(.)", function(x)
        return string.format("\\%03d", string.byte(x))
    end))
end

function decode(s)
     return (string.gsub(s, "\\(%d%d%d)", function(x)
        return "\\"..string.char(x)
    end))
end

s = code(s)
print(s)
s = string.gsub(s, "\".-\"", string.upper)
s = decode(s)
print(s)