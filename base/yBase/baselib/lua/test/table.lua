
Set = {}

Set.mt = {};

function Set.new(t)
	local tmp = t;
	setmetatable(tmp, Set.mt);
	return tmp;
end

function Set.union(a, b)
	local res = Set.new({});
	for k in pairs(a) do res[k] = true end;
	for k in pairs(b) do res[k] = true end;
	return res;
end

function Set.print(a)
	for i, v in pairs(a) do
		print(i, v);
	end
end

local a = {["3"] = true, };
local b = {["3"] = true, };

local sa = Set.new(a);
local sb = Set.new(b);

Set.mt.__add = Set.union;

local c = sa + sb;
Set.print(c);