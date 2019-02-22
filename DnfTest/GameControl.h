#pragma once
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
class account_info;
using namespace std;  
using namespace cv;

class CGameControl
{
public:
	CGameControl(HWND hShow);
	~CGameControl(void);
	void SelectAreaProcess();
	bool StartGame();
	bool InputCodes();
	bool CreateRole();
	void Stop();
	void SelectProfession();
	void SelectArea();
	void GameLoop();
	void EndGame();
private:
	void outputFile();
	void resetIndex();
	void setAccountIndex(const int& index);
	bool findCurrentAccountIndex();
	bool gameProcess();
	void clickAgreement();
	void inputAccount();
	void inputPasswordAndLogin();
	bool createOneRole();
	BOOL saveVerificationCodeImage();
	BOOL imageMatchFromHwnd(HWND hWnd,const TCHAR* ImagePath,float fSame,
		OUT int& nX,OUT int& nY,bool bSave, bool bGray = true /*是否用灰度图比较*/);
	IplImage* hBitmapToLpl(HBITMAP hBmp);
	BOOL findImageInGameWnd(const string& image, float fSame = 0.7, bool bGray = true);
	BOOL findImageInLoginWnd(const string& image);
	string createName(const unsigned int & count);
	BOOL killProcess(const string& processName);
	bool switchVPN();
private:
	HWND m_hShow;
	int	m_Index;
	bool m_Stop;
	int m_RoleIndex;
	CString m_LastIP;
};

