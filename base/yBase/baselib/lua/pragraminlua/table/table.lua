
local a = {1, 4, "123", "test"};
a.status = 10;

print(a[4]);
print(a.status);

print(#a);   --������ŷ�ӳ���鲿�ֵĴ�С
--a[2] = nil;  --��ʹ����Ϊnil������Ҳ֪�����ĳ���
print(#a);
print(a[2]);

table.insert(a, 11);    --Ĭ��������
print(a[5]);
table.insert(a, 2, 11); --�ڶ����������ò���λ��
print(a[2]);

table.remove(a);		--�Ƴ����һ��Ԫ��
print(a[6]);

print(table.concat(a, ":"));  --��ӡ���鲿�ֵ�ֵ(���в����� nil )

compare = function (a, b) return a > b end;
local b = {"alpha", "beta", "gamma", "delta"};
b.a = "test";
table.sort(b, compare);
print(table.concat(b, ","));