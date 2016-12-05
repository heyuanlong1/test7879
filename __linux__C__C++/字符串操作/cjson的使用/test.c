#include "cJSON.h"
#include "stdio.h"

int main(int argc, char const *argv[])
{
	char str[5024] = "{\"data\":[{\"desc\":\"免费入场\",\"gameID\":200682,\"type\":0},{\"desc\":\"没有机器人\",\"gameID\":200682,\"type\":0}],\"status\":0}";
    int i = 0;
	cJSON* root = cJSON_Parse(str);
	int status = cJSON_GetObjectItem(root, "status")->valueint;
	if (0 == status) {
		cJSON* dataArray = cJSON_GetObjectItem(root, "data");
		int arraySize = cJSON_GetArraySize(dataArray);
		for ( i = 0; i < arraySize; i++)
		{
			cJSON* item = cJSON_GetArrayItem(dataArray, i);
			char *desc = cJSON_GetObjectItem(item, "desc")->valuestring;
			int gameID = cJSON_GetObjectItem(item, "gameID")->valueint;
			printf("%s  %d\n",desc,gameID );
		}
	}
	cJSON_Delete(root);
	return 0;
}
