local t = {
        name = "hehe",
    }
    
local mt = {
        __index = {
            money = "900,0000",
        }
    }
setmetatable(t,mt);
    
print(t.money);