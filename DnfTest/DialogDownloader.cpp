// DialogDownloader.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogDownloader.h"
#include "afxdialogex.h"
#include "CurlInterface.h"


// DialogDownloader �Ի���

IMPLEMENT_DYNAMIC(DialogDownloader, CDialogEx)

DialogDownloader::DialogDownloader(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogDownloader::IDD, pParent)
{

}

DialogDownloader::~DialogDownloader()
{
}

void DialogDownloader::progressData(void* param)
{
	DialogDownloader* pThis = (DialogDownloader*)param;
	auto fileList = common::getFileList();
	pThis->progressDownload.SetRange(0, fileList.size());
	CCurlInterface curl;
	for (auto Index(fileList.begin()); Index != fileList.end(); Index++)
	{
		curl.fileDownload(*Index, pThis->GetSafeHwnd());
	}
	pThis->PostMessage(WM_CLOSE);
}

void DialogDownloader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, progressDownload);
}


LRESULT DialogDownloader::OnGameDownload(WPARAM wParam, LPARAM lParam)
{
	auto pos = (int)wParam;
	progressDownload.SetPos(pos);
	return 0;
}

BEGIN_MESSAGE_MAP(DialogDownloader, CDialogEx)
	ON_MESSAGE(WM_DOWNLOAD, &DialogDownloader::OnGameDownload)
END_MESSAGE_MAP()


// DialogDownloader ��Ϣ�������


BOOL DialogDownloader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	_beginthread(DialogDownloader::progressData, NULL, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
