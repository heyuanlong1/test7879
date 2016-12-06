
#include <stdio.h>
#include "MsConfig.h"
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	int gameID;
	std::string appKey;
	std::string appSecret;
	if (MsConfig::getInstance()->GetInteger("matchvs.game.id", gameID) == -1) {
		return -1;
	}
	cout << "gameID " << gameID<< endl;
	if (MsConfig::getInstance()->GetString("matchvs.key", appKey) == -1) {
		return -1;
	}
	cout << "appKey " << appKey<< endl;
	if (MsConfig::getInstance()->GetString("matchvs.secret", appSecret) == -1) {
		return -1;
	}
	cout << "appSecret " << appSecret<< endl;

	return 0;
}