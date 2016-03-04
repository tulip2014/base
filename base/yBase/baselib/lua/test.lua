
function test(n)
	print("test:",n)
end

a = string.dump(test, true);
b = load(a);
b(6)