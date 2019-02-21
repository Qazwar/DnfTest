// DialogIP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogIP.h"
#include "afxdialogex.h"


// DialogIP �Ի���

IMPLEMENT_DYNAMIC(DialogIP, CDialogEx)

DialogIP::DialogIP(CWnd* pParent /*=NULL*/)
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
