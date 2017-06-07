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

-----------------------------------------------------------list
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
table.sort(t)
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

-----------------------------------------------------------table
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
table.sort(t)					--table.sort 1,2.....顺序的值
print(table_dump(t))

for i,v in ipairs(t) do			--ipairs 输出 1,2.....顺序的值
	print(i,v)
end
print()
for k,v in pairs(t) do
	print(k,v)
end




-- http://www.tuicool.com/articles/mY7Rzi


