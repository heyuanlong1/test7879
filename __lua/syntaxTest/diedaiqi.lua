


function values( t )
	local i = 0
	return function (  )
		i = i + 1
		return t[i]
	end
end
t = {20,30,40}
for e in values(t) do
	print(e)
end


-- 泛型for在循环过程内部保存了迭代器函数。实际上它保存着3个值，一个迭代器函数，一个恒定状态和一个控制变量。
-- 变量列表的第一个元素称为 控制变量，nil就结束了

-- for <var-list> in <exp-list> do
-- 	<body>
-- end

-- 循环过程中控制变量的值依次为 a1=f(s,a0),a2=f(s,a1),依次类推，直至ai为nil结束循环。

print()  
local function iter( a, i)
	i = i + 1
	local v = a[i]
	if v then
		return i ,v
	end
end
function myipairs( a )
	return iter , a , 0
end
t = {20,30,40}
for i,v in myipairs(t) do
	print(i,v)
end




print()  
local function getnext(list ,node )
	if not node then
		return list
	else
		return node.next
	end
end
function traverse( list )
	return getnext ,list ,nil
end
list = {var = 1,next=nil}
list.next = {var = 2,next=nil}
list.next.next = {var = 3,next=nil}
for node in traverse(list) do
	print(node.var)
end



print()  
--以下为无状态迭代器  
function square(max,cur)  
   if cur<max then  
      	cur = cur+1  
   		return cur, cur*cur  
   end  
end  
--泛型 for 在自己内部保存迭代函数，实际上它保存三个值：  
--迭代函数square、状态常量3、控制变量0  
for i,n in square,3,0 do  
   print(i,n)  
end  


--  http://blog.csdn.net/hust_21314/article/details/51887689