#pragma once
#include "afxcmn.h"


// DialogDownloader 对话框

class DialogDownloader : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDownloader)

public:
	DialogDownloader(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DialogDownloader();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOADER };
	static void progressData(void* param);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	LRESULT OnGameDownload(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progressDownload;
	virtual BOOL OnInitDialog();
};
