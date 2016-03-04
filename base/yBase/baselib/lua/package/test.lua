
package.path = package.path..";D:\\work\\base\\yBase\\baselib\\lua\\package\\?.lua"
require "calc\\calc"

--print((package.path));
local a = 1;
--print(Test(1, 3));
--print((Calc:Add(2, 5)));
--print((Calc:Status()));

g_FileName = "c:\\scanresult\\task\\1234\\test.rar\\2\\2.COM Infection: Friday_the_13th.416.B";

local sName = string.match(g_FileName, "(.+)\\");
print(sName);
