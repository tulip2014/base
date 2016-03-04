
local a = "Atest";
local A = 65;
local B = 66;

print(string.byte(a, 1));  --�õ�ascii����
print(string.char(A, B));  --�õ��ַ�����������������

function Add(a, b)
	return a + b;
end
print((string.dump(Add)));


--[[��������������ʼ���ҵ�λ�ã����ĸ��������Ƿ�ص�ħ����]]
local c = "test .. name";
local iS, iE = string.find(c, ".", 1, true);  
print(iS, iE);


-- q ѡ�һ���ַ�����ʽ��Ϊ����˫�������𣬶��ڲ��ַ���ǡ����ת�崦����ַ����� 
-- ���ַ������԰�ȫ�ı� Lua ��������������
local d = string.format('test: %d,%q', 10, 'a string with "quotes" and \n new line')
print(d);

--[[
x: ������ x ������ ħ���ַ� ^$()%.[]*+-? �е�һԱ�� ��ʾ�ַ� x ����
.: ��һ���㣩�ɱ�ʾ�κ��ַ���
%a: ��ʾ�κ���ĸ��
%A	����ĸ	string.find("AB12","%A%A")	3 4
%c: ��ʾ�κο����ַ���
%d: ��ʾ�κ����֡�
%g: ��ʾ�κγ��հ׷���Ŀɴ�ӡ�ַ���
%l: ��ʾ����Сд��ĸ��
%L	��д��ĸ	string.find("ABab","%L%L")	1 2
%p: ��ʾ���б����š�
%s: ��ʾ���пհ��ַ���
%u: ��ʾ���д�д��ĸ��
%U	�Ǵ�д��ĸ	string.find("ABab","%U%U")	3 4
%w: ��ʾ������ĸ�����֡�
%x: ��ʾ���� 16 �������ַ��š�
%X	��ʮ����������	string.find("efgh","%X%X")	3 4
--]]

s = "hello 10.12World	20from  * Lua"
for w in string.gmatch(s, "%U+") do
	print(w);
	--print(string.byte(w))
end

--[[
-- ����[]�����ַ�����"-"Ϊ���ַ���"^"��ʾ�ַ����Ĳ�����
-- ����ʹ����ͷ�Χ����Ϊδ���塣 
��ˣ��� [%a-z] �� [a-%%] ������ģʽ��û�����塣
[^set]: ��ʾ set �Ĳ����� ���� set ������Ľ��͡�

+      ƥ��ǰһ�ַ�1�λ���(ƥ�����)
*      ƥ��ǰһ�ַ�0�λ���(ƥ�����)
-      ƥ��ǰһ�ַ�0�λ���(ƥ������)
?      ƥ��ǰһ�ַ�0�λ�1��(ֻҪ�п��ܣ�����ƥ��һ����)
��ע��������ǰһ�ַ���

 ��ģʽ��ǰ����Ϸ��� '^' ��ê�����ַ����Ŀ�ʼ����ƥ�䡣
 ��ģʽ�������Ϸ��� '$' ��ʹƥ�����ê�����ַ����Ľ�β��

-- �ַ���	����	ʾ��	���
-- [01]	ƥ���������	
--]]
print(string.find("3102123","[01]"));   -->  3 3
print(string.find("ABC[]D","%["));
print(string.find("AB12","%A"))

--����ģʽ��
-- print(string.find("abc", "."))	 -->1	-->.	-->�����ַ�
-- print(string.find("a bc", "%s"))	 -->2	-->%s	-->�հ��ַ�
-- print(string.find("a bc", "%S"))	 -->2	-->%S	-->�ǿհ��ַ�
-- print(string.find("ab?c", "%p"))	 -->2	-->%p	-->������(Сд)
-- print(string.find("ab?c", "%P"))	 -->2	-->%P	-->�Ǳ�����(��д)
-- print(string.find("ab?c", "%c"))	 -->2	-->%c	-->�����ַ�
-- print(string.find("ab?c", "%C"))	 -->2	-->%C	-->�ǿ����ַ�(��д)
-- print(string.find("ab?c", "%d"))	 -->2	-->%d	-->����
-- print(string.find("ab?c", "%D"))	 -->2	-->%D	-->������(��д)

--[[
ģʽ�������ڲ���С��������һ����ģʽ�� ��Щ��ģʽ����Ϊ �����
��ƥ��ɹ�ʱ���� ������ ƥ�䵽���ַ����е��Ӵ���������������δ������;��
�����������������ŵĴ�������š�
--]]
t = {}
s = "from=world, to=Lua"
for k, v, s in string.gmatch(s, "((%w+)=(%w+))") do
	print(k, v, s);
end

--�ַ�����/0�������ַ�
local f = "a\0bc\0"
print(string.len(f));
print(f)


local g = " infect: virus test ";
local h = string.match(g, "infect:(.+)");
print(h);


-- ȥ���ַ���ǰ��Ŀո�
local k = {"	qw er	", "	 test ", "wefg"};
for i, v in pairs(k) do
	local i = string.match(v, "%s*(.*[^%s])%s*");
	print(string.len(i));
	print(i)
end

--local i,j = string.find(g, ".+[^%s]$");
--print(i,j);