// DialogIP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogIP.h"
#include "afxdialogex.h"


// DialogIP �Ի���

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


// DialogIP ��Ϣ�������


BOOL DialogIP::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	comboboxMode.AddString(_T("ET�������"));
	comboboxMode.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
