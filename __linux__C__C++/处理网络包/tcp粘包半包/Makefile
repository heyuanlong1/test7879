
CC=g++

SERVE=us
CLIENT=uc
OBJSERVE=us.o 
OBJCLIENT=uc.o

CFLAGS=
LDFLAGS=-lpthread

#------------------------------------------------------------
.PHONY: all

all:    $(SERVE) $(CLIENT)

$(SERVE):   $(OBJSERVE)
	$(CC) $(LDFLAGS) -o $(SERVE) $(OBJSERVE)
        
$(CLIENT):   $(OBJCLIENT)
	$(CC) $(LDFLAGS) -o $(CLIENT) $(OBJCLIENT)
        

$(OBJSERVE): %.o:   server.cpp server.h base.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJCLIENT): %.o:   client.cpp client.h base.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o -f
	rm $(SERVE)
	rm $(CLIENT)
