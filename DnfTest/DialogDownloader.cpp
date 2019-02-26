// DialogDownloader.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DialogDownloader.h"
#include "afxdialogex.h"
#include "CurlInterface.h"


// DialogDownloader 对话框

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
		CString Text;
		Text.Format(_T("当前下载[%s]"), common::GetFileName(common::stringToCString(*Index)));
		pThis->GetDlgItem(IDC_STATIC_FILENAME)->SetWindowText(Text);
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
	CString Text;
	Text.Format(_T("%d"), pos);
	progressDownload.SetWindowText(Text);
	return 0;
}

BEGIN_MESSAGE_MAP(DialogDownloader, CDialogEx)
	ON_MESSAGE(WM_DOWNLOAD, &DialogDownloader::OnGameDownload)
END_MESSAGE_MAP()


// DialogDownloader 消息处理程序


BOOL DialogDownloader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	_beginthread(DialogDownloader::progressData, NULL, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
