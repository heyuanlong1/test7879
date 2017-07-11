
CC=gcc

SERVE=test
OBJSERVE=test.o 
CFLAGS=



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   $(OBJSERVE) 
	$(CC) $(CFLAGS) -o $(SERVE) $(OBJSERVE) 


$(OBJSERVE): %.o:  %.c 
	$(CC) $(CFLAGS) -c -o $@ $< 


clean:
	rm *.o -f
	rm $(SERVE)

