#include "stdafx.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "winio.h"
#include "DD.h"

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
#define   KBC_KEY_CMD   0x64         //键盘命令端口 
#define   KBC_KEY_DATA   0x60       //键盘数据端口 


void   KBCWait4IBE() 
{ 
	DWORD   dwRegVal=0; 
	do 
	{   
		bool flag = GetPortVal(KBC_KEY_CMD,&dwRegVal,1);
		if(!flag){
			LOG_DEBUG<<"GetPortVal failed "<< dwRegVal;
		}
		Sleep(100);
	} 
	while(dwRegVal & 0x00000001); 
} 

void   MyKeyDownEx(long   vKeyCoad)       //模拟扩展键按下，参数vKeyCoad是扩展键的虚拟码 
{ 
	long   btScancode; 
	btScancode   =   MapVirtualKey(vKeyCoad,   0); 

	KBCWait4IBE();       //等待键盘缓冲区为空 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1     );       //发送键盘写入命令 
	KBCWait4IBE(); 
	SetPortVal(KBC_KEY_DATA,   0xE0,   1   );   //写入扩展键标志信息 


	KBCWait4IBE();       //等待键盘缓冲区为空 
	SetPortVal(   KBC_KEY_CMD,   0xD2,   1   );         //发送键盘写入命令 
	KBCWait4IBE(); 
	SetPortVal(   KBC_KEY_DATA,   btScancode,   1   );   //写入按键信息,按下键 
} 


void   MyKeyUpEx(long   vKeyCoad)       //模拟扩展键弹起 
{ 
	long   btScancode; 
	btScancode   =   MapVirtualKey(vKeyCoad,   0); 

	KBCWait4IBE();       //等待键盘缓冲区为空 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1   );         //发送键盘写入命令 
	KBCWait4IBE(); 
	SetPortVal   (KBC_KEY_DATA,   0xE0,   1   );   //写入扩展键标志信息 


	KBCWait4IBE();     //等待键盘缓冲区为空 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1   );         //发送键盘写入命令 
	KBCWait4IBE(); 
	SetPortVal(KBC_KEY_DATA,   (btScancode   |   0x80),   1);     //写入按键信息，释放键 
} 

void   MyKeyDown(long   vKeyCoad) 
{ 
	LOG_DEBUG << "key down  key code "<< vKeyCoad;//发送键盘写入命令
	long   byScancode   =   MapVirtualKey(vKeyCoad,0);   
	KBCWait4IBE();//等待键盘缓冲区为空   
	if(SetPortVal(0x64,0xD2,1)==false)
		LOG_DEBUG << "发送键盘写入命令失败! key code "<< vKeyCoad;//发送键盘写入命令 

	LOG_DEBUG << "key down before input code" << vKeyCoad;
	KBCWait4IBE();//等待键盘缓冲区为空   
	if(SetPortVal(0x60,(ULONG)byScancode,1)==false)
		LOG_DEBUG << "按下键失败! " << vKeyCoad;//写入按键信息,按下键 
	LOG_DEBUG << "after input code" << vKeyCoad;
} 


void   MyKeyUp(long   vKeyCoad) 
{ 
	LOG_DEBUG << "key up  key code "<< vKeyCoad;//发送键盘写入命令
	long   byScancode   =   MapVirtualKey(vKeyCoad,0);   
	KBCWait4IBE();//等待键盘缓冲区为空   
	if(SetPortVal(0x64,0xD2,1)==false)
		LOG_DEBUG << "发送键盘写入命令失败! ";//发送键盘写入命令 

	LOG_DEBUG << "key up before input code" << vKeyCoad;
	KBCWait4IBE();//等待键盘缓冲区为空   
	if(SetPortVal(0x60,(ULONG)(byScancode   |   0x80),1)==false)
		LOG_DEBUG << "释放键失败! ";//写入按键信息，释放键 
	LOG_DEBUG << "key up after input code" << vKeyCoad;
} 

void CKeyMouMng::InputPassword( char* szBuffer)
{
	HWND hLoginWnd = NULL;
	hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
		return; 
	DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	// 将前台窗口线程贴附到当前线程（也就是程序A中的调用线程）
	AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
	
	//HWND hFocus = GetFocus();
	//for(auto i(0); i < strlen(szBuffer); i++)
	//{
	//	char szChar = szBuffer[i];
	//	if ( szBuffer[i] >= 'a' && szBuffer[i] <= 'z')
	//	{
	//		szChar -= 'a';
	//		szChar += 0x41;
	//	}
	//	else if ( szBuffer[i] >= '0' && szBuffer[i] <= '1')
	//	{
	//		szChar -= '0';
	//		szChar += 0x30;
	//	}
	//	MyKeyDown(szChar); //按下A键
	//	Sleep(300 + rand()%100);
	//	MyKeyUp(szChar);
	//	Sleep(300 + rand()%100);
	//}
	dd.DD_str(szBuffer);

}

