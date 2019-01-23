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

//�������̷���
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
	
	//���ȹ���ʽ������Ϸ
	//��ʱ���̵߳���ھ�����Ϸ���߳����
	//����ʱ,EAX�Ĵ������ǳ�������(OEP)
	//�޸�EAX�Ĵ��������ǵ�а���ڴ���(push XXXXXX,Call LoadLibraryA ��а���ڴ�����ɾ� jmp ԭʼ��OEP)
	try
	{
		//(1)�������̲���ͣ
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
		//��ȡ��EXE����ڵ�ַ
		GetThreadContext(lpProcessInformation->hThread,&conxt);

		TRACE(TEXT("EXE��ڵ�ַ: %08X"),conxt.Eax);
		DWORD dwEXEPoint = conxt.Eax;  //������ڵ��ַ

		LPVOID lpMemoryAddress = VirtualAllocEx(lpProcessInformation->hProcess,
			0,
			0x1000,
			MEM_COMMIT,
			PAGE_EXECUTE_READWRITE);

		TRACE(TEXT("DLL��ڵ�ַ: %08X"),lpMemoryAddress);

		DWORD dwWritePos = 0; //д����ֽ���
		DWORD dwPos = 0;
		//д��DLL��·��
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
			&dwPos);     //�ַ����������

		dwWritePos += dwPos;
		//�޸����
		conxt.Eax = (DWORD) lpMemoryAddress + dwWritePos;
		SetThreadContext(lpProcessInformation->hThread,&conxt);
		//push LoadLibraryA����
		BYTE PUSHCode[5];
		PUSHCode[0] = 0x68;   //push
		*(DWORD*)&PUSHCode[1] = (DWORD)lpMemoryAddress; //DLL������ʼλ��
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
		dwLoadLibraryWAddress = dwLoadLibraryWAddress - ((DWORD)lpMemoryAddress + dwWritePos) - 0x5; //XXXX��ת��ַ
		*(DWORD*)&CALLCode[1] = (DWORD)dwLoadLibraryWAddress;
		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE((DWORD)lpMemoryAddress + dwWritePos),
			CALLCode,
			sizeof(CALLCode),
			&dwPos);

		dwWritePos +=dwPos;

		//jmp ������ڵ�
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
		//���ǻ�����
		BYTE DeleteBuffer[0x1000] = {0};
		WriteProcessMemory(lpProcessInformation->hProcess,
			LPBYTE(lpMemoryAddress),			
			DeleteBuffer,
			sizeof(DeleteBuffer),
			&dwPos);

		//�ͷŻ�����
		VirtualFreeEx(lpProcessInformation->hProcess,
			lpMemoryAddress,
			0x1000,
			PAGE_EXECUTE_READWRITE);

		return IsRet;
	}catch(...)
	{
		TRACE(TEXT("CreatProcessInsertDLL() �����쳣"));
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
		_DbgPrint("DLL�Ѿ�ע��!");
	}
}