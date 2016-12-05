
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp jsoncpp.cpp
	$(CC) -g -o  $(SERVE) test.cpp jsoncpp.cpp

clean:
	rm -rf $(SERVE)
