#include "stdio.h"
#include "stdlib.h"


/* number of bits in a character */
#define NB	8
/* mask for one character (NB 1's) */
#define MC	((1 << NB) - 1)
void pack(char *buf, unsigned int n,int size,bool islittle)
{
	int i;
	buf[islittle ? 0 : size - 1] = (char)(n & MC);  /* first byte */
	for (i = 1; i < size; i++) {
		n >>= NB;
		buf[islittle ? i : size - 1 - i] = (char)(n & MC);
	}
	return;
}
int unpack(const char * str, bool islittle, int size) {
	unsigned int res = 0;
	int i;
	int limit = size;
	for (i = limit - 1; i >= 0; i--) {
		res <<= NB;
		res |= (unsigned int)(unsigned char)str[islittle ? i : size - 1 - i];
	}
	return (int)res;
}


int main(int argc, char const *argv[])
{
	{
		char buf[2];
		pack(buf,42,2,true);
		int res = unpack(buf,true,2);
		printf("%d\n",res );
	}
	{
		char buf[2];
		pack(buf,5678,4,false);
		int res = unpack(buf,false,4);
		printf("%d\n",res );		
	}

	return 0;
}