// DialogVerificationCode.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogVerificationCode.h"
#include "afxdialogex.h"


// DialogVerificationCode 对话框

IMPLEMENT_DYNAMIC(DialogVerificationCode, CDialogEx)

DialogVerificationCode::DialogVerificationCode(CWnd* pParent /*=NULL*/)
	: CChildDialog(DialogVerificationCode::IDD, pParent)
{

}

DialogVerificationCode::~DialogVerificationCode()
{
}

void DialogVerificationCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PLATFORM, comboBoxPlatform);
}


BEGIN_MESSAGE_MAP(DialogVerificationCode, CChildDialog)
END_MESSAGE_MAP()


// DialogVerificationCode 消息处理程序



BOOL DialogVerificationCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	comboBoxPlatform.AddString(_T("超人打码"));
	comboBoxPlatform.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
