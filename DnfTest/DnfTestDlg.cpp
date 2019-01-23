
// DnfTestDlg.cpp : ʵ���ļ�
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


// CDnfTestDlg �Ի���




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


// CDnfTestDlg ��Ϣ�������

BOOL CDnfTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ��WinIo��
	initListCtrl();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDnfTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	STARTUPINFOA StartupInfo;
	PROCESS_INFORMATION ProcessInformation;
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//ָ��wShowWindow��Ա��Ч
	StartupInfo.wShowWindow = TRUE;//�˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵�������
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
		AfxMessageBox(_T("������Ϸʧ��"), MB_OK);
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
	m_ListAccount.InsertColumn(0,"���к�",LVCFMT_LEFT, 50);
	m_ListAccount.InsertColumn(1,"״̬", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(2,"qq����", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(3,"����",LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(4,"��ɫ��", LVCFMT_LEFT, 200);
	for(auto i(0); i < config_instance.accounts.size(); i++)
	{
		 const auto& account = config_instance.accounts.at(i);
		 m_ListAccount.InsertItem(0, common::IntToCString(i+1));
		 m_ListAccount.SetItemText(i, 1, _T("������"));
		 m_ListAccount.SetItemText(i, 2, common::stringToCString(account.qq));
		 m_ListAccount.SetItemText(i, 3, common::stringToCString(account.password));
		 m_ListAccount.SetItemText(i, 4, common::stringToCString(account.role_name));
	}
}

void CDnfTestDlg::OnBnClickedButtonCreateRole()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, NULL);
}


void CDnfTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, NULL);
}




void CDnfTestDlg::OnEnChangeMfceditbrowseGame()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString Text;
	m_EditGame.GetWindowText(Text);
	if(Text.Compare(common::stringToCString(config_instance.game_path))!=0){
		config_instance.game_path = common::CStringTostring(Text);
		config_instance.SaveData();
	}
}


void CDnfTestDlg::OnEnUpdateMfceditbrowseGame()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// �������Խ� EM_SETEVENTMASK ��Ϣ���͵��ÿؼ���
	// ͬʱ�� ENM_UPDATE ��־�������㵽 lParam �����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
