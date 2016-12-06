#ifndef __MS_CONFIGURE_H__
#define __MS_CONFIGURE_H__


#include <string>
#include <map>



class MsConfig
{
public:
	static MsConfig* getInstance();
	~MsConfig();

	int GetString(const std::string &key, std::string &value);
	int GetInteger(const std::string &key, int &value);

private:
	MsConfig();
	bool load();
	void strTrim(std::string &str);
	
	std::map<std::string, std::string> _kv;
	bool _loaded;
};



#endif