// InjectDll.h: interface for the CInjectDll class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INJECTDLL_H__92737EC2_7306_49F1_A56D_7821A853A370__INCLUDED_)
#define AFX_INJECTDLL_H__92737EC2_7306_49F1_A56D_7821A853A370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL CreatProcessInsertDLL(const TCHAR *lpApplicationName,
	const TCHAR *lpDLLName,
	const TCHAR *lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	LPVOID lpEnvironment,
	const TCHAR *lpCurrentDirectory,
	LPSTARTUPINFO lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation);

void InjectGameDll(const TCHAR* szGameName,const TCHAR* szDllName,
				   const TCHAR* szGamePath,const TCHAR* szDLLPath);
						   
#endif // !defined(AFX_INJECTDLL_H__92737EC2_7306_49F1_A56D_7821A853A370__INCLUDED_)
