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
	configData.Get("verification_account_code", verification_account_code);
	configData.Get("verification_password", verification_password);
	LoadServerConfig();	
}

void Config::SaveData()
{
	neb::CJsonObject root;
	root.Add("game_path", game_path);
	root.Add("ip_address", ip_address);
	root.Add("ip_try_times", ip_try_times);
	root.Add("firstRole", firstRole);
	root.Add("firstRoleProfession", firstRoleProfession);
	root.Add("secondRole", secondRole);
	root.Add("secondRoleProfession", secondRoleProfession);
	root.Add("servername", servername);
	root.Add("areaname", areaname);
	root.Add("loginFailTimes", loginFailTimes);
	root.Add("verification_account_code", verification_account_code);
	root.Add("verification_password", verification_password);
	neb::CJsonObject base;
	base.Add("config", root);
	common::PostConfig(common::stringToCString(base.ToString()));
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

account_info::account_info(const string& qq, const string& password)
{
	this->qq = qq;
	this->password = password;
	this->accountStatus = STATUS_INIT;
}
