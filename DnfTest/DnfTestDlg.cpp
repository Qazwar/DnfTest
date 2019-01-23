
// DnfTestDlg.cpp : ʵ���ļ�
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


// CDnfTestDlg �Ի���




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
	InitializeWinIo();
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
	//	StartupInfo.cb = sizeof(STARTUPINFOA);

	// 	char strFilePath1[MAX_PATH] = "D:\\�������µ�\\TCLS\\Client.exe";              //�½���ִ�г���
	// 
	// 
	// 	PROCESS_INFORMATION pt;
	// 	STARTUPINFO si;                                       //PC������������ӣ��ᵼ�´������̳���wce���ã�ֱ��ΪNULL
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	ZeroMemory(&ProcessInformation, sizeof(ProcessInformation));

	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;//ָ��wShowWindow��Ա��Ч
	StartupInfo.wShowWindow = TRUE;//�˳�Ա��ΪTRUE�Ļ�����ʾ�½����̵�������
	CreateProcessA("C:\\Program Files\\��Ѷ��Ϸ\\���³�����ʿ\\TCLS\\Client.exe",
	NULL,
	NULL, NULL,
	0,
	NULL,
	NULL,
	"C:\\Program Files\\��Ѷ��Ϸ\\���³�����ʿ\\TCLS\\",
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, NULL);
}


void CDnfTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, NULL);
}


void CDnfTestDlg::OnBnClickedButtonCreateRoles()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CGameControl gameControl;
	gameControl.CreateRole();
}
