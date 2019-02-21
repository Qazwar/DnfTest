// DialogVerificationCode.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "ChildDialog.h"


// DialogVerificationCode 对话框

IMPLEMENT_DYNAMIC(CChildDialog, CDialogEx)

CChildDialog::CChildDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialogEx(nIDTemplate, pParent)
{

}

CChildDialog::~CChildDialog()
{
}

BEGIN_MESSAGE_MAP(CChildDialog, CDialogEx)
END_MESSAGE_MAP()

BOOL CChildDialog::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽ESC关闭窗体/
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE )
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
