#pragma once
#include "afxcmn.h"


// DialogDownloader �Ի���

class DialogDownloader : public CDialogEx
{
	DECLARE_DYNAMIC(DialogDownloader)

public:
	DialogDownloader(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogDownloader();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOADER };
	static void progressData(void* param);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	LRESULT OnGameDownload(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl progressDownload;
	virtual BOOL OnInitDialog();
};
