#pragma once
#include "afxwin.h"


// DialogVerificationCode 对话框

class CChildDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDialog)

public:
	CChildDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChildDialog();
	
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage是消息在送给TranslateMessage函数之前被调用的
	DECLARE_MESSAGE_MAP()
};
