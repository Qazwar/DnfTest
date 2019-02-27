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
	Contra(OldP.x, OldP.y);
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
	if(global_instance.getControllerType()==CONTROL_WINDOWS_MESSAGE){
		MouseButtonEx(MOUSE_LEFT_BUTTON);
	}
	else if (global_instance.getControllerType() == DD)
	{
		dd.DD_btn(2);
		Sleep(100);
		dd.DD_btn(1);         // 1左下 2左上 4右下 8右上
		Sleep(200);
		dd.DD_btn(2);
		Sleep(50);
		LOG_DEBUG<<"dd button click ";
	}
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

void CKeyMouMng::InputString(const string& buffer)
{
	if(global_instance.getControllerType()== CONTROL_WINDOWS_MESSAGE){
	for (int i = 0;i < buffer.size();i++ )
	{
		WAIT_STOP_RETURN(100);
		if ( buffer.at(i) >= 'a' && buffer.at(i) <= 'z')
		{
			KeyboardButtonEx(buffer.at(i)-32);
		}
		else if( buffer.at(i) >= 'A' && buffer.at(i) <= 'Z')
		{
			KeyboardButtonEx(buffer.at(i),VK_SHIFT);
		}
		else if ( buffer.at(i) >= '0' && buffer.at(i) <= '9')
		{
			KeyboardButtonEx(buffer.at(i));
		}
		else if ( buffer.at(i) == '_')
		{
			KeyboardButtonEx(VK_OEM_MINUS,VK_SHIFT);
		}		
		else if ( buffer.at(i) == '+')
		{
			KeyboardButtonEx(VK_OEM_PLUS,VK_SHIFT);
		}
		else if ( buffer.at(i) == '[')
		{
			KeyboardButtonEx(VK_OEM_4);
		}
		else if ( buffer.at(i) == ']')
		{
			KeyboardButtonEx(VK_OEM_6);
		}
		else
		{
			::MessageBox(NULL,_T("密码只能是 \"a-z\" \"A-Z\" \"0-9\" \"_\" \"+\" \"[\" \"]\" 其他不可以使用"),NULL,MB_OK);
		}
	}
	}else{
		InputByDD(buffer);
	}
}

void CKeyMouMng::MouseMoveAndClickGameWnd(int nX,int nY)
{
	RECT LoginWndRect;
	HWND hGameWnd = NULL;
	hGameWnd = GetGameWnd();
	if (hGameWnd == NULL) 
		return;
	if (hGameWnd)
	{
		::GetWindowRect(hGameWnd, &LoginWndRect);
		MouseMove(LoginWndRect.left+nX,LoginWndRect.top+nY);
	}
	MouseLClick();
}

void CKeyMouMng::InputByDD(const string& buffer)
{
	/*HWND hLoginWnd = NULL;
	hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
	return; */
	//DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	// 将前台窗口线程贴附到当前线程（也就是程序A中的调用线程）
	//AttachThreadInput(dwThread, GetCurrentThreadId(), TRUE);
	for(auto i(0); i < buffer.size(); i++)
	{
		char szChar = buffer.at(i);
		int code = 0;
		if ( buffer.at(i) >= 'a' && buffer.at(i) <= 'z')
		{
			code = getKeyCode(szChar);
		}
		else if ( buffer.at(i) >= '0' && buffer.at(i) <= '9')
		{
			code = 800 + szChar - '0';
		}
		dd.DD_key(code, 1);
		Sleep(300 + rand()%100);
		dd.DD_key(code, 2);
		Sleep(300 + rand()%100);
	}
	LOG_DEBUG<<" input by dd"<<buffer.c_str();
}

int CKeyMouMng::getKeyCode(const char & ch)
{
	auto ret(0);
	if(mapKeyCode.find(ch)!=mapKeyCode.end()){
		ret = mapKeyCode[ch];
	}
	return ret;
}

int CKeyMouMng::More(int mini,int maxi)
{
	//参数:1.区间最小值;2.区间最大值
	srand((unsigned)time(NULL)); //初始化随机数种子
	int impo=mini+rand()%(maxi-mini+1);

	return impo;
}

void CKeyMouMng::Contra(int dual_X,int dual_Y)
{
	//参数：1.目的地X坐标 2.目的地Y坐标

	double goverX=0,goverY=0; //轨迹X,Y
	double factuX=1,factuY=1; //当前鼠标坐标
	double problem_X=double(dual_X),problem_Y=double(dual_Y); //目的地坐标
	double used=0;
	int Shah=More(16,25); //移动步长
	int own=1; //移动延时
	int ShahX=0,ShahY=0; //移动偏差X,移动偏差Y

	POINT Paid; //声明一个结构存放鼠标当前X Y坐标
	GetCursorPos(&Paid); //获取鼠标当前位置

	factuX=double(Paid.x);//把当前鼠标坐标转换成双浮点数
	factuY=double(Paid.y);
	if (abs(factuX-problem_X)>abs(factuY-problem_Y))
	{
		used=abs(factuX-problem_X);
	}
	else
	{
		used=abs(factuY-problem_Y);
	}
	goverX=abs((problem_X-factuX))/used*Shah;  //轨迹X
	goverY=abs((problem_Y-factuY))/used*Shah;  //轨迹Y

	while(true)
	{
		if(factuX<=dual_X){
			factuX=factuX+goverX; //当前X坐标+轨迹X
		}else{
			factuX=factuX-goverX; //当前X坐标+轨迹X
		}
		if(factuY<=dual_Y){
			factuY=factuY+goverY; //当前Y坐标+轨迹Y
		}else{
			factuY=factuY-goverX;
		}
		own=More(10,25); //移动延时
		//每运行5次就随机产生一个随机偏差轨迹，模拟手动移动轨迹
		ShahX=More(-Shah*4,Shah*4); //移动偏差X
		ShahY=More(-Shah*4,Shah*4); //移动偏差Y
		
		auto current_X = int(factuX)+ShahX;
		auto current_Y = int(factuY)+ShahY;
		MouseMoveTo(current_X,current_Y); //任意移动鼠标的API也可以是驱动键盘
		Sleep(own); //随机延迟
		if (abs(dual_X-current_X)<=abs(ShahX) && abs(dual_Y-current_Y)<=abs(ShahY))
		{
			MouseMoveTo(dual_X,dual_Y);//移动到终点附近后直接移动到终点
			break;
		}
	}
}

void CKeyMouMng::MouseMoveTo(int dual_X,int dual_Y)
{
	if(global_instance.getControllerType()==CONTROL_WINDOWS_MESSAGE){
		SetCursorPos(dual_X, dual_Y);
	}else if (global_instance.getControllerType()== DD)
	{
		LOG_DEBUG<<"dd mouse move";
		dd.DD_mov(dual_X, dual_Y);
	}
}

