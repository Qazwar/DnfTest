
// DnfTestDlg.h : ͷ�ļ�
//

#pragma once


// CDnfTestDlg �Ի���
class CDnfTestDlg : public CDialogEx
{
// ����
public:
	CDnfTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DNFTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
