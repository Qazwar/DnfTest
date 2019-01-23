
// DnfTestDlg.h : 头文件
//

#pragma once


// CDnfTestDlg 对话框
class CDnfTestDlg : public CDialogEx
{
// 构造
public:
	CDnfTestDlg(CWnd* pParent = NULL);	// 标准构造函数

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
public:
	afx_msg void OnBnClickedButtonStart();
private:
	static void StartProcess(void*);
	static void StartInputCodes(void*);
	static void StartCreateRole(void*);
public:
	afx_msg void OnBnClickedButtonCreateRole();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonCreateRoles();
};
