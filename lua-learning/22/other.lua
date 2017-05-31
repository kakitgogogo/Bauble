print(os.getenv("HOME"))

function create(dirname)
    os.execute("mkdir "..dirname)
end

create("oooo")

os.remove("oooo")

