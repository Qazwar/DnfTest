
// DnfTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "afxbutton.h"

class CGameControl;
// CDnfTestDlg 对话框
class CDnfTestDlg : public CDialogEx
{
// 构造
public:
	CDnfTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CDnfTestDlg();
// 对话框数据
	enum { IDD = IDD_DNFTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	static void StartProcess(void* param);
	static void StartInputCodes(void*param);
	static void StartCreateRole(void*param);
	void InitData();
	LRESULT OnUpdateGameStatus(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_ListAccount;
	CMFCEditBrowseCtrl m_EditGameDir;
	afx_msg void OnEnChangeMfceditbrowseGame();
	afx_msg void OnEnUpdateMfceditbrowseGame();
	CComboBox m_ComboRiskGroup;
	CComboBox m_ComboRoleName;
	CComboBox m_ComboArea;
	CComboBox m_ComboServer;
	afx_msg void OnCbnSelchangeComboArea();
	CEdit m_EditGameStatus;
private:
	CGameControl *m_gameControl;
protected:
	CBrush m_brush; 
	void onGameStatusChange(const GameStatus& status, LPARAM lParam = NULL);
	bool SaveUIInfo();//保存ui信息，用于创建角色
public:
	CEdit m_EditIP;
	CComboBox m_ComboFirstRole;
	CComboBox m_ComboSecondRole;
	afx_msg void OnCbnSelchangeComboFirstRole();
	afx_msg void OnCbnSelchangeComboSecondRole();
	CComboBox m_ComboxFirstProfession;
	CComboBox m_ComboxSecondProfession;
	CString m_EditLocalIP;
	CSpinButtonCtrl m_SpinRetry;
	afx_msg void OnEnChangeEditRetryTimes();
	int m_EditRetry;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CMFCButton buttonStart;
	afx_msg void OnBnClickedMfcbuttonStart();
	CMFCButton buttonStop;
	afx_msg void OnBnClickedMfcbuttonStop();
	afx_msg void OnBnClickedMfcbuttonCreateRole();
	afx_msg void OnBnClickedMfcbuttonInputPassword();
	afx_msg void OnBnClickedMfcbuttonTestProfession();
	afx_msg void OnBnClickedMfcbuttonTestArea();
};
