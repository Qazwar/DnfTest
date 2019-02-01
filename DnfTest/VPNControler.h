#pragma once
class CVPNControler
{
public:
	CVPNControler(void);
	~CVPNControler(void);
	void clickOnSwitchButton();
	static CString GetSystemIp(void);
	void clickOnDisconnectButton();
private:
	void MouseMoveAndClick(int nX,int nY);
};

