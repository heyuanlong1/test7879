
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp systemVSem.h
	$(CC) -g -o  $(SERVE) test.cpp 

clean:
	rm -rf $(SERVE)
