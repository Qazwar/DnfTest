#include <boost/serialization/singleton.hpp>

class account_info{
public:
	string qq;
	string password;
	string role_name;
	account_info(const string& qq = "", const string& password = "", const string& role_name = "");
};


class Config :public boost::serialization::singleton<Config>
{

public:
	Config();
	vector<account_info> accounts;
private:
	string readFileIntoString(char * filename);
	  
};  
#define config_instance Config::get_mutable_instance()