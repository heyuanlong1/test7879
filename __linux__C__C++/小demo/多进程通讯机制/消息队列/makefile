
CC=gcc
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):   test.c testmsgrcv.c testmsgsnd.c 
	$(CC) -g -o  test test.c 
	$(CC) -g -o  testmsgrcv testmsgrcv.c 
	$(CC) -g -o  testmsgsnd testmsgsnd.c 

clean:
	rm -rf $(SERVE)
	rm -rf testmsgrcv
	rm -rf testmsgsnd

