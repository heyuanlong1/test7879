#include "json/json.h"
#include "json/json-forwards.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main(int argc, char const *argv[])
{
	char str[5024] = "{\"data\":[{\"desc\":\"免费入场\",\"gameID\":200682,\"type\":0},{\"desc\":\"没有机器人\",\"gameID\":200682,\"type\":0}],\"status\":0}";
    int i = 0;
    Json::Reader reader;
    Json::Value root;
    Json::Value data;
    if (!reader.parse(str, root, false)){
    	printf("error\n");
        return -1;
    }

	int status = root["status"].asInt();
	if (0 == status) {
		Json::Value data = root["data"];
		int arraySize = data.size();
		for ( i = 0; i < arraySize; i++)
		{
			string desc = data[i]["desc"].asString();
			int gameID =  data[i]["gameID"].asInt();
			cout<<desc<<"    "<<gameID<<endl;
		}
	}

	return 0;
}
