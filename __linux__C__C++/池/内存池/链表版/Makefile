
CC=gcc

SERVE=test
OBJSERVE=hMemoryPool.o 
CFLAGS=



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

