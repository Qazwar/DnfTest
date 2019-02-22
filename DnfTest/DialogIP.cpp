// DialogIP.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogIP.h"
#include "afxdialogex.h"


// DialogIP 对话框

IMPLEMENT_DYNAMIC(DialogIP, CDialogEx)

DialogIP::DialogIP(CWnd* pParent /*=NULL*/)
	: CChildDialog(DialogIP::IDD, pParent)
{

}

DialogIP::~DialogIP()
{
}

void DialogIP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODE, comboboxMode);
}


BEGIN_MESSAGE_MAP(DialogIP, CDialogEx)
END_MESSAGE_MAP()


// DialogIP 消息处理程序


BOOL DialogIP::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	comboboxMode.AddString(_T("ET代理软件"));
	comboboxMode.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
