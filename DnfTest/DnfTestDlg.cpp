
// DnfTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DnfTestDlg.h"
#include "afxdialogex.h"
#include "GameControl.h"
#include "Config.h"
#include "Global.h"
#include "HttpClient.h"
#include "DialogRegister.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDnfTestDlg �Ի���




CDnfTestDlg::CDnfTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDnfTestDlg::IDD, pParent), m_gameControl(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDnfTestDlg::~CDnfTestDlg()
{
	if(m_gameControl){
		delete m_gameControl;
		m_gameControl = NULL;
	}
}

void CDnfTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_ListAccount);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_GAME, m_EditGameDir);
	DDX_Control(pDX, IDC_COMBO_RISK_GROUP, m_ComboRiskGroup);
	DDX_Control(pDX, IDC_COMBO_ROLE_NAME, m_ComboRoleName);
	DDX_Control(pDX, IDC_COMBO_AREA, m_ComboArea);
	DDX_Control(pDX, IDC_COMBO_SERVER, m_ComboServer);
	DDX_Control(pDX, IDC_EDIT_GAME_STATUS, m_EditGameStatus);
	DDX_Control(pDX, IDC_BUTTON_START, m_ButtonStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_ButtonStop);
	DDX_Control(pDX, IDC_EDIT_IP, m_EditIP);
	DDX_Control(pDX, IDC_COMBO_FIRST_ROLE, m_ComboFirstRole);
	DDX_Control(pDX, IDC_COMBO_SECOND_ROLE, m_ComboSecondRole);
	DDX_Control(pDX, IDC_COMBO_FIRST_PROFESSION, m_ComboxFirstProfession);
	DDX_Control(pDX, IDC_COMBO_SECOND_PROFESSION, m_ComboxSecondProfession);
}

BEGIN_MESSAGE_MAP(CDnfTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDnfTestDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ROLE, &CDnfTestDlg::OnBnClickedButtonCreateRole)
	ON_BN_CLICKED(IDC_BUTTON1, &CDnfTestDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnChangeMfceditbrowseGame)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnUpdateMfceditbrowseGame)
	ON_CBN_SELCHANGE(IDC_COMBO_AREA, &CDnfTestDlg::OnCbnSelchangeComboArea)
	ON_MESSAGE(WM_UPDATE_GAME_STATUS, &CDnfTestDlg::OnUpdateGameStatus)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDnfTestDlg::OnBnClickedButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST_ROLE, &CDnfTestDlg::OnCbnSelchangeComboFirstRole)
	ON_CBN_SELCHANGE(IDC_COMBO_SECOND_ROLE, &CDnfTestDlg::OnCbnSelchangeComboSecondRole)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PROFESSION, &CDnfTestDlg::OnBnClickedButtonTestProfession)
	ON_BN_CLICKED(IDC_BUTTON_TEST_AREA, &CDnfTestDlg::OnBnClickedButtonTestArea)
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
	InitData();
	if(common::QueryUser()==0){
		this->ShowWindow(SW_HIDE);
		CDialogRegister dlg;
		dlg.DoModal();
	}
	this->ShowWindow(SW_SHOW);
#ifndef DEBUG
	this->GetDlgItem(IDC_BUTTON_CREATE_ROLE)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
#endif
	
	m_gameControl = new CGameControl(this->GetSafeHwnd());
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
	if(common::QueryUser()==0){
		AfxMessageBox("�û�δ��Ȩ");
		return;
	}
	SaveUIInfo();
	_beginthread(CDnfTestDlg::StartProcess, NULL, this);
}

void CDnfTestDlg::StartProcess(void* param)
{
	CDnfTestDlg*pThis = (CDnfTestDlg*)param;
	pThis->m_ButtonStart.EnableWindow(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	while(pThis->m_gameControl->GameProcess()){
	}
	pThis->onGameStatusChange(GAME_ALL_ACCOUNT_DONE);
	pThis->m_ButtonStart.EnableWindow(TRUE);
}


void CDnfTestDlg::StartInputCodes(void*param)
{
	CDnfTestDlg*pThis = (CDnfTestDlg*)param;
	pThis->m_gameControl->InputCodes();
}

void CDnfTestDlg::StartCreateRole(void*param)
{
	CDnfTestDlg*pThis = (CDnfTestDlg*)param;
	pThis->m_gameControl->CreateRole();
	pThis->m_gameControl->EndGame();
}

void CDnfTestDlg::InitData()
{
	m_EditGameDir.SetWindowText(common::stringToCString(config_instance.game_path));
	m_ListAccount.InsertColumn(0,"���к�",LVCFMT_LEFT, 50);
	m_ListAccount.InsertColumn(1,"��ɫ״̬", LVCFMT_LEFT, 100);
	m_ListAccount.InsertColumn(2,"qq����", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(3,"����",LVCFMT_LEFT, 200);
	for(auto i(0); i < config_instance.accounts.size(); i++)
	{
		 const auto& account = config_instance.accounts.at(i);
		 m_ListAccount.InsertItem(i, common::IntToCString(i+1));
		 m_ListAccount.SetItemText(i, 1, _T("δ����"));
		 m_ListAccount.SetItemText(i, 2, common::stringToCString(account.qq));
		 m_ListAccount.SetItemText(i, 3, common::stringToCString(account.password));
	}
	auto roleNameArray = common::SplitString(config_instance.role_name_type, ';');
	for (auto i(0); i < roleNameArray->GetSize(); i++)
	{
		m_ComboRoleName.AddString(roleNameArray->GetAt(i));
	}
	if(roleNameArray->GetSize()>0){
		m_ComboRoleName.SetCurSel(0);
	}
	delete roleNameArray;
	roleNameArray = nullptr;

	auto riskGroupArray = common::SplitString(config_instance.role_name_type, ';');
	for (auto i(0); i < riskGroupArray->GetSize(); i++)
	{
		m_ComboRiskGroup.AddString(riskGroupArray->GetAt(i));
	}
	if(riskGroupArray->GetSize()>0){
		m_ComboRiskGroup.SetCurSel(0);
	}
	delete riskGroupArray;
	riskGroupArray = nullptr;
	m_ComboArea.AddString(_T("��ѡ�����"));
	for (int i(0); i < config_instance.game_area.size(); i++)
	{
		m_ComboArea.AddString(common::stringToCString(config_instance.game_area.at(i).name));
	}
	m_ComboArea.SetCurSel(0);

	auto firstRoleArray = common::SplitString(config_instance.first_role, ';');
	for (auto i(0); i < firstRoleArray->GetSize(); i++)
	{
		m_ComboFirstRole.AddString(firstRoleArray->GetAt(i));
	}
	if(firstRoleArray->GetSize()>0){
		m_ComboFirstRole.SetCurSel(0);
		OnCbnSelchangeComboFirstRole();
	}
	delete firstRoleArray;
	firstRoleArray = nullptr;

	auto secondRoleArray = common::SplitString(config_instance.second_role, ';');
	for (auto i(0); i < secondRoleArray->GetSize(); i++)
	{
		m_ComboSecondRole.AddString(secondRoleArray->GetAt(i));
	}
	if(secondRoleArray->GetSize()>0){
		m_ComboSecondRole.SetCurSel(0);
		OnCbnSelchangeComboSecondRole();
	}
	delete secondRoleArray;
	secondRoleArray = nullptr;
}

LRESULT CDnfTestDlg::OnUpdateGameStatus(WPARAM wParam, LPARAM lParam)
{
	GameStatus status = (GameStatus)wParam;
	onGameStatusChange(status, lParam);
	return 0;
}

void CDnfTestDlg::OnBnClickedButtonCreateRole()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, this);
}


void CDnfTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, this);
}




void CDnfTestDlg::OnEnChangeMfceditbrowseGame()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString Text;
	m_EditGameDir.GetWindowText(Text);
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


void CDnfTestDlg::OnCbnSelchangeComboArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const auto & Index = m_ComboArea.GetCurSel() - 1;
	const auto & list_server = config_instance.game_area.at(Index).server;
	m_ComboServer.ResetContent();
	m_ComboServer.AddString(_T("��ѡ��"));
	for (int i(0); i < list_server.size(); i++)
	{
		m_ComboServer.AddString(common::stringToCString(list_server.at(i)));
	}
	if(m_ComboServer.GetCount()>0){
		m_ComboServer.SetCurSel(0);
	}
}

void CDnfTestDlg::onGameStatusChange(const GameStatus& status, LPARAM lParam)
{
	if (status == GAME_START)
	{
		m_EditGameStatus.SetWindowText(_T("��Ϸ������"));
	}else if (status == GAME_LOGIN)
	{
		m_EditGameStatus.SetWindowText(_T("��Ϸ��¼��"));
	}else if (status == GAME_CREATE_ROLE)
	{
		m_EditGameStatus.SetWindowText(_T("������ɫ"));
	}else if (status == GAME_CREATE_ROLE_DONE)
	{
		m_EditGameStatus.SetWindowText(_T("������ɫ���"));
	}else if (status == GAME_ALL_ACCOUNT_DONE)
	{
		m_EditGameStatus.SetWindowText(_T("�����˺Ŵ������"));
	}else if (status == GAME_STOP)
	{
		m_EditGameStatus.SetWindowText(_T("��Ϸ����"));
	}else if (status == GAME_IP)
	{
		CString Text = _T("�л�ip�ɹ�")+*(CString*)lParam;
		m_EditIP.SetWindowText(Text);
	}else if (status == GAME_IP_FAILED)
	{
		CString Text = _T("�л�ipʧ��");
		m_EditIP.SetWindowText(Text);
	}
}


void CDnfTestDlg::SaveUIInfo()
{
	m_ComboFirstRole.GetWindowText(global_instance.firstRole);
	m_ComboxFirstProfession.GetWindowText(global_instance.firstRoleProfession);
	m_ComboSecondRole.GetWindowText(global_instance.secondRole);
	m_ComboxSecondProfession.GetWindowText(global_instance.secondRoleProfession);
	m_ComboServer.GetWindowText(global_instance.servername);
	m_ComboArea.GetWindowText(global_instance.areaname);
}

void CDnfTestDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_gameControl->Stop();
}


void CDnfTestDlg::OnCbnSelchangeComboFirstRole()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csText;
	m_ComboFirstRole.GetWindowText(csText);
	auto Text = common::CStringTostring(csText);
	const auto & list_profession = config_instance.professions;
	m_ComboxFirstProfession.ResetContent();
	for (int i(0); i < list_profession.size(); i++)
	{
		if(list_profession.at(i).name == Text){
			for (int j(0); j < list_profession.at(i).profession.size(); j++){
				m_ComboxFirstProfession.AddString(common::stringToCString(list_profession.at(i).profession.at(j)));
			}
			break;
		}
	}
	if(m_ComboxFirstProfession.GetCount()>0){
		m_ComboxFirstProfession.SetCurSel(0);
	}
}


void CDnfTestDlg::OnCbnSelchangeComboSecondRole()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csText;
	m_ComboSecondRole.GetWindowText(csText);
	auto Text = common::CStringTostring(csText);
	const auto & list_profession = config_instance.professions;
	m_ComboxSecondProfession.ResetContent();
	for (int i(0); i < list_profession.size(); i++)
	{
		if(list_profession.at(i).name == Text){
			for (int j(0); j < list_profession.at(i).profession.size(); j++){
				m_ComboxSecondProfession.AddString(common::stringToCString(list_profession.at(i).profession.at(j)));
			}
			break;
		}
	}
	if(m_ComboxSecondProfession.GetCount()>0){
		m_ComboxSecondProfession.SetCurSel(0);
	}
}


void CDnfTestDlg::OnBnClickedButtonTestProfession()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveUIInfo();
	m_gameControl->SelectProfession();
}


void CDnfTestDlg::OnBnClickedButtonTestArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveUIInfo();
	m_gameControl->SelectArea();
}
