#pragma once
#include "afxwin.h"
#include "Controls\ChildDialog.h"


// DialogVerificationCode �Ի���

class DialogVerificationCode : public CChildDialog
{
	DECLARE_DYNAMIC(DialogVerificationCode)

public:
	DialogVerificationCode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogVerificationCode();

// �Ի�������
	enum { IDD = IDD_DIALOG_VERIFICATION_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox comboBoxPlatform;
	CString accountCode;
	CString password;
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonRecharge();
};
