CC=gcc

SERVE=testhiredis
CFLAGS=`pkg-config hiredis --libs --cflags`


#------------------------------------------------------------
.PHONY: all

all:    $(SERVE) 

$(SERVE): testhiredis.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm $(SERVE)
