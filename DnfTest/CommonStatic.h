#pragma once
namespace common{
	CString IntToCString(const int& i);
	CString stringToCString(const string& str);
	string CStringTostring(const CString& cs);
	CStringArray* SplitString(const string& str,  char split);

}