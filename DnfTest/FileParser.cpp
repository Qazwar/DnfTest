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
