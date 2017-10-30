
CC=gcc

SERVE=test
OBJSERVE=hPthreadPool.o 
CFLAGS= -lpthread



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   $(OBJSERVE) test.c
	$(CC) $(CFLAGS) -o $(SERVE) $(OBJSERVE) test.c


$(OBJSERVE): %.o:  %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< 


clean:
	rm *.o -f
	rm $(SERVE)

