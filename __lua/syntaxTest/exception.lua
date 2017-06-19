
-- 某些lua版本的xpcall不会传参数。



local status, msgId, req = pcall(function()
                assert(true, "errormsg")
                return 1000, "req"
            end)
print(status,msgId,req)  					--true    1000    req

local status, msgId, req = pcall(function()
                assert(false, "errormsg")
                return 1000, "req"
            end)
print(status,msgId,req)						--false   exception.lua:8: errormsg       nil

local status, msgId, req = pcall(function()
                assert(nil, "errormsg")
                return 1000, "req"
            end)
print(status,msgId,req)						--false   exception.lua:14: errormsg      nil



function square(a)
      return a * "a"
end
function square1(a)
      return a * a
end

print()
local status, retval = pcall(square,10);
print ("Status: ", status)        -- 打印 "false" 
print ("Return Value: ", retval)


print("----------------------------")
status,  ret = xpcall(square, debug.traceback, 10)
print ("status: ", status)        -- 打印 "false" 
print ("ret: ", ret)

print("----------------------------")
status,  ret = xpcall(square, function(errormsg) print(debug.traceback(errormsg)) end, 10)
print ("status: ", status)        -- 打印 "false" 
print ("ret: ", ret)

print("+++++++++++++++++++++++++++++")
status,  ret = xpcall(square1, function(errormsg) print(errormsg) end, 10)
print ("status: ", status)        -- 打印 "false" 
print ("ret: ", ret)


print("+++++++++++++++++++++++++++++")
xpcall(function(i) print(i) error('error..') end, function() print(debug.traceback()) end, 33)