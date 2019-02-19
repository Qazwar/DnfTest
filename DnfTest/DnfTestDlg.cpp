
// DnfTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DnfTest.h"
#include "DnfTestDlg.h"
#include "afxdialogex.h"
#include "GameControl.h"
#include "Config.h"
#include "Global.h"
#include "DialogRegister.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDnfTestDlg �Ի���




CDnfTestDlg::CDnfTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDnfTestDlg::IDD, pParent), m_gameControl(NULL)
	, m_EditRetry(0)
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
	DDX_Control(pDX, IDC_EDIT_IP, m_EditIP);
	DDX_Control(pDX, IDC_COMBO_FIRST_ROLE, m_ComboFirstRole);
	DDX_Control(pDX, IDC_COMBO_SECOND_ROLE, m_ComboSecondRole);
	DDX_Control(pDX, IDC_COMBO_FIRST_PROFESSION, m_ComboxFirstProfession);
	DDX_Control(pDX, IDC_COMBO_SECOND_PROFESSION, m_ComboxSecondProfession);
	DDX_Text(pDX, IDC_EDIT_LOCAL_IP, m_EditLocalIP);
	DDX_Control(pDX, IDC_SPIN_RETRY, m_SpinRetry);
	DDX_Text(pDX, IDC_EDIT_RETRY_TIMES, m_EditRetry);
	DDX_Control(pDX, IDC_MFCBUTTON_START, buttonStart);
	DDX_Control(pDX, IDC_MFCBUTTON_STOP, buttonStop);
}

BEGIN_MESSAGE_MAP(CDnfTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnChangeMfceditbrowseGame)
	ON_EN_UPDATE(IDC_MFCEDITBROWSE_GAME, &CDnfTestDlg::OnEnUpdateMfceditbrowseGame)
	ON_CBN_SELCHANGE(IDC_COMBO_AREA, &CDnfTestDlg::OnCbnSelchangeComboArea)
	ON_MESSAGE(WM_UPDATE_GAME_STATUS, &CDnfTestDlg::OnUpdateGameStatus)
	ON_CBN_SELCHANGE(IDC_COMBO_FIRST_ROLE, &CDnfTestDlg::OnCbnSelchangeComboFirstRole)
	ON_CBN_SELCHANGE(IDC_COMBO_SECOND_ROLE, &CDnfTestDlg::OnCbnSelchangeComboSecondRole)
	ON_EN_CHANGE(IDC_EDIT_RETRY_TIMES, &CDnfTestDlg::OnEnChangeEditRetryTimes)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MFCBUTTON_START, &CDnfTestDlg::OnBnClickedMfcbuttonStart)
	ON_BN_CLICKED(IDC_MFCBUTTON_STOP, &CDnfTestDlg::OnBnClickedMfcbuttonStop)
	ON_BN_CLICKED(IDC_MFCBUTTON_CREATE_ROLE, &CDnfTestDlg::OnBnClickedMfcbuttonCreateRole)
	ON_BN_CLICKED(IDC_MFCBUTTON_INPUT_PASSWORD, &CDnfTestDlg::OnBnClickedMfcbuttonInputPassword)
	ON_BN_CLICKED(IDC_MFCBUTTON_TEST_PROFESSION, &CDnfTestDlg::OnBnClickedMfcbuttonTestProfession)
	ON_BN_CLICKED(IDC_MFCBUTTON_TEST_AREA, &CDnfTestDlg::OnBnClickedMfcbuttonTestArea)
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
		PostQuitMessage(0);
	}
	this->ShowWindow(SW_SHOW);
#ifndef DEBUG
	this->GetDlgItem(IDC_MFCBUTTON_CREATE_ROLE)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_MFCBUTTON_INPUT_PASSWORD)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_MFCBUTTON_TEST_PROFESSION)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_MFCBUTTON_TEST_AREA)->ShowWindow(SW_HIDE);
#endif	
	m_SpinRetry.SetBuddy(GetDlgItem(IDC_EDIT_RETRY_TIMES));
	m_SpinRetry.SetRange(1, 9);
	m_SpinRetry.SetPos(config_instance.loginFailTimes);
	m_gameControl = new CGameControl(this->GetSafeHwnd());
	m_brush.CreateSolidBrush(global_instance.getBackgroundColor()); 
	buttonStart.SetFaceColor(global_instance.getButtonColor());
	buttonStop.SetFaceColor(global_instance.getButtonColor());
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_CREATE_ROLE))->SetFaceColor(global_instance.getButtonColor());
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_INPUT_PASSWORD))->SetFaceColor(global_instance.getButtonColor());
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_TEST_PROFESSION))->SetFaceColor(global_instance.getButtonColor());
	((CMFCButton*)GetDlgItem(IDC_MFCBUTTON_TEST_AREA))->SetFaceColor(global_instance.getButtonColor());
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
		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect, global_instance.getBackgroundColor());
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDnfTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDnfTestDlg::OnBnClickedMfcbuttonStart()
{
	if(common::QueryUser()==0){
		AfxMessageBox("�û�δ��Ȩ");
		return;
	}
	if(!SaveUIInfo()){
		return;
	}
	_beginthread(CDnfTestDlg::StartProcess, NULL, this);
}

void CDnfTestDlg::StartProcess(void* param)
{
	CDnfTestDlg*pThis = (CDnfTestDlg*)param;
	pThis->buttonStart.EnableWindow(FALSE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	while(pThis->m_gameControl->GameProcess()){
	}
	pThis->onGameStatusChange(GAME_ALL_ACCOUNT_DONE);
	pThis->buttonStart.EnableWindow(TRUE);
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
	m_EditLocalIP = common::stringToCString(config_instance.ip_address);
	this->UpdateData(FALSE);
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
	for (int i(0); i < config_instance.game_area.size(); i++)
	{
		m_ComboArea.AddString(common::stringToCString(config_instance.game_area.at(i).name));
	}
	m_ComboArea.SetCurSel(m_ComboArea.FindStringExact(0, common::stringToCString(config_instance.areaname)));
	OnCbnSelchangeComboArea();
	m_ComboServer.SetCurSel(m_ComboServer.FindStringExact(0, common::stringToCString(config_instance.servername)));

	auto firstRoleArray = common::SplitString(config_instance.first_role, ';');
	for (auto i(0); i < firstRoleArray->GetSize(); i++)
	{
		m_ComboFirstRole.AddString(firstRoleArray->GetAt(i));
	}
	m_ComboFirstRole.SetCurSel(m_ComboFirstRole.FindStringExact(0, common::stringToCString(config_instance.firstRole)));
	OnCbnSelchangeComboFirstRole();
	if(config_instance.firstRoleProfession.size()!=0){
		m_ComboxFirstProfession.SetCurSel(m_ComboxFirstProfession.FindStringExact(0, common::stringToCString(config_instance.firstRoleProfession)));
	}

	delete firstRoleArray;
	firstRoleArray = nullptr;
	
	auto secondRoleArray = common::SplitString(config_instance.second_role, ';');
	for (auto i(0); i < secondRoleArray->GetSize(); i++)
	{
		m_ComboSecondRole.AddString(secondRoleArray->GetAt(i));
	}
	m_ComboSecondRole.SetCurSel(m_ComboSecondRole.FindStringExact(0, common::stringToCString(config_instance.secondRole)));
	OnCbnSelchangeComboSecondRole();
	if(config_instance.secondRoleProfession.size()!=0){
		m_ComboxSecondProfession.SetCurSel(m_ComboxSecondProfession.FindStringExact(0, common::stringToCString(config_instance.secondRoleProfession)));
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

void CDnfTestDlg::OnEnChangeMfceditbrowseGame()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	const auto & Index = m_ComboArea.GetCurSel();
	if(config_instance.game_area.size()<=Index){
		return;
	}
	const auto & list_server = config_instance.game_area.at(Index).server;
	m_ComboServer.ResetContent();
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


bool CDnfTestDlg::SaveUIInfo()
{
	CString csGamePath, csFirstRole, csFistRoleProfession, csSecondRole, csSecondRoleProfession, csServerName, csAreaName;
	m_EditGameDir.GetWindowText(csGamePath);
	m_ComboFirstRole.GetWindowText(csFirstRole);
	m_ComboxFirstProfession.GetWindowText(csFistRoleProfession);
	m_ComboSecondRole.GetWindowText(csSecondRole);
	m_ComboxSecondProfession.GetWindowText(csSecondRoleProfession);
	m_ComboServer.GetWindowText(csServerName);
	m_ComboArea.GetWindowText(csAreaName);

	config_instance.game_path = common::CStringTostring(csGamePath);
	config_instance.firstRole = common::CStringTostring(csFirstRole);
	config_instance.firstRoleProfession = common::CStringTostring(csFistRoleProfession);
	config_instance.secondRole = common::CStringTostring(csSecondRole);
	config_instance.secondRoleProfession = common::CStringTostring(csSecondRoleProfession);
	config_instance.servername = common::CStringTostring(csServerName);
	config_instance.areaname = common::CStringTostring(csAreaName);
	config_instance.loginFailTimes = m_EditRetry;
	config_instance.ip_address = m_EditLocalIP;
	if(config_instance.game_path.size()==0){
		AfxMessageBox("��Ϸ·������Ϊ��");
		return false;
	}
	if(config_instance.ip_address.size()==0){
		AfxMessageBox("����ip����Ϊ��");
		return false;
	}
	if(config_instance.areaname.size()==0){
		AfxMessageBox("��������Ϊ��");
		return false;
	}
	if(config_instance.firstRole.size()==0){
		AfxMessageBox("��һ��ɫ����Ϊ��");
		return false;
	}
	if(config_instance.secondRole.size()==0){
		AfxMessageBox("�ڶ���ɫ����Ϊ��");
		return false;
	}
	return true;
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


void CDnfTestDlg::OnEnChangeEditRetryTimes()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_EditRetry = m_SpinRetry.GetPos();
}


HBRUSH CDnfTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//�����һ���Ƿ�Ϊ�Ի�����ж����
	if(nCtlColor ==CTLCOLOR_STATIC)
	{
		pDC-> SetBkMode(TRANSPARENT); //�������屳��Ϊ͸��
		pDC-> SetBkColor(global_instance.getBackgroundColor()); //���屳��ɫ
		return (HBRUSH)m_brush.GetSafeHandle();
	}
	return hbr;
}



void CDnfTestDlg::OnBnClickedMfcbuttonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_gameControl->Stop();
}


void CDnfTestDlg::OnBnClickedMfcbuttonCreateRole()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, this);
}


void CDnfTestDlg::OnBnClickedMfcbuttonInputPassword()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, this);
}


void CDnfTestDlg::OnBnClickedMfcbuttonTestProfession()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveUIInfo();
	m_gameControl->SelectProfession();
}


void CDnfTestDlg::OnBnClickedMfcbuttonTestArea()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SaveUIInfo();
	config_instance.SaveData();
	m_gameControl->SelectArea();
}
