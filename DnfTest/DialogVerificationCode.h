#pragma once
#include "afxwin.h"
#include "Controls\ChildDialog.h"


// DialogVerificationCode 对话框

class DialogVerificationCode : public CChildDialog
{
	DECLARE_DYNAMIC(DialogVerificationCode)

public:
	DialogVerificationCode(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogVerificationCode();

// 对话框数据
	enum { IDD = IDD_DIALOG_VERIFICATION_CODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox comboBoxPlatform;
	CString accountCode;
	CString password;
	afx_msg void OnBnClickedButtonQuery();
	afx_msg void OnBnClickedButtonRecharge();
};
