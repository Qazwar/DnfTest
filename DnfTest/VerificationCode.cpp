#include "StdAfx.h"
#include "VerificationCode.h"


CVerificationCode::CVerificationCode(void)
{
	hyzmIns = LoadLibrary(_T("dc.dll"));
	pGetUserInfo=(lpGetUserInfo)GetProcAddress (hyzmIns,"GetUserInfo");
	pRecByte_A=(lpRecByte_A)GetProcAddress (hyzmIns,"RecByte_A");
	pRecYZM_A=(lpRecYZM_A)GetProcAddress (hyzmIns,"RecYZM_A");
	pReportError=(lpReportError)GetProcAddress (hyzmIns,"ReportError");
}


CVerificationCode::~CVerificationCode(void)
{
	FreeLibrary(hyzmIns);
}
