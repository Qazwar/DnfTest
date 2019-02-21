#include "stdafx.h"
#include "config.h"
#include <string>
#include <fstream>
#include <sstream>
#include "CJsonObject\CJsonObject.hpp"


Config::Config()
{
	neb::CJsonObject file_root;
	file_root.Parse(common::GetConfig());
	neb::CJsonObject configData;
	file_root.Get("config", configData);
	ip_try_times = 10;
	configData.Get("game_path", game_path);
	configData.Get("ip_address", ip_address);
	//configData.Get("ip_try_times", ip_try_times);
	configData.Get("firstRole", firstRole);
	configData.Get("firstRoleProfession", firstRoleProfession);
	configData.Get("secondRole", secondRole);
	configData.Get("secondRoleProfession", secondRoleProfession);
	configData.Get("servername", servername);
	configData.Get("areaname", areaname);
	configData.Get("loginFailTimes", loginFailTimes);
	LoadServerConfig();
	LoadAccountConfig();	
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

void Config::SaveData()
{
	neb::CJsonObject root;
	root.Add("game_path", this->game_path);
	root.Add("ip_address", this->ip_address);
	root.Add("ip_try_times", this->ip_try_times);
	root.Add("firstRole", this->firstRole);
	root.Add("firstRoleProfession", this->firstRoleProfession);
	root.Add("secondRole", this->secondRole);
	root.Add("secondRoleProfession", this->secondRoleProfession);
	root.Add("servername", this->servername);
	root.Add("areaname", this->areaname);
	root.Add("loginFailTimes", this->loginFailTimes);
	SaveAccountData();
	neb::CJsonObject base;
	base.Add("config", root);
	common::PostConfig(common::stringToCString(base.ToString()));
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

void Config::LoadAccountConfig()
{
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
		item.Get("status", account.status);
		this->accounts.push_back(account);
	}
}

void Config::LoadServerConfig()
{
	neb::CJsonObject server_root;
	server_root.Parse(common::GetDefines());
	server_root.Get("risk_group_type", risk_group_type);
	server_root.Get("role_name_type", role_name_type);
	server_root.Get("first_role", first_role);
	server_root.Get("second_role", second_role);
	auto gameArea = server_root["area"];
	for (auto i(0); i < gameArea.GetArraySize(); i++)
	{
		const auto item = gameArea[i];
		area TmpArea;
		item.Get("name", TmpArea.name);
		item.Get("group", TmpArea.group);
		item.Get("index", TmpArea.index);
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

	auto profession = server_root["profession"];
	for (auto i(0); i < profession.GetArraySize(); i++)
	{
		const auto item = profession[i];
		first_profession TmpProfession;
		item.Get("name", TmpProfession.name);
		neb::CJsonObject profession;
		item.Get("profession", profession);
		for (auto j(0); j < profession.GetArraySize(); j++)
		{
			string profession_name;
			profession.Get(j, profession_name);
			TmpProfession.profession.push_back(profession_name);
		}
		professions.push_back(TmpProfession);
	}

	auto profession_positions = server_root["profession_position"];
	for (auto i(0); i < profession_positions.GetArraySize(); i++)
	{
		profession_position item;
		profession_positions[i].Get("name", item.name);
		profession_positions[i].Get("positionX", item.positionX);
		profession_positions[i].Get("positionY", item.positionY);
		professionPositions.push_back(item);
	}
}

account_info::account_info(const string& qq, const string& password, const string& role_name)
{
	this->qq = qq;
	this->password = password;
	this->role_name = role_name;
	this->status = STATUS_INIT;
}
