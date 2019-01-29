#include "stdafx.h"
#include "KeyMouMng.h"
#include "WaitForEvent.h"
#include "winio.h"
#include "DD.h"

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
	SetCursorPos(OldP.x,OldP.y);
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
			::MessageBox(NULL,_T("����ֻ���� \"a-z\" \"A-Z\" \"0-9\" \"_\" \"+\" \"[\" \"]\" ����������ʹ��"),NULL,MB_OK);
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
#define   KBC_KEY_CMD   0x64         //��������˿� 
#define   KBC_KEY_DATA   0x60       //�������ݶ˿� 


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

void   MyKeyDownEx(long   vKeyCoad)       //ģ����չ�����£�����vKeyCoad����չ���������� 
{ 
	long   btScancode; 
	btScancode   =   MapVirtualKey(vKeyCoad,   0); 

	KBCWait4IBE();       //�ȴ����̻�����Ϊ�� 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1     );       //���ͼ���д������ 
	KBCWait4IBE(); 
	SetPortVal(KBC_KEY_DATA,   0xE0,   1   );   //д����չ����־��Ϣ 


	KBCWait4IBE();       //�ȴ����̻�����Ϊ�� 
	SetPortVal(   KBC_KEY_CMD,   0xD2,   1   );         //���ͼ���д������ 
	KBCWait4IBE(); 
	SetPortVal(   KBC_KEY_DATA,   btScancode,   1   );   //д�밴����Ϣ,���¼� 
} 


void   MyKeyUpEx(long   vKeyCoad)       //ģ����չ������ 
{ 
	long   btScancode; 
	btScancode   =   MapVirtualKey(vKeyCoad,   0); 

	KBCWait4IBE();       //�ȴ����̻�����Ϊ�� 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1   );         //���ͼ���д������ 
	KBCWait4IBE(); 
	SetPortVal   (KBC_KEY_DATA,   0xE0,   1   );   //д����չ����־��Ϣ 


	KBCWait4IBE();     //�ȴ����̻�����Ϊ�� 
	SetPortVal(KBC_KEY_CMD,   0xD2,   1   );         //���ͼ���д������ 
	KBCWait4IBE(); 
	SetPortVal(KBC_KEY_DATA,   (btScancode   |   0x80),   1);     //д�밴����Ϣ���ͷż� 
} 

void   MyKeyDown(long   vKeyCoad) 
{ 
	LOG_DEBUG << "key down  key code "<< vKeyCoad;//���ͼ���д������
	long   byScancode   =   MapVirtualKey(vKeyCoad,0);   
	KBCWait4IBE();//�ȴ����̻�����Ϊ��   
	if(SetPortVal(0x64,0xD2,1)==false)
		LOG_DEBUG << "���ͼ���д������ʧ��! key code "<< vKeyCoad;//���ͼ���д������ 

	LOG_DEBUG << "key down before input code" << vKeyCoad;
	KBCWait4IBE();//�ȴ����̻�����Ϊ��   
	if(SetPortVal(0x60,(ULONG)byScancode,1)==false)
		LOG_DEBUG << "���¼�ʧ��! " << vKeyCoad;//д�밴����Ϣ,���¼� 
	LOG_DEBUG << "after input code" << vKeyCoad;
} 


void   MyKeyUp(long   vKeyCoad) 
{ 
	LOG_DEBUG << "key up  key code "<< vKeyCoad;//���ͼ���д������
	long   byScancode   =   MapVirtualKey(vKeyCoad,0);   
	KBCWait4IBE();//�ȴ����̻�����Ϊ��   
	if(SetPortVal(0x64,0xD2,1)==false)
		LOG_DEBUG << "���ͼ���д������ʧ��! ";//���ͼ���д������ 

	LOG_DEBUG << "key up before input code" << vKeyCoad;
	KBCWait4IBE();//�ȴ����̻�����Ϊ��   
	if(SetPortVal(0x60,(ULONG)(byScancode   |   0x80),1)==false)
		LOG_DEBUG << "�ͷż�ʧ��! ";//д�밴����Ϣ���ͷż� 
	LOG_DEBUG << "key up after input code" << vKeyCoad;
} 

void CKeyMouMng::InputPassword( char* szBuffer)
{
	HWND hLoginWnd = NULL;
	hLoginWnd = GetLoginWnd();
	if (hLoginWnd == NULL) 
		return; 
	DWORD dwThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
	// ��ǰ̨�����߳���������ǰ�̣߳�Ҳ���ǳ���A�еĵ����̣߳�
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
	//	MyKeyDown(szChar); //����A��
	//	Sleep(300 + rand()%100);
	//	MyKeyUp(szChar);
	//	Sleep(300 + rand()%100);
	//}
	dd.DD_str(szBuffer);

}

