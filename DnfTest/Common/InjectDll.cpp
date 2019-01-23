// InjectDll.cpp: implementation of the CInjectDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InjectDll.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//创建进程方法
BOOL CreatProcessInsertDLL(const TCHAR *lpApplicationName,
						   const TCHAR *lpDLLName,
						   const TCHAR *lpCommandLine,
						   LPSECURITY_ATTRIBUTES lpProcessAttributes,
						   LPSECURITY_ATTRIBUTES lpThreadAttributes,
						   BOOL bInheritHandles,
						   LPVOID lpEnvironment,
						   const TCHAR *lpCurrentDirectory,
						   LPSTARTUPINFO lpStartupInfo,
						   LPPROCESS_INFORMATION lpProcessInformation)
						   
{
	
	//首先挂起方式启动游戏
	//当时主线程的入口就是游戏的线程入口
	//挂起时,EAX寄存器就是程序的入口(OEP)
	//修改EAX寄存器到我们的邪恶内存中(push XXXXXX,Call LoadLibraryA 把邪恶内存清理干净 jmp 原始的OEP)
	try
	{
		//(1)创建进程并暂停
		BOOL IsRet = ::CreateProcess(lpApplicationName,
			(LPSTR)lpCommandLine,
			lpProcessAttributes,lpThreadAttributes,
			bInheritHandles,
			CREATE_SUSPENDED,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			lpProcessInformation);
		if(IsRet == FALSE)
		{
			TRACE(TEXT("CreateProcess GetLastError :%d"),GetLastError());
		}
		CONTEXT conxt = {0};
		conxt.ContextFlags = CONTEXT_FULL;  //********
		//获取到EXE的入口地址
		GetThreadContext(lpProcessInformation->hThread,&conxt);

		TRACE(TEXT("EXE入口地址: %08X"),conxt.Eax);
		DWORD dwEXEPoint = conxt.Eax;  //保存入口点地址

		LPVOID lpMemoryAddress = VirtualAllocEx(lpProcessInformation->hProcess,
			0,
			0x1000,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE);

		TRACE(TEXT("DLL入口地址: %08X"),lpMemoryAddress);

		DWORD dwWritePos = 0; //写入的字节数
		DWORD dwPos = 0;
		//写入DLL的路径
		WriteProcessMemory(lpProcessInformation->hProcess,
			lpMemoryAddress,
			lpDLLName,
			lstrlen(lpDLLName),
			&dwPos);

		dwWritePos += dwPos;
		char StrZero = 0;

		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE ((DWORD)lpMemoryAddress + dwWritePos),
			&StrZero,
			sizeof(char),
			&dwPos);     //字符串构建完毕

		dwWritePos += dwPos;
		//修改入口
		conxt.Eax = (DWORD) lpMemoryAddress + dwWritePos;
		SetThreadContext(lpProcessInformation->hThread,&conxt);
		//push LoadLibraryA参数
		BYTE PUSHCode[5];
		PUSHCode[0] = 0x68;   //push
		*(DWORD*)&PUSHCode[1] = (DWORD)lpMemoryAddress; //DLL名称起始位置
		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE((DWORD)lpMemoryAddress + dwWritePos),
			PUSHCode,
			sizeof(PUSHCode),
			&dwPos);
		dwWritePos +=dwPos;  //Push xxxx
		HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
		//call LoadLibraryW
		BYTE CALLCode[5];
		DWORD dwLoadLibraryWAddress = (DWORD)GetProcAddress(hModule,"LoadLibraryA");
		CALLCode[0] = 0xE8;  
		//TRACE(TEXT("dwLoadLibraryWAddress = %08X,CodeAddress = %08X"),dwLoadLibraryWAddress,(DWORD)lpMemoryAddress + dwWritePos);
		dwLoadLibraryWAddress = dwLoadLibraryWAddress - ((DWORD)lpMemoryAddress + dwWritePos) - 0x5; //XXXX跳转地址
		*(DWORD*)&CALLCode[1] = (DWORD)dwLoadLibraryWAddress;
		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE((DWORD)lpMemoryAddress + dwWritePos),
			CALLCode,
			sizeof(CALLCode),
			&dwPos);

		dwWritePos +=dwPos;

		//jmp 程序入口点
		BYTE JMPCode[5];
		DWORD dwPoint = dwEXEPoint - ((DWORD)lpMemoryAddress + dwWritePos) - 0x5;
		JMPCode[0] = 0xE9;
		*(DWORD*)&JMPCode[1] = dwPoint;
		WriteProcessMemory(lpProcessInformation->hProcess,
				LPBYTE((DWORD)lpMemoryAddress + dwWritePos),
				JMPCode,
				sizeof(JMPCode),
				&dwPos);

		::ResumeThread(lpProcessInformation->hThread);
		Sleep(6000);
		//覆盖缓冲区
		BYTE DeleteBuffer[0x1000] = {0};
		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE(lpMemoryAddress),			
			DeleteBuffer,
			sizeof(DeleteBuffer),
			&dwPos);

		//释放缓冲区
		VirtualFreeEx(lpProcessInformation->hProcess,
			lpMemoryAddress,
			0x1000,
			PAGE_EXECUTE_READWRITE);

		return IsRet;
	}catch(...)
	{
		TRACE(TEXT("CreatProcessInsertDLL() 发生异常"));
	}
	return FALSE;
}

void InjectGameDll(const TCHAR* szGameName,const TCHAR* szDllName,
				   const TCHAR* szGamePath,const TCHAR* szDLLPath)
{
	string strGamePath;
	string strGameFullPath;
	string strDllFullPath;

	strGamePath = szGamePath;

	strGameFullPath = szGamePath;
	strGameFullPath += szGameName;

	strDllFullPath = szDLLPath;
	strDllFullPath += szDllName;

	STARTUPINFO StartupInfo = {0};
	PROCESS_INFORMATION ProcessInformation = {0};
	StartupInfo.cb = sizeof(STARTUPINFO);
	if( CreatProcessInsertDLL(
		strGameFullPath.c_str(),
		strDllFullPath.c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		NULL,
		strGamePath.c_str(),
		&StartupInfo,
		&ProcessInformation) == TRUE )
	{
		_DbgPrint("DLL已经注入!");
	}
}