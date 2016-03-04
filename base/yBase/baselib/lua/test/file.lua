
function RemoveSpace(src)
	return string.match(src, "^%s*(.-)%s*$")
end


local a = "		1	123           "
local b = "123"
--print(a)
--print(b)
local ret = RemoveSpace(a)
print(ret)
print(string.len(ret))