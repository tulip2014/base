
local a = {1, 4, "123", "test"};
a.status = 10;

print(a[4]);
print(a.status);

print(#a);   --这个符号反映数组部分的大小
--a[2] = nil;  --即使设置为nil，后面也知道他的长度
print(#a);
print(a[2]);

table.insert(a, 11);    --默认最后插入
print(a[5]);
table.insert(a, 2, 11); --第二个参数设置插入位置
print(a[2]);

table.remove(a);		--移除最后一个元素
print(a[6]);

print(table.concat(a, ":"));  --打印数组部分的值(其中不能有 nil )

compare = function (a, b) return a > b end;
local b = {"alpha", "beta", "gamma", "delta"};
b.a = "test";
table.sort(b, compare);
print(table.concat(b, ","));