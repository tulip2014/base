
-- co = coroutine.create(
	-- function()
		-- for i = 1, 10 do
			-- print(i);
			-- print("print:"..tostring(i), coroutine.yield("yiled:"..tostring(i)));
		-- end
		-- print(coroutine.yield("yiled:"..tostring(i)));
		-- print("end")
	-- end)
	
-- print(co);
-- print(coroutine.status(co));
-- print(coroutine.resume(co));
-- print(coroutine.status(co));
-- print(coroutine.resume(co, 11));


-- function foo (a)
    -- print("foo", a)  -- foo 2
    -- return coroutine.yield(2 * a) -- return: a , b 
-- end
 
-- co = coroutine.create(function (a , b)
    -- print("co-body", a, b) -- co-body 1 10
    -- local r = foo(a + 1)
     
    -- print("co-body2", r)
    -- local r, s = coroutine.yield(a + b, a - b)
     
    -- print("co-body3", r, s)
    -- return b, "end"
-- end)
        
-- print("main", coroutine.resume(co, 1, 10)) -- true, 4
-- print("------")
-- print("main", coroutine.resume(co, "r")) -- true 11 -9


co = coroutine.create(
    function (a , b)
        print("params", a, b)
        coroutine.yield(3, 3)
    end
)
coroutine.resume(co, 1, 2);
print(coroutine.resume(co, 4, 5))
