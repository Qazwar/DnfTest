#pragma once
#include "afxwin.h"


// DialogVerificationCode �Ի���

class CChildDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDialog)

public:
	CChildDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChildDialog();
	
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage����Ϣ���͸�TranslateMessage����֮ǰ�����õ�
	DECLARE_MESSAGE_MAP()
};
