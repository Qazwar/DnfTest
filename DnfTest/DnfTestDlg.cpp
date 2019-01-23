
// DnfTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DnfTestDlg.h"
#include "afxdialogex.h"
#include "GameControl.h"
#include "winio.h"
#include "Config.h"

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
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_ListAccount);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_GAME, m_EditGame);
}

BEGIN_MESSAGE_MAP(CDnfTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDnfTestDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ROLE, &CDnfTestDlg::OnBnClickedButtonCreateRole)
	ON_BN_CLICKED(IDC_BUTTON1, &CDnfTestDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnChangeMfceditbrowseGame)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnUpdateMfceditbrowseGame)
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
	initListCtrl();
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
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//指定wShowWindow成员有效
	StartupInfo.wShowWindow = TRUE;//此成员设为TRUE的话则显示新建进程的主窗口
	CreateProcessA((config_instance.game_path+"\\Client.exe").c_str(),
		NULL,
		NULL, NULL,
		0,
		NULL,
		NULL,
		config_instance.game_path.c_str(),
		&StartupInfo,
		&ProcessInformation);
	if(ProcessInformation.hProcess==NULL)
	{
		AfxMessageBox(_T("启动游戏失败"), MB_OK);
		return;
	}
	CGameControl gameControl;
	gameControl.InputCodes();
	gameControl.CreateRole();
	gameControl.EndGame();
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

void CDnfTestDlg::initListCtrl()
{
	m_EditGame.SetWindowText(common::stringToCString(config_instance.game_path));
	m_ListAccount.InsertColumn(0,"序列号",LVCFMT_LEFT, 50);
	m_ListAccount.InsertColumn(1,"状态", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(2,"qq号码", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(3,"密码",LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(4,"角色名", LVCFMT_LEFT, 200);
	for(auto i(0); i < config_instance.accounts.size(); i++)
	{
		 const auto& account = config_instance.accounts.at(i);
		 m_ListAccount.InsertItem(0, common::IntToCString(i+1));
		 m_ListAccount.SetItemText(i, 1, _T("启动中"));
		 m_ListAccount.SetItemText(i, 2, common::stringToCString(account.qq));
		 m_ListAccount.SetItemText(i, 3, common::stringToCString(account.password));
		 m_ListAccount.SetItemText(i, 4, common::stringToCString(account.role_name));
	}
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




void CDnfTestDlg::OnEnChangeMfceditbrowseGame()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString Text;
	m_EditGame.GetWindowText(Text);
	if(Text.Compare(common::stringToCString(config_instance.game_path))!=0){
		config_instance.game_path = common::CStringTostring(Text);
		config_instance.SaveData();
	}
}


void CDnfTestDlg::OnEnUpdateMfceditbrowseGame()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
