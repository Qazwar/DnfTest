// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// DnfAutoTester.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO: �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������
HWND GetLoginWnd()
{
	string strbuffer;
	strbuffer = "���³�����ʿ��¼����";auto iTimes(0);
	HWND hWnd1 = NULL;
	while (hWnd1==NULL&&iTimes++<=20)
	{
		hWnd1 = ::FindWindow(NULL,strbuffer.c_str());
		Sleep(1000);
	}
	return hWnd1;
}


HWND GetGameWnd()
{
	string strbuffer;
	strbuffer = "���³�����ʿ";
	HWND hWnd1 = NULL;
	auto iTimes(0);
	while (hWnd1==NULL&&iTimes++<=20)
	{
		hWnd1 = ::FindWindow(NULL,strbuffer.c_str());
		Sleep(1000);
	}
	return hWnd1;
}

bool GetPath(OUT char* path)
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	GetModuleFileName(NULL, sFilename, _MAX_PATH);
	_splitpath((const char *)sFilename, (char *)sDrive, (char *)sDir, (char *)sFname, (char *)sExt);
	sprintf_s(path,_MAX_PATH,"%s%s",sDrive,sDir);
	return true;
}

void __cdecl _DbgPrint(char* lpszFormat, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[sizeof(TCHAR) * 10240] ={TEXT("WatchDogX ---> Debug:")};
#ifdef UNICODE
	nBuf = _vstprintf(szBuffer + wcslen(szBuffer)*sizeof(TCHAR), lpszFormat, args); 
#else
	nBuf = _vstprintf(szBuffer + strlen(szBuffer)*sizeof(TCHAR), lpszFormat, args); 
#endif
	// was there an error? was the expanded string too long?
	//ASSERT(nBuf >= 0);
	va_end(args);
	OutputDebugString(szBuffer);
#endif // _DEBUG
}