#include "stdafx.h"


namespace common{
	CString IntToCString(const int& i)
	{
		CString csText;
		csText.Format(_T("%d"), i);
		return csText;
	}

	CString stringToCString(const string& str)
	{
		CString ans;  
		ans.Format("%s", str.c_str());  
		return ans;
	}

	string CStringTostring(const CString& cs)
	{
		 return (LPSTR)(LPCTSTR)cs;
	}
}