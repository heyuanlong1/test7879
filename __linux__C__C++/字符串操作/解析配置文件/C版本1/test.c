
#include <stdio.h>
#include "MsConfig.h"
#include <string.h>


int main(int argc, char const *argv[])
{
	char value1[100];
	char value2[100];
	
	load();
	
	config_get_key_value("matchvs.game.id", value1);
	config_get_key_value("matchvs.key", value2);
	
	printf("%s\n",value1);
	printf("%s\n",value2);

	return 0;
}