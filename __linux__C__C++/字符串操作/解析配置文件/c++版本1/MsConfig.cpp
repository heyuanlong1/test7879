#include "MsConfig.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define MS_CONF_FILE_PATH "./ko.conf"
#define MS_CONF_LOAD_FAIL						10001		//
#define MS_OK							0
#define MS_FAIL							-1


MsConfig::MsConfig()
{
	_loaded = load();
}
bool MsConfig::load()
{
	std::string f;
	const std::string CharsToTrim = "#;";


	f.append(MS_CONF_FILE_PATH);
	ifstream ifile(f.c_str());

	if (!ifile.is_open()) {
		return false;
	}
	string line;
	string k;
	string v;
	while (getline(ifile, line))
	{
		strTrim(line);
		if (line.empty() || line[0] == '#' || line[0] == ';'){
			if (ifile.eof()){
				break;
			}
			continue;
		}
		line = line.substr(0, line.find_last_not_of(CharsToTrim) + 1);
		if (line.find('=') != std::string::npos) {
			k = line.substr(0, line.find('='));
			v = line.substr(line.find('=') + 1);
			strTrim(k);
			strTrim(v);
			if (!k.empty()){
				_kv.insert(make_pair(k, v));
			}
		}
		if (ifile.eof()) {
			break;
		}
	}
	ifile.close();
	return true;
}
MsConfig::~MsConfig()
{

}

MsConfig* MsConfig::getInstance()
{
	static MsConfig obj;
	return &obj;
}

int  MsConfig::GetString(const std::string &key, std::string &value)
{
	if (_loaded == false) {
		return MS_CONF_LOAD_FAIL;
	}
	std::map<std::string, std::string>::iterator it;
	it = _kv.find(key);
	if (it == _kv.end()) {
		value = "";
		return MS_FAIL;
	}
	else {
		value = it->second;
		return MS_OK;
	}
}
int MsConfig::GetInteger(const std::string &key, int &value)
{
	if (_loaded == false) {
		return MS_CONF_LOAD_FAIL;
	}
	std::string v;
	int ret = GetString(key, v);
	if (ret != MS_FAIL) {
		value = atoi(v.c_str());
	}
	return ret;
}


void MsConfig::strTrim(std::string &str)
{
	const std::string CharsToTrim = " \t\n\r";

	size_t startIndex = str.find_first_not_of(CharsToTrim);
	if (startIndex == std::string::npos)
	{
		str.erase();
		return;
	}

	str = str.substr(startIndex, str.size() - startIndex);
	str = str.substr(0, str.find_last_not_of(CharsToTrim) + 1);
}