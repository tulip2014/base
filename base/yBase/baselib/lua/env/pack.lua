

function testbar(env)
	local _ENV = env;
	_ENV.x = 1;
	return function ()
		return _ENV.x;
	end
end

local env = {};
local f = testbar(env);
print(_ENV.x);
print(f());