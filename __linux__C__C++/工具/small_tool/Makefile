
CC=gcc

SERVE=testhtool
OBJSERVE=htool.o 
CFLAGS=



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   $(OBJSERVE) testhtool.c
	$(CC) $(CFLAGS) -o $(SERVE) $(OBJSERVE) testhtool.c


$(OBJSERVE): %.o:  %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $< 


clean:
	rm *.o -f
	rm $(SERVE)

