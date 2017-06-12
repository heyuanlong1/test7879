
CC=gcc
CCj=g++

SERVE=us
CLIENT=uc
OBJCLIENT=uc.o

CFLAGS= -g
LDFLAGS=-lpthread

#------------------------------------------------------------
.PHONY: all

all:    $(SERVE) $(CLIENT)

$(SERVE):   
	$(CC) $(LDFLAGS) -o $(SERVE) gateway.c v_rbtree.c main.c
        
$(CLIENT): $(OBJCLIENT)
	$(CCj) $(LDFLAGS) -o $(CLIENT) $(OBJCLIENT)
        

$(OBJCLIENT): %.o:   client.cpp client.h base.h
	$(CCj) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o -f
	rm $(SERVE)
	rm $(CLIENT)
