#include "stdafx.h"
#include "config.h"
#include <string>
#include <fstream>
#include <sstream>
#include "CJsonObject\CJsonObject.hpp"


Config::Config()
{
	neb::CJsonObject pt_root;
	pt_root.Parse(readFileIntoString("config.json"));
	pt_root.Get("game_path", game_path);
	pt_root.Get("risk_group", risk_group);
	pt_root.Get("role_name", role_name);
	pt_root.Get("ip_address", ip_address);
	pt_root.Get("ip_try_times", ip_try_times);
	auto gameArea = pt_root["area"];
	for (auto i(0); i < gameArea.GetArraySize(); i++)
	{
		const auto item = gameArea[i];
		area TmpArea;
		item.Get("name", TmpArea.name);
		neb::CJsonObject server;
		item.Get("server", server);
		for (auto j(0); j < server.GetArraySize(); j++)
		{
			string server_name;
			server.Get(j, server_name);
			TmpArea.server.push_back(server_name);
		}
		game_area.push_back(TmpArea);
	}
	neb::CJsonObject pt_root_account;
	pt_root_account.Parse(readFileIntoString("account_config.json"));

	auto accounts = pt_root_account["accounts"];
	auto length = accounts.GetArraySize();
	for (auto i(0); i < length; i++)
	{
		const auto item = accounts[i];
		account_info account;
		item.Get("qq", account.qq);
		item.Get("password", account.password);
		item.Get("role_name", account.role_name);
		item.Get("status", account.status);
		this->accounts.push_back(account);
	}
}

std::string Config::readFileIntoString(char * filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

void Config::SaveData()
{
	ofstream ofile("config.json");
	neb::CJsonObject pt_root;
	pt_root.Add("game_path", this->game_path);	
	auto str = pt_root.ToString();
	ofile.write(str.c_str(), str.size());
	ofile.close();
	SaveAccountData();
}

void Config::SaveAccountData()
{
	ofstream ofile("account_config.json");
	neb::CJsonObject pt_root;
	neb::CJsonObject pt_accounts;
	for (auto i(0); i < this->accounts.size(); i++)
	{
		const auto account = this->accounts.at(i);
		neb::CJsonObject pt_item;
		pt_item.Add("qq", account.qq);
		pt_item.Add("password", account.password);
		pt_item.Add("role_name", account.role_name);
		pt_item.Add("status", account.status);
		pt_accounts.Add(pt_item);
	}
	pt_root.Add("accounts",pt_accounts);
	auto str = pt_root.ToString();
	ofile.write(str.c_str(), str.size());
	ofile.close();
}

account_info::account_info(const string& qq, const string& password, const string& role_name)
{
	this->qq = qq;
	this->password = password;
	this->role_name = role_name;
	this->status = STATUS_INIT;
}
