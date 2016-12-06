
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.cpp MsConfig.cpp
	$(CC) -g -o  $(SERVE) test.cpp MsConfig.cpp

clean:
	rm -rf $(SERVE)
