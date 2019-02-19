#include "StdAfx.h"
#include "VPNControler.h"
#include "KeyMouMng.h"
#include <afxinet.h>


CVPNControler::CVPNControler(void)
{
}


CVPNControler::~CVPNControler(void)
{
}

void CVPNControler::clickOnSwitchButton()
{
	MouseMoveAndClick(115, 570);
}

CString CVPNControler::GetSystemIp(void)
{
	try
	{
		CString csSource;
		CString csAddress;
		CString csIPAddress;
		CInternetSession mySession("iptest",0);
		CHttpFile* myHttpFile=NULL;
		myHttpFile=(CHttpFile*)mySession.OpenURL("http://2019.ip138.com/ic.asp", 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD, NULL, 0);//��ȡ�����ַ
		while(myHttpFile->ReadString(csSource))
		{  		
			int begin=0;
			begin=csSource.Find(_T("["),0);
			if(begin!=-1)//����ҵ�"["�� ����"]"  �������ڵ��ı����� �������ip
			{
				int end=csSource.Find(_T("]"));
				csIPAddress.Format(_T("%s"),csSource.Mid(begin+1,end-begin-1));//��ȡ����ip
				return csIPAddress;
			}
		}
	}
	catch (CException* e)
	{
	}
	return _T("");
}

void CVPNControler::clickOnDisconnectButton()
{
	MouseMoveAndClick(293, 570);
}

void CVPNControler::MouseMoveAndClick(int nX,int nY)
{
	RECT LoginWndRect;
	HWND hLoginWnd = NULL;
	hLoginWnd = GetVPNWnd();
	if (hLoginWnd == NULL) 
		return;
	if (hLoginWnd)
	{
		::GetWindowRect(hLoginWnd, &LoginWndRect);
		CKeyMouMng::Ptr()->MouseMove(LoginWndRect.left+nX,LoginWndRect.top+nY);
	}
	CKeyMouMng::Ptr()->MouseButtonEx(MOUSE_LEFT_BUTTON);
}
