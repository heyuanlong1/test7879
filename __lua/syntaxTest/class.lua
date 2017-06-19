
Account = {balance = 0}
function Account.withDraw(self, v)
     self.balance = self.balance - v
end

a = Account
Account = nil
a.withDraw(a, 100)
print(a.balance)




Book = {balance = 0}
function Book:withDraw(v) 			-- 注意这里的冒号":"
     self.balance = self.balance - v
end

a = Book
Book = nil
a:withDraw(100) 					-- 注意这里的调用时，也需要冒号":"
print(a.balance)





Person = {balance = 0}
function Person.withDraw( v)
     Person.balance = Person.balance - v
end


Person.withDraw(100)
print(Person.balance)

a = Person
Person = nil
a.withDraw(100)						-- 这里会报错
print(a.balance)


--   http://www.jellythink.com/archives/529