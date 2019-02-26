#pragma once
#include "CJsonObject\CJsonObject.hpp"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;


namespace common{
	CString IntToCString(const int& i);
	CString stringToCString(const string& str);
	string CStringTostring(const CString& cs);
	CStringArray* SplitString(const string& str,  char split);
	vector<CString> SplitCString(CString strSource, CString ch);
	string getMAC();
	int QueryUser();
	int RegisterUser();
	string GetDefines();
	void PostConfig(const CString& data);
	string GetConfig();
	string GetVersion();
	int Upgrade();
	list<string> getFileList();
	string GetFilePath(const string& path);
	CString GetFileName(CString path);
	CString GetModuleDir();
	void CreateDir(CString path);
	BOOL DirectoryExist(const CString&path);
	BOOL CreateDirectory(const CString&path);
}