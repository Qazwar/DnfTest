#pragma once
#include "singleton.h"

class CVerificationCode:public Pattern::Singleton<CVerificationCode>
{
public:
	CVerificationCode(void);
	~CVerificationCode(void);
private:
	HMODULE hyzmIns;
	//动态声明dll函数
	typedef  LPSTR  (__stdcall *lpGetUserInfo)(LPCSTR strUser,LPCSTR strPass);  //GetUserInfo获取剩余点数
	typedef LPSTR (__stdcall *lpRecByte_A)(BYTE* byte,int len,LPCSTR strUser,LPCSTR strPass,LPCSTR strSoftId);  //RecByte_A按图片字节识别
	typedef LPSTR (__stdcall *lpRecYZM_A)(LPCSTR strPath,LPCSTR strUser,LPCSTR strPass,LPCSTR strSoftId);//RecYzm_A 按图片本地路径识别
	typedef void (__stdcall *lpReportError)(LPCSTR strUser,LPCSTR  strDaMaworker);   //ReportError报告错误
public:
	lpGetUserInfo  pGetUserInfo;
	lpRecByte_A pRecByte_A;
	lpRecYZM_A pRecYZM_A;
	lpReportError  pReportError;
};

