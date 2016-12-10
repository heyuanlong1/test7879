
CC=gcc
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.c MsConfig.c
	$(CC) -g -o  $(SERVE) test.c MsConfig.c

clean:
	rm -rf $(SERVE)
