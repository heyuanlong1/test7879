CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE): test.cpp log.cpp log.h
	$(CC) -o $(SERVE) test.cpp log.cpp

clean:
	rm -rf $(SERVE)
