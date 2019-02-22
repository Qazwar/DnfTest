// DialogVerificationCode.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogVerificationCode.h"
#include "afxdialogex.h"
#include "VerificationCode.h"
#include "Config.h"


// DialogVerificationCode 对话框

IMPLEMENT_DYNAMIC(DialogVerificationCode, CDialogEx)

DialogVerificationCode::DialogVerificationCode(CWnd* pParent /*=NULL*/)
	: CChildDialog(DialogVerificationCode::IDD, pParent)
	, accountCode(_T(""))
	, password(_T(""))
{

}

DialogVerificationCode::~DialogVerificationCode()
{
}

void DialogVerificationCode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PLATFORM, comboBoxPlatform);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT_CODE, accountCode);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, password);
}


BEGIN_MESSAGE_MAP(DialogVerificationCode, CChildDialog)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &DialogVerificationCode::OnBnClickedButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_RECHARGE, &DialogVerificationCode::OnBnClickedButtonRecharge)
END_MESSAGE_MAP()


// DialogVerificationCode 消息处理程序



BOOL DialogVerificationCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	comboBoxPlatform.AddString(_T("超人打码"));
	comboBoxPlatform.SetCurSel(0);
	accountCode = common::stringToCString(config_instance.verification_account_code);
	password = common::stringToCString(config_instance.verification_password);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void DialogVerificationCode::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CString strRe = CVerificationCode::Ptr()->pGetUserInfo(accountCode, password);
	GetDlgItem(IDC_STATIC_BALANCE)->SetWindowText(strRe+_T("积分"));
	config_instance.verification_account_code = common::CStringTostring(accountCode);
	config_instance.verification_password = common::CStringTostring(password);
}


void DialogVerificationCode::OnBnClickedButtonRecharge()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(0,NULL,_T("http://www.chaorendama.com/"),NULL,NULL,SW_NORMAL);
}
