#pragma once
#include "afxwin.h"
#include "Controls\ChildDialog.h"


// DialogIP �Ի���

class DialogIP : public CChildDialog
{
	DECLARE_DYNAMIC(DialogIP)

public:
	DialogIP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogIP();

// �Ի�������
	enum { IDD = IDD_DIALOG_VPN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboboxMode;
};
