#pragma once
#include "afxwin.h"
#include "Controls\ChildDialog.h"


// DialogIP 对话框

class DialogIP : public CChildDialog
{
	DECLARE_DYNAMIC(DialogIP)

public:
	DialogIP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogIP();

// 对话框数据
	enum { IDD = IDD_DIALOG_VPN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboboxMode;
};
