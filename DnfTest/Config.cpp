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
	auto length = pt_root.GetArraySize();
	for (auto i(0); i < length; i++)
	{
		const auto item = pt_root[i];
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
	//���ļ����뵽ostringstream����buf��
	ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//������������buf�������ַ���
	return buf.str();
}

account_info::account_info(const string& qq, const string& password, const string& role_name)
{
	this->qq = qq;
	this->password = password;
	this->role_name = role_name;
}
