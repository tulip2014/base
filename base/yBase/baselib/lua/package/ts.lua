
TableA = {};

function TableA.Fun(tb)
	print(tb);
end

TableB = {};

function TableB:CallBack(cb)
	self.callback = cb;
end

function TableB:OnClick(go)
	print(go)
end

a = TableB.OnClick;
a(TableB,"a")