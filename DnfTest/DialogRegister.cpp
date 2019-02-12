// DialogRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogRegister.h"
#include "afxdialogex.h"


// CDialogRegister �Ի���

IMPLEMENT_DYNAMIC(CDialogRegister, CDialogEx)

CDialogRegister::CDialogRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRegister::IDD, pParent)
{

}

CDialogRegister::~CDialogRegister()
{
}

void CDialogRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAC, m_EditMac);
}


BEGIN_MESSAGE_MAP(CDialogRegister, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogRegister::OnBnClickedOk)
END_MESSAGE_MAP()


// CDialogRegister ��Ϣ�������


BOOL CDialogRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_EditMac.SetWindowText(global_instance.getMac().c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogRegister::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(common::RegisterUser()==0){
		AfxMessageBox("�Ѿ�����ע����Ϣ");
		PostQuitMessage(0);
	}
	CDialogEx::OnOK();
}
