all:queryServer queryServerd zktest1

queryServer:queryServer.o
	gcc -L/usr/local/lib/ -lzookeeper_mt -o $@ $^ 

queryServer.o:queryServer.c
	gcc -DTHREADED -I/usr/local/include/zookeeper -o $@ -c $^


queryServerd:queryServerd.o
	gcc -L/usr/local/lib/ -lzookeeper_mt -o $@ $^ 

queryServerd.o:queryServerd.c
	gcc -g -DTHREADED -I/usr/local/include/zookeeper -o $@ -c $^


zktest1:zktest1.o
	gcc -L/usr/local/lib/ -lzookeeper_mt -o $@ $^ 

zktest1.o:zktest1.c
	gcc -g -DTHREADED -I/usr/local/include/zookeeper -o $@ -c $^


.PHONY:clean

clean:
	rm queryServer.o queryServer
	rm queryServerd.o queryServerd
	rm zktest1.o zktest1