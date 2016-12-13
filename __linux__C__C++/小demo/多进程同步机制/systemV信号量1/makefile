
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp systemVSem.h test2.cpp  test3.cpp 
	$(CC) -g -o  $(SERVE) test.cpp 
	$(CC) -g -o  test2 test2.cpp 
	$(CC) -g -o  test3 test3.cpp 

clean:
	rm -rf $(SERVE)
	rm -rf test2
	rm -rf test3
