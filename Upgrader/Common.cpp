#include "StdAfx.h"
#include "Common.h"

BOOL CCommon::CopyDirectory(CString source, CString target, HWND hwnd)
{
	CreateDirectory(target,NULL); //��ȡĿ�꿽����·�� ����������
	CFileFind finder;  
	CString path=source+_T("\\*.*"); //��Ҫ�������ļ���·��
	// AfxMessageBox(path);  //������
	bool bWorking = finder.FindFile(path);  //�Ƿ��ҵ�����Ҫ�������ļ���·��
	while(bWorking){  
		bWorking = finder.FindNextFile();  
		//AfxMessageBox(finder.GetFileName());  //������
		if(finder.IsDirectory() && !finder.IsDots()){ //�ǲ�����Ч���ļ���
			CopyDirectory(finder.GetFilePath(),target+_T("\\")+finder.GetFileName(), hwnd); //�ݹ�����ļ���
		}  
		else{ //���ļ���ֱ�Ӹ��� 
			auto fileName = new CString(finder.GetFileName());
			CopyFile(finder.GetFilePath(),target+_T("\\")+finder.GetFileName(),FALSE);  //�����ļ����µ������ļ�
			PostMessageA(hwnd, WM_UPDATE_COPY, (WPARAM)fileName, NULL);
		}  
	}
	return TRUE;
}

CString CCommon::GetModuleDir()
{
	HMODULE module = GetModuleHandle(0); 
	char pFileName[MAX_PATH]; 
	GetModuleFileName(module, pFileName, MAX_PATH); 

	CString csFullPath(pFileName); 
	int nPos = csFullPath.ReverseFind( _T('\\') ); 
	if( nPos > 0 ) 
	{
		return csFullPath.Left( nPos ); 
	}			
	return CString(""); 
}

void CCommon::DeleteDirectory(CString strDir)
{
	if(strDir.IsEmpty())
	{ 
		RemoveDirectory(strDir); 
		return; 
	} 
	//����ɾ���ļ������ļ���
	CFileFind   ff; 
	BOOL bFound = ff.FindFile(strDir+ _T("\\*"),0); 
	while(bFound) 
	{ 
		bFound = ff.FindNextFile();
		if(ff.GetFileName()== _T(".")||ff.GetFileName()== _T("..")) 
			continue; 

		//ȥ���ļ�(��)ֻ�������� 
		SetFileAttributes(ff.GetFilePath(),FILE_ATTRIBUTE_NORMAL); 
		if(ff.IsDirectory())  
		{   
			//�ݹ�ɾ�����ļ��� 
			DeleteDirectory(ff.GetFilePath()); 
			RemoveDirectory(ff.GetFilePath()); 
		} 
		else 
		{ 
			DeleteFile(ff.GetFilePath());   //ɾ���ļ� 
		} 
	} 

	ff.Close();
	//Ȼ��ɾ�����ļ��� 
	RemoveDirectory(strDir); 
}
