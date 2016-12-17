
CC=gcc
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   test.c test1.c
	$(CC) -g -o  test test.c 
	$(CC) -g -o  test1 test1.c 

clean:
	rm -rf $(SERVE)
	rm -rf test1

