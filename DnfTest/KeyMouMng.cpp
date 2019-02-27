#include "stdafx.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "DD.h"
#include "GameControl.h"

//=============================����ģʽ============================
//����ƶ�
BOOL CKeyMouMng::MouseMove(LONG dx, LONG dy, USHORT Flags)
{
	POINT OldP;
	switch (Flags)
	{
	case MOUSE_MOVE_RELATIVE:
		{
			//����ԭʼ���λ��
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
		//̧��
		::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		//ѹ��
		::mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(200);
		//̧��
		::mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(50);
		return TRUE;
	}
	else if (ButtonFlags == MOUSE_RIGHT_BUTTON)
	{
		//̧��
		::mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		//ѹ��
		::mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		Sleep(200);
		//̧��
		::mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		Sleep(50);
		return TRUE;
	}
	return FALSE;
}

//�����
void CKeyMouMng::MouseLClick()
{
	if(global_instance.getControllerType()==CONTROL_WINDOWS_MESSAGE){
		MouseButtonEx(MOUSE_LEFT_BUTTON);
	}
	else if (global_instance.getControllerType() == DD)
	{
		dd.DD_btn(2);
		Sleep(100);
		dd.DD_btn(1);         // 1���� 2���� 4���� 8����
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
	auto ret = SendInput(1, input, sizeof(INPUT));//����ֵ�ǳɹ�����ĸ���
	if (ret!=1)
	{
		LOG_DEBUG<<"�������뷵��ֵ:"<<ret<<",����: ", GetLastError();
	}
	//�������ͷ�������һ�����Ҫ��
	input[1].ki.dwFlags = KEYEVENTF_KEYUP|KEYEVENTF_SCANCODE;
	Sleep(20 + rand()%150);
	input[1].ki.time = GetTickCount();
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	ret = SendInput(1, input+1, sizeof(INPUT));//����ֵ�ǳɹ�����ĸ���
	if (ret!=1)
	{
		LOG_DEBUG<<"�������뷵��ֵ:"<<ret<<",����: ", GetLastError();
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
			::MessageBox(NULL,_T("����ֻ���� \"a-z\" \"A-Z\" \"0-9\" \"_\" \"+\" \"[\" \"]\" ����������ʹ��"),NULL,MB_OK);
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
	// ��ǰ̨�����߳���������ǰ�̣߳�Ҳ���ǳ���A�еĵ����̣߳�
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
	//����:1.������Сֵ;2.�������ֵ
	srand((unsigned)time(NULL)); //��ʼ�����������
	int impo=mini+rand()%(maxi-mini+1);

	return impo;
}

void CKeyMouMng::Contra(int dual_X,int dual_Y)
{
	//������1.Ŀ�ĵ�X���� 2.Ŀ�ĵ�Y����

	double goverX=0,goverY=0; //�켣X,Y
	double factuX=1,factuY=1; //��ǰ�������
	double problem_X=double(dual_X),problem_Y=double(dual_Y); //Ŀ�ĵ�����
	double used=0;
	int Shah=More(16,25); //�ƶ�����
	int own=1; //�ƶ���ʱ
	int ShahX=0,ShahY=0; //�ƶ�ƫ��X,�ƶ�ƫ��Y

	POINT Paid; //����һ���ṹ�����굱ǰX Y����
	GetCursorPos(&Paid); //��ȡ��굱ǰλ��

	factuX=double(Paid.x);//�ѵ�ǰ�������ת����˫������
	factuY=double(Paid.y);
	if (abs(factuX-problem_X)>abs(factuY-problem_Y))
	{
		used=abs(factuX-problem_X);
	}
	else
	{
		used=abs(factuY-problem_Y);
	}
	goverX=abs((problem_X-factuX))/used*Shah;  //�켣X
	goverY=abs((problem_Y-factuY))/used*Shah;  //�켣Y

	while(true)
	{
		if(factuX<=dual_X){
			factuX=factuX+goverX; //��ǰX����+�켣X
		}else{
			factuX=factuX-goverX; //��ǰX����+�켣X
		}
		if(factuY<=dual_Y){
			factuY=factuY+goverY; //��ǰY����+�켣Y
		}else{
			factuY=factuY-goverX;
		}
		own=More(10,25); //�ƶ���ʱ
		//ÿ����5�ξ��������һ�����ƫ��켣��ģ���ֶ��ƶ��켣
		ShahX=More(-Shah*4,Shah*4); //�ƶ�ƫ��X
		ShahY=More(-Shah*4,Shah*4); //�ƶ�ƫ��Y
		
		auto current_X = int(factuX)+ShahX;
		auto current_Y = int(factuY)+ShahY;
		MouseMoveTo(current_X,current_Y); //�����ƶ�����APIҲ��������������
		Sleep(own); //����ӳ�
		if (abs(dual_X-current_X)<=abs(ShahX) && abs(dual_Y-current_Y)<=abs(ShahY))
		{
			MouseMoveTo(dual_X,dual_Y);//�ƶ����յ㸽����ֱ���ƶ����յ�
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

