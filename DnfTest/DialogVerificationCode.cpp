// DialogVerificationCode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogVerificationCode.h"
#include "afxdialogex.h"


// DialogVerificationCode �Ի���

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


// DialogVerificationCode ��Ϣ�������



BOOL DialogVerificationCode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	comboBoxPlatform.AddString(_T("���˴���"));
	comboBoxPlatform.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
