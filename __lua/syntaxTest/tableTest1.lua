function table_dump(tbl)
    -- assert("table" == type(tbl))
    local tmp ={}
    local i = 1

    if("table" ~= type(tbl)) then
        return tostring(tbl)
    end

    for k, v in pairs(tbl) do
        if("table" == type(v)) then
            tmp[i] = tostring(k).."="..table_dump(v)
        else
            tmp[i] = tostring(k).."="..tostring(v)
        end
        i = i+1
    end
    return "{"..table.concat(tmp," ").."}"
end


print("-----------------------------------------------------------list")
local t = {10,20,30}

table.insert(t,2,15)			--在第二个位置插入值
table.insert(t,40)
print(table_dump(t))

table.remove(t,2)				--删除第二个位置的数据
table.remove(t)

print(table_dump(t))


table.insert(t,2,60)
table.insert(t,3,1)
table.sort(t) 					--要求 1,2.....顺序的值 不能有nil并且类型相同
print(table_dump(t))

print("table.concat:"..table.concat(t,","))

for i,v in ipairs(t) do
	print(i,v)
end
print()
for k,v in pairs(t) do
	print(k,v)
end


t = {{a=2,b=22},{a=3,b=13},{a=1,b=1} }
print(table_dump(t))

--table.sort(t)  attempt to compare two table values (报错)
table.sort(t,function ( arg1,arg2 )
	return arg1.a > arg2.a
	--return arg1.b > arg2.b
end)
print(table_dump(t))

print("-----------------------------------------------------------table")

t = {key3 = 44}
t.key2 = 22
k = "key1"
t[k] = 33
print(table_dump(t))
table.sort(t)
print(table_dump(t))
t[1] = 22
t[2] = 12
t[10] = 1
print(table_dump(t))
table.sort(t)					--要求 1,2.....顺序的值 不能有nil并且类型相同
print(table_dump(t))

for i,v in ipairs(t) do			--ipairs 输出 1,2.....顺序的值
	print(i,v)
end
print()
for k,v in pairs(t) do
	print(k,v)
end

print("table.concat:"..table.concat(t,",")) 	--只连接 1,2.....顺序的值


print("-----------------------------------------------------------删除list的正确姿势")

local t = { 24,9,1,5,48,51,71,66}
for i, v in ipairs( t ) do
    if v > 30  then
        table.remove(t, i)
    end
end
print("false:"..table_dump(t))

local t = { 24,9,1,5,48,51,71,66}
for i = #t, 1, -1 do
    if t[i] > 30  then
        table.remove(t, i)
    end
end
print("从后往前删除  true:"..table_dump(t))

local t = { 24,9,1,5,48,51,71,66}
local i = 1
while i <= #t do
    if t[i] > 30 then
        table.remove(t, i)
    else
        i = i + 1
    end
end
print("while删除   true:"..table_dump(t))


print("-----------------------------------------------------------table使用, {}的使用会影响到原{}")

local t = {}
t.key1 = { person1={},person2={}}
t.key2 = { person1={},person2={}}
for _,v in pairs(t) do
	for _,pv in pairs(v) do				--pv是personN的指针
		pv.age = 10
		pv.name = "long"
	end
end
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
local t2p2 = t.key2.person2				--t2p2存储的t.key2.person2地址的指针变量
t2p2.age = 200
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
local t2p2a = t.key2.person2.age		-- t2p2a 不是{} 所以不影响原值
t2p2a = 300
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
t.key2.person2 = nil
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
local lkey2 = t.key2					--lkey2 存储的t.key2地址的指针变量
lkey2.person1= nil
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
t.key1.person2.age = nil
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

print()
local lkey1= t.key1 					--lkey1 存储的t.key1地址的指针变量
lkey1= nil								--仅仅是改变了lkey1本身存储的值
for _,v in pairs(t) do
	for _,pv in pairs(v) do
		print(pv.age,pv.name)
	end
end

t["key1"].person1.age = 45
print(t.key1.person1.age)
print(t["key1"].person1["age"])

print("-----------------------------------------------------------建议list和table不要混合使用")






