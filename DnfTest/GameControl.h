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
	CGameControl(void);
	~CGameControl(void);
	void ClickLoginInArea();
	void InputCodes();
	void CreateRole();
	void EndGame();
	void SetAccountIndex(const int& index);
private:
	bool IsCanCreateRoles();
	void ClickAgreement();
	bool IsCanLogin();
	BOOL ImageMatchFromHwnd(HWND hWnd,const TCHAR* ImagePath,float fSame,
		OUT int& nX,OUT int& nY,bool bSave);
	IplImage* HBitmapToLpl(HBITMAP hBmp);
private:
	int	m_Index;
};

