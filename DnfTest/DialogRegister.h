#pragma once
#include "afxwin.h"


// CDialogRegister 对话框

class CDialogRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRegister)

public:
	CDialogRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogRegister();

// 对话框数据
	enum { IDD = IDD_DIALOG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_EditMac;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
