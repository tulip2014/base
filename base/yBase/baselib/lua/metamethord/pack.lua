
local oldload = load;
local oldprint = print;

function packprint()
	print("_G:", _G.y);
	print("_ENV:", _ENV.y);
--[[	for i,v in pairs(_ENV) do
		print(i, type(v))
	end--]]
end

x = 1;
print(_ENV.y);

--local _tenv = _ENV;
_tenv = {};
oldprint(_tenv);
oldprint(_ENV);
_tenv.a = 1;
_tenv.tprint = oldprint;
oldprint(_tenv);
--_ENV = {};

local a = oldload("tprint('qwe');y = 2;", "test", "t", _tenv);
a();
--load("print(y)", "test", nil, _tenv);

oldprint(_ENV.y);
oldprint(_tenv.y);
oldprint(_tenv);
oldprint(_ENV);