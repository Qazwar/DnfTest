#pragma once

#include "singleton.h"
#include "DD.h"

//
// Define the keyboard input data Flags.
//

#define KEY_MAKE  0
#define KEY_BREAK 1
#define KEY_E0    1
#define KEY_E1    3
#define KEY_TERMSRV_SET_LED 8
#define KEY_TERMSRV_SHADOW  0x10
#define KEY_TERMSRV_VKPACKET 0x20

#define KEY_DOWN                0
#define KEY_UP                  KEYEVENTF_KEYUP
#define KEY_BLANK                -1

#define DIR_KEY_DOWN			KEY_E0
#define DIR_KEY_UP				KEY_E1

#define DK_LEFT					75
#define DK_RIGHT				77
#define DK_UP					72
#define DK_DOWN					80

//
// Define the mouse button state indicators.
//
#define MOUSE_LEFT_BUTTON        0x0001
#define MOUSE_RIGHT_BUTTON       0x0002
#define MOUSE_LEFT_BUTTON_DOWN   0x0001  // Left Button changed to down.
#define MOUSE_LEFT_BUTTON_UP     0x0002  // Left Button changed to up.
#define MOUSE_RIGHT_BUTTON_DOWN  0x0004  // Right Button changed to down.
#define MOUSE_RIGHT_BUTTON_UP    0x0008  // Right Button changed to up.
#define MOUSE_MIDDLE_BUTTON_DOWN 0x0010  // Middle Button changed to down.
#define MOUSE_MIDDLE_BUTTON_UP   0x0020  // Middle Button changed to up.

#define MOUSE_BUTTON_1_DOWN     MOUSE_LEFT_BUTTON_DOWN
#define MOUSE_BUTTON_1_UP       MOUSE_LEFT_BUTTON_UP
#define MOUSE_BUTTON_2_DOWN     MOUSE_RIGHT_BUTTON_DOWN
#define MOUSE_BUTTON_2_UP       MOUSE_RIGHT_BUTTON_UP
#define MOUSE_BUTTON_3_DOWN     MOUSE_MIDDLE_BUTTON_DOWN
#define MOUSE_BUTTON_3_UP       MOUSE_MIDDLE_BUTTON_UP

#define MOUSE_BUTTON_4_DOWN     0x0040
#define MOUSE_BUTTON_4_UP       0x0080
#define MOUSE_BUTTON_5_DOWN     0x0100
#define MOUSE_BUTTON_5_UP       0x0200

#define MOUSE_WHEEL             0x0400

//
// Define the mouse indicator flags.
//
#define MOUSE_MOVE_RELATIVE         0
#define MOUSE_MOVE_ABSOLUTE         1
#define MOUSE_VIRTUAL_DESKTOP    0x02  // the coordinates are mapped to the virtual desktop
#define MOUSE_ATTRIBUTES_CHANGED 0x04  // requery for mouse attributes

class CKeyMouMng : public Pattern::Singleton<CKeyMouMng>
{
public:
	CKeyMouMng(void){
		dd.GetFunAddr(_T("DD85590.32.dll"));
		mapKeyCode['a'] = 401;
		mapKeyCode['s'] = 402;
		mapKeyCode['d'] = 403;
		mapKeyCode['f'] = 404;
		mapKeyCode['g'] = 405;
		mapKeyCode['h'] = 406;
		mapKeyCode['j'] = 407;
		mapKeyCode['k'] = 408;
		mapKeyCode['l'] = 409;
		mapKeyCode['q'] = 301;
		mapKeyCode['w'] = 302;
		mapKeyCode['e'] = 303;
		mapKeyCode['r'] = 304;
		mapKeyCode['t'] = 305;
		mapKeyCode['y'] = 306;
		mapKeyCode['u'] = 307;
		mapKeyCode['i'] = 308;
		mapKeyCode['o'] = 309;
		mapKeyCode['p'] = 310;
		mapKeyCode['z'] = 501;
		mapKeyCode['x'] = 502;
		mapKeyCode['c'] = 503;
		mapKeyCode['v'] = 504;
		mapKeyCode['b'] = 505;
		mapKeyCode['n'] = 506;
		mapKeyCode['m'] = 507;

	};
	~CKeyMouMng(){};
public:
	static BOOL MouseMove(LONG dx, LONG dy, USHORT Flags = MOUSE_MOVE_ABSOLUTE);
	static BOOL MouseButtonEx(USHORT ButtonFlags);
	void MouseLClick();
	BOOL KeyboardButton(BYTE VirtualKey, BYTE Flags);
	void DirKeyDown(BYTE key);
	void DirKeyUp(BYTE key);
	BOOL KeyboardButtonEx(BYTE VirtualKey, int ControlVirtualKey = KEY_BLANK);

	void MouseMoveAndClick(int nX,int nY);
	void MouseMoveInLoginWnd(int nX,int nY);
	void InputCharByKeyBoard(const char* szBuffer);
	void MouseMoveAndClickGameWnd(int nX,int nY);
	void InputPassword(char* szBuffer);
	int getKeyCode(const char & ch);
public:
	CDD dd ;  // DD ¿‡ 
private:
	map<char, int> mapKeyCode;
};