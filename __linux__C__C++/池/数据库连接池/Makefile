
CC=g++

SERVE=us
CLIENT=uc
OBJSERVE=mysqlPool.o controlUnit.o server.o 
OBJCLIENT=client.o

CFLAGS=-I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient
CLIENT_CFLAGS= -lpthread


#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) $(CLIENT)

$(SERVE):   $(OBJSERVE)
	$(CC) $(CFLAGS) -o $(SERVE) $(OBJSERVE)
        
$(CLIENT):   $(OBJCLIENT)
	$(CC) $(CLIENT_CFLAGS) -o $(CLIENT) $(OBJCLIENT)
        

$(OBJSERVE): %.o:  %.cpp %.h
	$(CC) $(CFLAGS) -c -o $@ $< 

$(OBJCLIENT): %.o:   %.cpp client.h base.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o -f
	rm $(SERVE)
	rm $(CLIENT)
