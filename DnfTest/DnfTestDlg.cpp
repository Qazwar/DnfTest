
// DnfTestDlg.cpp : 实现文件
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


// CDnfTestDlg 对话框




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
	DDX_Control(pDX, IDC_BUTTON_START, m_ButtonStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_ButtonStop);
	DDX_Control(pDX, IDC_EDIT_IP, m_EditIP);
	DDX_Control(pDX, IDC_COMBO_FIRST_ROLE, m_ComboFirstRole);
	DDX_Control(pDX, IDC_COMBO_SECOND_ROLE, m_ComboSecondRole);
	DDX_Control(pDX, IDC_COMBO_FIRST_PROFESSION, m_ComboxFirstProfession);
	DDX_Control(pDX, IDC_COMBO_SECOND_PROFESSION, m_ComboxSecondProfession);
	DDX_Text(pDX, IDC_EDIT_LOCAL_IP, m_EditLocalIP);
	DDX_Control(pDX, IDC_SPIN_RETRY, m_SpinRetry);
	DDX_Text(pDX, IDC_EDIT_RETRY_TIMES, m_EditRetry);
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
	ON_EN_CHANGE(IDC_EDIT_RETRY_TIMES, &CDnfTestDlg::OnEnChangeEditRetryTimes)
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
	this->GetDlgItem(IDC_BUTTON_TEST_PROFESSION)->ShowWindow(SW_HIDE);
	this->GetDlgItem(IDC_BUTTON_TEST_AREA)->ShowWindow(SW_HIDE);
#endif
	
	m_SpinRetry.SetBuddy(GetDlgItem(IDC_EDIT_RETRY_TIMES));
	m_SpinRetry.SetRange(1, 9);
	m_SpinRetry.SetPos(config_instance.loginFailTimes);
	m_gameControl = new CGameControl(this->GetSafeHwnd());
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
	if(common::QueryUser()==0){
		AfxMessageBox("用户未授权");
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
	pThis->m_ButtonStart.EnableWindow(FALSE);
	// TODO: 在此添加控件通知处理程序代码	
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
	m_EditLocalIP = common::stringToCString(config_instance.ip_address);
	this->UpdateData(FALSE);
	m_EditGameDir.SetWindowText(common::stringToCString(config_instance.game_path));
	m_ListAccount.InsertColumn(0,"序列号",LVCFMT_LEFT, 50);
	m_ListAccount.InsertColumn(1,"角色状态", LVCFMT_LEFT, 100);
	m_ListAccount.InsertColumn(2,"qq号码", LVCFMT_LEFT, 200);
	m_ListAccount.InsertColumn(3,"密码",LVCFMT_LEFT, 200);
	for(auto i(0); i < config_instance.accounts.size(); i++)
	{
		 const auto& account = config_instance.accounts.at(i);
		 m_ListAccount.InsertItem(i, common::IntToCString(i+1));
		 m_ListAccount.SetItemText(i, 1, _T("未创建"));
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

void CDnfTestDlg::OnBnClickedButtonCreateRole()
{
	// TODO: 在此添加控件通知处理程序代码
	_beginthread(CDnfTestDlg::StartCreateRole, NULL, this);
}


void CDnfTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	_beginthread(CDnfTestDlg::StartInputCodes, NULL, this);
}




void CDnfTestDlg::OnEnChangeMfceditbrowseGame()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDnfTestDlg::OnEnUpdateMfceditbrowseGame()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDnfTestDlg::OnCbnSelchangeComboArea()
{
	// TODO: 在此添加控件通知处理程序代码
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
		m_EditGameStatus.SetWindowText(_T("游戏启动中"));
	}else if (status == GAME_LOGIN)
	{
		m_EditGameStatus.SetWindowText(_T("游戏登录中"));
	}else if (status == GAME_CREATE_ROLE)
	{
		m_EditGameStatus.SetWindowText(_T("创建角色"));
	}else if (status == GAME_CREATE_ROLE_DONE)
	{
		m_EditGameStatus.SetWindowText(_T("创建角色完成"));
	}else if (status == GAME_ALL_ACCOUNT_DONE)
	{
		m_EditGameStatus.SetWindowText(_T("所有账号创建完成"));
	}else if (status == GAME_STOP)
	{
		m_EditGameStatus.SetWindowText(_T("游戏结束"));
	}else if (status == GAME_IP)
	{
		CString Text = _T("切换ip成功")+*(CString*)lParam;
		m_EditIP.SetWindowText(Text);
	}else if (status == GAME_IP_FAILED)
	{
		CString Text = _T("切换ip失败");
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
		AfxMessageBox("游戏路径不能为空");
		return false;
	}
	if(config_instance.ip_address.size()==0){
		AfxMessageBox("本地ip不能为空");
		return false;
	}
	if(config_instance.areaname.size()==0){
		AfxMessageBox("大区不能为空");
		return false;
	}
	if(config_instance.firstRole.size()==0){
		AfxMessageBox("第一角色不能为空");
		return false;
	}
	if(config_instance.secondRole.size()==0){
		AfxMessageBox("第二角色不能为空");
		return false;
	}
	return true;
}

void CDnfTestDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gameControl->Stop();
}


void CDnfTestDlg::OnCbnSelchangeComboFirstRole()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	SaveUIInfo();
	m_gameControl->SelectProfession();
}


void CDnfTestDlg::OnBnClickedButtonTestArea()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveUIInfo();
	config_instance.SaveData();
	m_gameControl->SelectArea();
}


void CDnfTestDlg::OnEnChangeEditRetryTimes()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	m_EditRetry = m_SpinRetry.GetPos();
}
