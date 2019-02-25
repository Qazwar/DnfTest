#pragma once
namespace CCommon
{
	BOOL CopyDirectory(CString source, CString target, HWND hwnd);
	CString GetModuleDir();
	void DeleteDirectory(CString strDir);
};

