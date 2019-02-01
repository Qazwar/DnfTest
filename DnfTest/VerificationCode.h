#pragma once
#include "singleton.h"

class CVerificationCode:public Pattern::Singleton<CVerificationCode>
{
public:
	CVerificationCode(void);
	~CVerificationCode(void);
private:
	HMODULE hyzmIns;
	//��̬����dll����
	typedef  LPSTR  (__stdcall *lpGetUserInfo)(LPCSTR strUser,LPCSTR strPass);  //GetUserInfo��ȡʣ�����
	typedef LPSTR (__stdcall *lpRecByte_A)(BYTE* byte,int len,LPCSTR strUser,LPCSTR strPass,LPCSTR strSoftId);  //RecByte_A��ͼƬ�ֽ�ʶ��
	typedef LPSTR (__stdcall *lpRecYZM_A)(LPCSTR strPath,LPCSTR strUser,LPCSTR strPass,LPCSTR strSoftId);//RecYzm_A ��ͼƬ����·��ʶ��
	typedef void (__stdcall *lpReportError)(LPCSTR strUser,LPCSTR  strDaMaworker);   //ReportError�������
public:
	lpGetUserInfo  pGetUserInfo;
	lpRecByte_A pRecByte_A;
	lpRecYZM_A pRecYZM_A;
	lpReportError  pReportError;
};

