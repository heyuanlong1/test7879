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

