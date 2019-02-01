#include "stdafx.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "DD.h"
#include "GameControl.h"

//=============================按键模式============================
//鼠标移动
BOOL CKeyMouMng::MouseMove(LONG dx, LONG dy, USHORT Flags)
{
	POINT OldP;
	switch (Flags)
	{
	case MOUSE_MOVE_RELATIVE:
		{
			//保存原始鼠标位置
			GetCursorPos(&OldP);
			OldP.x += dx;
			OldP.y += dy;
		}
		break;
	case MOUSE_MOVE_ABSOLUTE:
		{
			OldP.x = dx;
			OldP.y = dy;
		}
		break;
	default:
		return FALSE;
	}
	SetCursorPos(OldP.x,OldP.y);
	return TRUE;
}

BOOL CKeyMouMng::MouseButtonEx(USHORT ButtonFlags)
{
	if (ButtonFlags == MOUSE_LEFT_BUTTON)
	{
		//抬起
		::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		//压下
		::mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(200);
		//抬起
		::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(50);
		return TRUE;
	}
	else if (ButtonFlags == MOUSE_RIGHT_BUTTON)
	{
		//抬起
		::mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		//压下
		::mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		Sleep(200);
		//抬起
		::mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		Sleep(50);
		return TRUE;
	}
	return FALSE;
}

//鼠标点击
void CKeyMouMng::MouseLClick()
{
	MouseButtonEx(MOUSE_LEFT_BUTTON);
}

void CKeyMouMng::MouseMoveInLoginWnd(int nX,int nY)
{
	RECT LoginWndRect;
	HWND hLoginWnd = NULL;
	hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
		return;
	if (hLoginWnd)
	{
		::GetWindowRect(hLoginWnd, &LoginWndRect);
		MouseMove(LoginWndRect.left+nX,LoginWndRect.top+nY);
	}
}

void CKeyMouMng::MouseMoveAndClick(int nX,int nY)
{
	MouseMoveInLoginWnd(nX,nY);
	MouseLClick();
}

BOOL CKeyMouMng::KeyboardButton(BYTE VirtualKey, BYTE Flags)
{
	::keybd_event(VirtualKey, ::MapVirtualKey(VirtualKey,0), Flags, 0);
	return TRUE;
}

BOOL CKeyMouMng::KeyboardButtonEx(BYTE VirtualKey, int ControlVirtualKey)
{
	INPUT input[2];
	memset(input, 0, sizeof(input));

	input[0].type = input[1].type = INPUT_KEYBOARD;
	input[0].ki.wVk  = input[1].ki.wVk = VirtualKey;
	input[0].ki.wScan = input[1].ki.wScan = ::MapVirtualKeyEx(VirtualKey,MAPVK_VK_TO_VSC, GetKeyboardLayout(0));
	input[0].ki.time = GetTickCount();
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();
	auto ret = SendInput(1, input, sizeof(INPUT));//返回值是成功插入的个数
	if (ret!=1)
	{
		LOG_DEBUG<<"输入密码返回值:"<<ret<<",错误: ", GetLastError();
	}
	//接下来释放它，这一点很重要。
	input[1].ki.dwFlags = KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
	Sleep(20 + rand()%150);
	input[1].ki.time = GetTickCount();
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	ret = SendInput(1, input+1, sizeof(INPUT));//返回值是成功插入的个数
	if (ret!=1)
	{
		LOG_DEBUG<<"输入密码返回值:"<<ret<<",错误: ", GetLastError();
	}

	return TRUE;
}

void CKeyMouMng::DirKeyDown(BYTE key)
{
	KeyboardButton(key, DIR_KEY_DOWN);
}

void CKeyMouMng::DirKeyUp(BYTE key)
{
	KeyboardButton(key, DIR_KEY_UP);
}

void CKeyMouMng::InputCharByKeyBoard(const char* szBuffer)
{
	for (int i = 0;i < lstrlen(szBuffer);i++ )
	{
		WAIT_STOP_RETURN(100);
		if ( szBuffer[i] >= 'a' && szBuffer[i] <= 'z')
		{
			KeyboardButtonEx(szBuffer[i]-32);
		}
		else if( szBuffer[i] >= 'A' && szBuffer[i] <= 'Z')
		{
			KeyboardButtonEx(szBuffer[i],VK_SHIFT);
		}
		else if ( szBuffer[i] >= '0' && szBuffer[i] <= '9')
		{
			KeyboardButtonEx(szBuffer[i]);
		}
		else if ( szBuffer[i] == '_')
		{
			KeyboardButtonEx(VK_OEM_MINUS,VK_SHIFT);
		}		
		else if ( szBuffer[i] == '+')
		{
			KeyboardButtonEx(VK_OEM_PLUS,VK_SHIFT);
		}
		else if ( szBuffer[i] == '[')
		{
			KeyboardButtonEx(VK_OEM_4);
		}
		else if ( szBuffer[i] == ']')
		{
			KeyboardButtonEx(VK_OEM_6);
		}
		else
		{
			::MessageBox(NULL,_T("密码只能是 \"a-z\" \"A-Z\" \"0-9\" \"_\" \"+\" \"[\" \"]\" 其他不可以使用"),NULL,MB_OK);
		}
	}
}

void CKeyMouMng::MouseMoveAndClickGameWnd(int nX,int nY)
{
	RECT LoginWndRect;
	HWND hLoginWnd = NULL;
	hLoginWnd = GetGameWnd();
	if (hLoginWnd == NULL) 
		return;
	if (hLoginWnd)
	{
		::GetWindowRect(hLoginWnd, &LoginWndRect);
		MouseMove(LoginWndRect.left+nX,LoginWndRect.top+nY);
	}
	MouseLClick();
	MouseLClick();
}

void CKeyMouMng::InputPassword( char* szBuffer)
{
	HWND hLoginWnd = NULL;
	hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
		return; 
	//DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	// 将前台窗口线程贴附到当前线程（也就是程序A中的调用线程）
	//AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
	for(auto i(0); i < strlen(szBuffer); i++)
	{
		char szChar = szBuffer[i];
		int code = 0;
		if ( szBuffer[i] >= 'a' && szBuffer[i] <= 'z')
		{
			code = getKeyCode(szChar);
		}
		else if ( szBuffer[i] >= '0' && szBuffer[i] <= '9')
		{
			code = 800 + szChar - '0';
		}
		dd.DD_key(code, 1);
		Sleep(300 + rand()%100);
		dd.DD_key(code, 2);
		Sleep(300 + rand()%100);
	}
	LOG_DEBUG<<" input password "<<szBuffer;
}

int CKeyMouMng::getKeyCode(const char & ch)
{
	auto ret(0);
	if(mapKeyCode.find(ch)!=mapKeyCode.end()){
		ret = mapKeyCode[ch];
	}
	return ret;
}

