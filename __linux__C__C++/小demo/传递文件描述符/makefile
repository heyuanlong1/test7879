
CC=gcc
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   test.c 
	$(CC) -g -o  test test.c 

clean:
	rm -rf $(SERVE)

