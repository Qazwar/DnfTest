
// Upgrader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUpgraderApp:
// �йش����ʵ�֣������ Upgrader.cpp
//

class CUpgraderApp : public CWinApp
{
public:
	CUpgraderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpgraderApp theApp;