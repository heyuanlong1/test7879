
CC=g++
1signal=1signal
2sigaction=2sigaction
3sigset=3sigset
4sig=4sig
5sig=5sig




#------------------------------------------------------------
.PHONY: all clean

all:    $(1signal)  $(2sigaction)  $(3sigset)   $(4sig)  $(5sig) 

$(1signal):1signal.c 
	$(CC) -g -o  $(1signal) 1signal.c
$(2sigaction):2sigaction.c 
	$(CC) -g -o  $(2sigaction) 2sigaction.c
$(3sigset):3sigset.c 
	$(CC) -g -o  $(3sigset) 3sigset.c
$(4sig):4sig.c 
	$(CC) -g -o  $(4sig) 4sig.c
$(5sig):5sig.cpp
	$(CC) -g -o  $(5sig) 5sig.cpp

clean:
	rm -rf $(1signal)
	rm -rf $(2sigaction)
	rm -rf $(3sigset)
	rm -rf $(4sig)
	rm -rf $(5sig)
