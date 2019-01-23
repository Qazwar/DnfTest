
// DnfTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DnfTestDlg.h"
#include "afxdialogex.h"
#include "GameControl.h"
#include "winio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDnfTestDlg 对话框




CDnfTestDlg::CDnfTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDnfTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDnfTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDnfTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDnfTestDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ROLE, &CDnfTestDlg::OnBnClickedButtonCreateRole)
	ON_BN_CLICKED(IDC_BUTTON1, &CDnfTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ROLES, &CDnfTestDlg::OnBnClickedButtonCreateRoles)
END_MESSAGE_MAP()


// CDnfTestDlg 消息处理程序

BOOL CDnfTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//初始化WinIo库
	InitializeWinIo();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDnfTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDnfTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDnfTestDlg::OnBnClickedButtonStart()
{
	_beginthread(CDnfTestDlg::StartProcess, NULL, NULL);
}

void CDnfTestDlg::StartProcess(void*)
{
	// TODO: 在此添加控件通知处理程序代码
	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
	//	StartupInfo.cb = sizeof(STARTUPINFOA);

	// 	char strFilePath1[MAX_PATH] = "D:\\天涯明月刀\\TCLS\\Client.exe";              //新进程执行程序
	// 
	// 
	// 	PROCESS_INFORMATION pt;
	// 	STARTUPINFO si;                                       //PC版这个参数不加，会导致创建进程出错，wce不用，直接为NULL
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
	StartupInfo.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
	CreateProcessA("C:\\Program Files\\腾讯游戏\\地下城与勇士\\TCLS\\Client.exe",
	NULL,
	NULL, NULL,
	0,
	NULL,
	NULL,
	"C:\\Program Files\\腾讯游戏\\地下城与勇士\\TCLS\\",
	&StartupInfo,
	&ProcessInformation);
	Sleep(5000);
	CGameControl gameControl;
	gameControl.InputCodes();
	/*gameControl.CreateRole(_T("kkkgggg"));
	gameControl.EndGame();*/
}


void CDnfTestDlg::StartInputCodes(void*)
{
	CGameControl gameControl;
	gameControl.InputCodes();
}

void CDnfTestDlg::StartCreateRole(void*)
{
	CGameControl gameControl;
	gameControl.CreateRole();
	gameControl.EndGame();
}

void CDnfTestDlg::OnBnClickedButtonCreateRole()
{
	// TODO: 在此添加控件通知处理程序代码
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, NULL);
}


void CDnfTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, NULL);
}


void CDnfTestDlg::OnBnClickedButtonCreateRoles()
{
	// TODO: 在此添加控件通知处理程序代码
	CGameControl gameControl;
	gameControl.CreateRole();
}
