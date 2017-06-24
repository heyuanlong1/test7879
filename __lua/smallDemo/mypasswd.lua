
cs , key , str = ...

--str 环形循环 加key字符，小于300加300

local function encrypt(str, key)
	local strlen = #str
	local keyi = 1
	local keylen = #key
	local res=""

	for i=1,strlen do
		local num = string.byte(string.sub(str,i,i)) + string.byte(string.sub(key,keyi,keyi))
		if num < 100 then
			num = num + 300
		end
		res = tostring(num) .. res
		keyi = keyi + 1
		if keyi > keylen then
			keyi = 1
		end
	end
	return res
end


local function decrypt(str, key)
	local strlen = #str
	local keyi = 1
	local keylen = #key
	local res=""

	for i=strlen,1,-3 do
		local num = tonumber(string.sub(str,i - 2,i)) 
		if num > 300 then
			num = num - 300
		end
		num  = num - string.byte(string.sub(key,keyi,keyi))
		res = res .. string.char(num) 
		keyi = keyi + 1
		if keyi > keylen then
			keyi = 1
		end
	end
	return res
end


if cs == "e" then
	print(encrypt(str,key))
elseif cs == "d" then
	print(decrypt(str,key))
else
	print("[e|d] [key] [str]")
end
