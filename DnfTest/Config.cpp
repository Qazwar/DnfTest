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
	auto accounts = pt_root["accounts"];
	auto length = accounts.GetArraySize();
	for (auto i(0); i < length; i++)
	{
		const auto item = accounts[i];
		account_info account;
		item.Get("qq", account.qq);
		item.Get("password", account.password);
		item.Get("role_name", account.role_name);
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
	neb::CJsonObject pt_accounts;
	for (auto i(0); i < this->accounts.size(); i++)
	{
		const auto account = this->accounts.at(i);
		neb::CJsonObject pt_item;
		pt_item.Add("qq", account.qq);
		pt_item.Add("password", account.password);
		pt_item.Add("role_name", account.role_name);
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
}
