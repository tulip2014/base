
--package.path = "D:\\work\\base\\yBase\\baselib\\lua\\metamethord\\pack.lua";
--require "pack"



local oldprint = print;
local oldload = load;
local oldpairs = pairs;
local _tenv = {};
_tenv.tprint = print;
_tenv.tpairs = pairs;


oldprint(_ENV);
oldprint(_ENV._G);
oldprint(_G);

local _tg = _G;
_G = {};
_ENV = _tenv;
x = 1;
for i,v in tpairs(_ENV) do
	tprint(i);
end

oldprint(_ENV);
oldprint(_G);

local a = oldload("y = 1; for i,v in tpairs(_ENV) do tprint(i); end", "test", "t", _tenv);
a()


;
local b = oldload("y = y + 1;", "test", "t", _tenv);
b();

oldprint(_tenv.y);
oldprint(_ENV.y);
oldprint(_tg.y)
