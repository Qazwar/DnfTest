// DialogVerificationCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogVerificationCode.h"
#include "afxdialogex.h"
#include "VerificationCode.h"
#include "Config.h"


// DialogVerificationCode �Ի���

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


// DialogVerificationCode ��Ϣ�������



BOOL DialogVerificationCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	comboBoxPlatform.AddString(_T("���˴���"));
	comboBoxPlatform.SetCurSel(0);
	accountCode = common::stringToCString(config_instance.verification_account_code);
	password = common::stringToCString(config_instance.verification_password);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void DialogVerificationCode::OnBnClickedButtonQuery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	CString strRe = CVerificationCode::Ptr()->pGetUserInfo(accountCode, password);
	GetDlgItem(IDC_STATIC_BALANCE)->SetWindowText(strRe+_T("����"));
	config_instance.verification_account_code = common::CStringTostring(accountCode);
	config_instance.verification_password = common::CStringTostring(password);
}


void DialogVerificationCode::OnBnClickedButtonRecharge()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(0,NULL,_T("http://www.chaorendama.com/"),NULL,NULL,SW_NORMAL);
}
