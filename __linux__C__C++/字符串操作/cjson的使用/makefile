
CC=g++
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):
	$(CC) -g -o  $(SERVE) test.c cJSON.c

clean:
	rm -rf $(SERVE)
