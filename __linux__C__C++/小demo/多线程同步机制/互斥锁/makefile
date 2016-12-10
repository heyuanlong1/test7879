
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp CommonMutex.h
	$(CC) -g -o  $(SERVE) test.cpp -lpthread

clean:
	rm -rf $(SERVE)
