#pragma once
#include "CJsonObject\CJsonObject.hpp"
namespace common{
	CString IntToCString(const int& i);
	CString stringToCString(const string& str);
	string CStringTostring(const CString& cs);
	CStringArray* SplitString(const string& str,  char split);
	string getMAC();
	int QueryUser();
	int RegisterUser();
	string GetDefines();
}