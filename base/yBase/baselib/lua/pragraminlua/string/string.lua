
local a = "Atest";
local A = 65;
local B = 66;

print(string.byte(a, 1));  --得到ascii编码
print(string.char(A, B));  --得到字符，可以输入多个参数

function Add(a, b)
	return a + b;
end
print((string.dump(Add)));


--[[第三个参数，开始查找的位置，第四个参数，是否关掉魔术字]]
local c = "test .. name";
local iS, iE = string.find(c, ".", 1, true);  
print(iS, iE);


-- q 选项将一个字符串格式化为两个双引号括起，对内部字符做恰当的转义处理的字符串。 
-- 该字符串可以安全的被 Lua 解释器读回来。
local d = string.format('test: %d,%q', 10, 'a string with "quotes" and \n new line')
print(d);

--[[
x: （这里 x 不能是 魔法字符 ^$()%.[]*+-? 中的一员） 表示字符 x 自身。
.: （一个点）可表示任何字符。
%a: 表示任何字母。
%A	非字母	string.find("AB12","%A%A")	3 4
%c: 表示任何控制字符。
%d: 表示任何数字。
%g: 表示任何除空白符外的可打印字符。
%l: 表示所有小写字母。
%L	大写字母	string.find("ABab","%L%L")	1 2
%p: 表示所有标点符号。
%s: 表示所有空白字符。
%u: 表示所有大写字母。
%U	非大写字母	string.find("ABab","%U%U")	3 4
%w: 表示所有字母及数字。
%x: 表示所有 16 进制数字符号。
%X	非十六进制数字	string.find("efgh","%X%X")	3 4
--]]

s = "hello 10.12World	20from  * Lua"
for w in string.gmatch(s, "%U+") do
	print(w);
	--print(string.byte(w))
end

--[[
-- 【用[]创建字符集，"-"为连字符，"^"表示字符集的补集】
-- 交叉使用类和范围的行为未定义。 
因此，像 [%a-z] 或 [a-%%] 这样的模式串没有意义。
[^set]: 表示 set 的补集， 其中 set 如上面的解释。

+      匹配前一字符1次或多次(匹配最多)
*      匹配前一字符0次或多次(匹配最多)
-      匹配前一字符0次或多次(匹配最少)
?      匹配前一字符0次或1次(只要有可能，它会匹配一个；)
（注意这里是前一字符）

 在模式最前面加上符号 '^' 将锚定从字符串的开始处做匹配。
 在模式最后面加上符号 '$' 将使匹配过程锚定到字符串的结尾。

-- 字符类	描述	示例	结果
-- [01]	匹配二进制数	
--]]
print(string.find("3102123","[01]"));   -->  3 3
print(string.find("ABC[]D","%["));
print(string.find("AB12","%A"))

--基本模式串
-- print(string.find("abc", "."))	 -->1	-->.	-->任意字符
-- print(string.find("a bc", "%s"))	 -->2	-->%s	-->空白字符
-- print(string.find("a bc", "%S"))	 -->2	-->%S	-->非空白字符
-- print(string.find("ab?c", "%p"))	 -->2	-->%p	-->标点符号(小写)
-- print(string.find("ab?c", "%P"))	 -->2	-->%P	-->非标点符号(大写)
-- print(string.find("ab?c", "%c"))	 -->2	-->%c	-->控制字符
-- print(string.find("ab?c", "%C"))	 -->2	-->%C	-->非控制字符(大写)
-- print(string.find("ab?c", "%d"))	 -->2	-->%d	-->数字
-- print(string.find("ab?c", "%D"))	 -->2	-->%D	-->非数字(大写)

--[[
模式可以在内部用小括号括起一个子模式； 这些子模式被称为 捕获物。
当匹配成功时，由 捕获物 匹配到的字符串中的子串被保存起来用于未来的用途。
捕获物以它们左括号的次序来编号。
--]]
t = {}
s = "from=world, to=Lua"
for k, v, s in string.gmatch(s, "((%w+)=(%w+))") do
	print(k, v, s);
end

--字符串和/0结束的字符
local f = "a\0bc\0"
print(string.len(f));
print(f)


local g = " infect: virus test ";
local h = string.match(g, "infect:(.+)");
print(h);


-- 去掉字符串前后的空格
local k = {"	qw er	", "	 test ", "wefg"};
for i, v in pairs(k) do
	local i = string.match(v, "%s*(.*[^%s])%s*");
	print(string.len(i));
	print(i)
end

--local i,j = string.find(g, ".+[^%s]$");
--print(i,j);