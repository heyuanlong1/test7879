local num = 65134
local str = string.char(num / 256) .. string.char(num % 256)
print("#str:"..#str)
local s = str:byte(1) * 256 + str:byte(2)				--65134
print(s)
local s = string.byte(str) * 256 + string.byte(str,2)	--65134
print(s)


-- string.char(number)	 返回字符
-- string.byte(string)	 返回数字

print("------------------------------------------------")
s = "[in brackets]"  
print(string.sub(s, 2, -2)) --> in brackets   返回去除第一个和最后一个字符后的子串.


--string.gsub(s, pattern, reps[,limitNumber]) 将s中所有符合pattern的字串替换为reps,返回结果串+匹配数
--s 源字符串
--待替换的字符模式
--替换为reps
--limitNumber 限制替换的次数

print(string.gsub("hello, world", "o", "a"))  		-- hella, warld        2  
print(string.gsub("hello, world", "(o)", "%1-%1")) 	-- hello-o, wo-orld    2  
print(string.gsub("hello Lua", "(.)(.)", "%2%1")) 	-- ehll ouLa  4  每2个字符进行位置调换  

function trim (s) return (string.gsub(s, "^%s*(.-)%s*$", "%1")) end --注意匹配数用括号丢弃  
print(trim("  sfsdf "))


--string.gsub(s, pattern, func)
--func 对找到的匹配操作,并传出替换值

s, n = string.gsub("hello world", "l+", function(s) return "xxx" end)   
print(s, n) -- hexxxo worxxxd 2  


print("------------------------------------------------")

--string.gfind(s, pattern)
--返回一个迭代器，迭代器每执行一次，返回下一个匹配串;
iter = string.gfind("a=b c=d", "[^%s+]=[^%s+]")  
print(iter()) --<== a=b  
print(iter()) --<== c=d  
for v in string.gfind("a=b c=d", "[^%s+]=[^%s+]")  do
	print(v)
end


--string.match(s,d) --与string.find类似 但是返回匹配的子串,只返回第一个匹配的字符串
--s 源字符串
--d 目标字符串或者模式
print(string.match("hello hello helloworld","hello")) --hello  
local today="today is 19/2/2001"  
print(string.match(today,"%d+/%d+/%d"))--19/2/2001  


--string.gmatch() 
--返回一个迭代函数 可以遍历出现的目标字符串
iter = string.gmatch("hello hello helloworld","hello") --hello  
print(iter()) --
print(iter()) --
for v in string.gmatch("hello hello helloworld","hello")  do
	print(v)
end

print("------------------------------------------------")

function dieDaiQi()
    local i = 0;
    print("i:"..i)
    return function()
        i = i + 1;
        print("j:"..i)
        if i > 5 then
        	return nil
        end   
        return i;
    end
end

for v in dieDaiQi()  do
	print()
end