#makefile
.PHONY:clean all
#防止clean时出错而中断

CC=g++ -g -Wall


ht:main.cpp http.cpp http.h KOHttpBase.h KOHttpGo.cpp KOHttpGo.h
	$(CC) main.cpp http.cpp KOHttpGo.cpp -o ht

clean:#可以在命令中输入 make clean 来执行这个规则
	$(RM) *.o ht
