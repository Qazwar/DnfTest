#pragma once
#include "afxwin.h"


// CDialogRegister �Ի���

class CDialogRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRegister)

public:
	CDialogRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogRegister();

// �Ի�������
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_EditMac;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
