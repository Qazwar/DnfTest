#include "StdAfx.h"
#include "Common.h"

BOOL CCommon::CopyDirectory(CString source, CString target, HWND hwnd)
{
	CreateDirectory(target,NULL); //获取目标拷贝的路径 拷贝至哪里
	CFileFind finder;  
	CString path=source+_T("\\*.*"); //需要拷贝的文件的路径
	// AfxMessageBox(path);  //调试用
	bool bWorking = finder.FindFile(path);  //是否找到了需要拷贝的文件的路径
	while(bWorking){  
		bWorking = finder.FindNextFile();  
		//AfxMessageBox(finder.GetFileName());  //调试用
		if(finder.IsDirectory() && !finder.IsDots()){ //是不是有效的文件夹
			CopyDirectory(finder.GetFilePath(),target+_T("\\")+finder.GetFileName(), hwnd); //递归查找文件夹
		}  
		else{ //是文件则直接复制 
			auto fileName = new CString(finder.GetFileName());
			CopyFile(finder.GetFilePath(),target+_T("\\")+finder.GetFileName(),FALSE);  //拷贝文件夹下的所有文件
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
	//首先删除文件及子文件夹
	CFileFind   ff; 
	BOOL bFound = ff.FindFile(strDir+ _T("\\*"),0); 
	while(bFound) 
	{ 
		bFound = ff.FindNextFile();
		if(ff.GetFileName()== _T(".")||ff.GetFileName()== _T("..")) 
			continue; 

		//去掉文件(夹)只读等属性 
		SetFileAttributes(ff.GetFilePath(),FILE_ATTRIBUTE_NORMAL); 
		if(ff.IsDirectory())  
		{   
			//递归删除子文件夹 
			DeleteDirectory(ff.GetFilePath()); 
			RemoveDirectory(ff.GetFilePath()); 
		} 
		else 
		{ 
			DeleteFile(ff.GetFilePath());   //删除文件 
		} 
	} 

	ff.Close();
	//然后删除该文件夹 
	RemoveDirectory(strDir); 
}
