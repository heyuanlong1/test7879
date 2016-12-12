
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp CommonCond.h
	$(CC) -g -o  $(SERVE) test.cpp -lpthread

clean:
	rm -rf $(SERVE)
