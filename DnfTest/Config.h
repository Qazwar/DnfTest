#include <boost/serialization/singleton.hpp>
#include "CJsonObject/CJsonObject.hpp"



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
//第一职业
class first_profession{
public:
	string name;
	vector<string> profession;
};

class profession_position{
public:
	string name;
	int positionX;
	int positionY;
};

class Config :public boost::serialization::singleton<Config>
{

public:
	Config();
	string ip_address;
	string game_path;
	vector<area> game_area;
	string risk_group_type;
	string role_name_type;
	int ip_try_times;
	string first_role;
	string second_role;
	vector<account_info> accounts;
	vector<first_profession> professions;
	vector<profession_position> professionPositions;
	void SaveData();
	void SaveAccountData();
private:
	string readFileIntoString(char * filename);
	neb::CJsonObject m_file_root;
	  
};  
#define config_instance Config::get_mutable_instance()