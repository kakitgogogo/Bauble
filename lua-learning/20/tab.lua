function expandTabs(s, tab)
    tab = tab or 8
    local corr = 0
    s = string.gsub(s, "()\t", function(p)
        local sp = tab  - (p-1+corr)%tab
        return string.rep(" ", sp)
    end)
    return s
end

