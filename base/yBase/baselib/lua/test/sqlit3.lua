
require "luasql.sqlite3"

function test()
	env = assert(luasql.sqlite3());
	print(123);
	return 1;
end

local iRet = test();