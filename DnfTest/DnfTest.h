
// DnfTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDnfTestApp:
// �йش����ʵ�֣������ DnfTest.cpp
//

class CDnfTestApp : public CWinApp
{
public:
	CDnfTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDnfTestApp theApp;