
from RedisDB import RedisDB


r = RedisDB();
r.init()

print r.set("test","1111")
print r.get("test")
print ""

print r.hset("keykey","field1",12)
print r.hget("keykey","field1")
print ""

print r.hmset("keykey",{'field1':1,'field2':2,'field3':3})
print r.hmget("keykey",["field1","field2","field3"])
print ""


