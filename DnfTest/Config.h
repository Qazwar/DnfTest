#include <boost/serialization/singleton.hpp>



class account_info{
public:
	string qq;
	string password;
	string role_name;
	int status;
	account_info(const string& qq = "", const string& password = "", const string& role_name = "");
};

class area{
public:
	string name;
	vector<string> server;
};

class Config :public boost::serialization::singleton<Config>
{

public:
	Config();
	string game_path;
	vector<area> game_area;
	string risk_group;
	string role_name;
	vector<account_info> accounts;
	void SaveData();
	void SaveAccountData();
private:
	string readFileIntoString(char * filename);
	  
};  
#define config_instance Config::get_mutable_instance()