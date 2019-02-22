#include "StdAfx.h"
#include "FileParser.h"
#include <fstream>
#include <sstream>
#include "Config.h"

CFileParser::CFileParser(void)
{
}


CFileParser::~CFileParser(void)
{
}

void CFileParser::LoadData(const CString& path)
{
	CStdioFile mFile;
	if (mFile.Open(path,CFile::modeReadWrite))
	{
		CString strData = "";
		config_instance.accounts.clear();
		while(mFile.ReadString(strData)){
			auto data = common::SplitCString(strData, "||||");
			if (data.size()==2)
			{
				account_info account;
				account.qq = common::CStringTostring(data.at(0));
				account.password = common::CStringTostring(data.at(1));
				config_instance.accounts.push_back(account);
			}
		};
		mFile.Close();
	}
}

void CFileParser::SaveData()
{
	map<CString, AccountStatus> mapFiles;
	mapFiles.insert(pair<CString, AccountStatus>(_T("��¼ʧ���б�.txt"),STATUS_LOGIN_FAILED));
	mapFiles.insert(pair<CString, AccountStatus>(_T("������ɫ�ɹ��б�.txt"),STATUS_SUCCESS));
	mapFiles.insert(pair<CString, AccountStatus>(_T("��Ϸ�쳣�˺��б�.txt"),STATUS_EXCEPTION));
	CTime time = CTime::GetCurrentTime();
	CString text;
	text.Format(_T("��%d��%d��%d��%dʱ%d��%d�롿DNF�ʺ���������\\"), time.GetYear(), time.GetMonth(), time.GetDay(),
		time.GetHour(), time.GetMinute(), time.GetSecond());
	if (!PathIsDirectory(text))
	{
		CreateDirectory(text, 0);//�������򴴽�
	}
	for (auto index(mapFiles.begin()); index != mapFiles.end(); index++)
	{
		CStdioFile mFile;
		if (mFile.Open( text+index->first, CFile::modeReadWrite|CFile::modeCreate))
		{
			for ( auto account(config_instance.accounts.begin()); account != config_instance.accounts.end(); account++)
			{
				if(account->accountStatus == index->second){
					CString content;
					content.Append(common::stringToCString(account->qq));
					content.Append(_T("||||"));
					content.Append(common::stringToCString(account->password));
					if(account->role_name.size()>0){
						content.Append(_T("||||"));
						content.Append(_T("��ɫ"));
						for(auto roleIndex(0); roleIndex < account->role_name.size(); roleIndex++)
						{
							content.Append(_T("||||"));
							content.Append(common::stringToCString(account->role_name.at(roleIndex)));
						}
					}
				}
			}			
			mFile.Close();
		}
	}
}
