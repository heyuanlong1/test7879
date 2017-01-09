
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp MsThreadBase.h
	$(CC) -o $(SERVE) test.cpp -lpthread
clean:
	rm -rf $(SERVE)
