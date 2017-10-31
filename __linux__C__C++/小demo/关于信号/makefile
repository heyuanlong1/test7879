
CC=g++
1signal=1signal
2sigaction=2sigaction
3sigset=3sigset
4sig=4sig
5sig=5sig
6sig=6sig
7sigsuspend=7sigsuspend



#------------------------------------------------------------
.PHONY: all clean

all:    $(1signal)  $(2sigaction)  $(3sigset)   $(4sig)  $(5sig) $(6sig)  $(7sigsuspend) 

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
$(6sig):6sig.cpp
	$(CC) -g -o  $(6sig) 6sig.cpp -lpthread
$(7sigsuspend):7sigsuspend.cpp
	$(CC) -g -o  $(7sigsuspend) 7sigsuspend.cpp -lpthread
clean:
	rm -rf $(1signal)
	rm -rf $(2sigaction)
	rm -rf $(3sigset)
	rm -rf $(4sig)
	rm -rf $(5sig)
	rm -rf $(6sig)
	rm -rf $(7sigsuspend)
