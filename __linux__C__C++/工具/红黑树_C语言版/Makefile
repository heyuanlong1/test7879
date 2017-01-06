
CC=gcc
SERVE=test



#------------------------------------------------------------
.PHONY: all clean

all:    $(SERVE) 

$(SERVE):test.c v_rbtree.c v_rbtree.h
	$(CC) -o $(SERVE) test.c v_rbtree.c

clean:
	rm -rf $(SERVE)
