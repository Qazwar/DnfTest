// DialogRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogRegister.h"
#include "afxdialogex.h"


// CDialogRegister 对话框

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


// CDialogRegister 消息处理程序


BOOL CDialogRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EditMac.SetWindowText(global_instance.getMac().c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogRegister::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(common::RegisterUser()==0){
		AfxMessageBox("已经发送注册信息");
		PostQuitMessage(0);
	}
	CDialogEx::OnOK();
}
