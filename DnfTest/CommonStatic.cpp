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

	CStringArray* SplitString(const string& str,  char split)
	{
		CStringArray *strArray = new CStringArray;
		auto cs = stringToCString(str);
		int iIndex = 0;
		while (1)
		{
			iIndex = cs.Find(split);
			if(iIndex >= 0)
			{
				strArray->Add(cs.Left(iIndex));
				cs = cs.Right(cs.GetLength()-iIndex-1);
			}
			else
			{
				break;
			}
		}
		strArray->Add(cs);
		return strArray;
	}

}