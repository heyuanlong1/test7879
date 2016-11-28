#include "lobby_mian.h"


int main(int argc, char const *argv[])
{
	if (conf_init() < 0) {
		exit(1);
	}
	if (busi_lobby_init() < 0) {
		exit(1);
	}
	if (busi_lobby_loop() < 0) {
		exit(1);
	}

	return 0;
}


