Dir=busi/lobby/control/ \
busi/lobby/model/ \
busi/room/control/ \
busi/room/model/ \
cahe/control/ \
cahe/model/redis/ \
conf/ \
db/control/ \
db/model/mysql/ \
event/ \
log/ \
mem/ \
net/ \
tool/ 

testDir=testClient/

#---------------------------
Src=$(foreach dir,$(Dir), $(wildcard $(dir)*.cpp))
lobbySrc=$(Src) main/lobby_mian.cpp
roomSrc=$(Src) main/room_mian.cpp
testSrc=$(foreach dir,$(testDir), $(wildcard $(dir)*.cpp))

lobbyObj=$(patsubst %.cpp,%.o,$(lobbySrc))
roomObj=$(patsubst %.cpp,%.o,$(roomSrc))
testObj=$(patsubst %.cpp,%.o,$(testSrc))

#---------------------------
makeDir=./makeDir
lobby=bin/lobby
room=bin/room
testClient=bin/testClient

#---------------------------
Inc=Dir main/
LibDbMysql=-L../third-party/libs -lmysqlclient
CC=g++
CFLAGS= -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient

#---------------------------

.PHONY: all clean

all:	$(makeDir) $(testClient) $(lobby) 
	@echo all is ok

test:$(makeDir) $(testClient)
	@echo test is ok

$(makeDir):$(MAIN)
	mkdir -p bin/
	@echo $(lobbySrc)



#------------
$(testClient):$(testObj)
	$(CC) -o $@ $^

#------------ 
$(lobby):$(lobbyObj)
	$(CC) $(CFLAGS) -o $@ $^


BLCCFLAGS=
busi/lobby/control/%.o:busi/lobby/control/%.cpp
	$(CC) $(BLCCFLAGS)  -c -o $@ $^ -I./ 

BLMOBJCFLAGS=
busi/lobby/model/%.o:busi/lobby/model/%.cpp
	$(CC) $(BLMOBJCFLAGS)  -c -o $@ $^ -I./ 

BRCOBJCFLAGS=
busi/room/control/%.o:busi/room/control/%.cpp
	$(CC) $(BRCOBJCFLAGS)  -c -o $@ $^ -I./ 

BRMOBJCFLAGS=
busi/room/model/%.o:busi/room/model/%.cpp
	$(CC) $(BRMOBJCFLAGS)  -c -o $@ $^ -I./ 

CCOBJCFLAGS=
cahe/control/%.o:cahe/control/%.cpp
	$(CC) $(CCOBJCFLAGS)  -c -o $@ $^ -I./ 

CMROBJCFLAGS=
cahe/model/redis/%.o:cahe/model/redis/%.cpp
	$(CC) $(CMROBJCFLAGS)  -c -o $@ $^ -I./ 

COBJCFLAGS=
conf/%.o:conf/%.cpp
	$(CC) $(COBJCFLAGS)  -c -o $@ $^ -I./ 

DCOBJCFLAGS=-I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
db/control/%.o:db/control/%.cpp
	$(CC) $(DCOBJCFLAGS)  -c -o $@ $^ -I./ 

DMMOBJCFLAGS=-I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
db/model/mysql/%.o:db/model/mysql/%.cpp
	$(CC) $(DMMOBJCFLAGS)  -c -o $@ $^ -I./ 

EOBJCFLAGS=
event/%.o:event/%.cpp
	$(CC) $(EOBJCFLAGS)  -c -o $@ $^ -I./ 

LOBJCFLAGS= -DHVS_DEBUG
log/%.o:log/%.cpp
	$(CC) $(LOBJCFLAGS)  -c -o $@ $^ -I./ 

MOBJCFLAGS=
mem/%.o:mem/%.cpp
	$(CC) $(MOBJCFLAGS)  -c -o $@ $^ -I./ 

NOBJCFLAGS=
net/%.o:net/%.cpp
	$(CC) $(NOBJCFLAGS)  -c -o $@ $^ -I./ 

TOBJCFLAGS=
tool/%.o:tool/%.cpp
	$(CC) $(TOBJCFLAGS)  -c -o $@ $^ -I./ 

main/lobby_mian.o:main/lobby_mian.cpp
	$(CC)  -c -o $@ $^ -I./ 

clean:
	rm -rf $(testObj)
	rm -rf $(lobbyObj)
	rm -rf $(testClient)
	rm -rf $(lobby)
	rm -rf $(room)


# $(Dir)%.o: $(lobbySrc)
# 	$(CC) -c -o $@  $(patsubst %.o,%.cpp,$@ )  -I./ 

