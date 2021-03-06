#pragma  once
enum AccountStatus {
	STATUS_INIT = 0,
	STATUS_LOGIN_FAILED = 1,//登录失败
	STATUS_SUCCESS = 2,//成功创建角色的账号
	STATUS_EXCEPTION = 3//游戏异常账号
};

enum GameStatus{
	GAME_INIT = 0,
	GAME_START,//游戏开始
	GAME_LOGIN,//游戏登录
	GAME_CREATE_ROLE,//创建角色
	GAME_CREATE_ROLE_DONE,//创建角色完成
	GAME_ALL_ACCOUNT_DONE,//账户都已经创建角色
	GAME_STOP,//停止游戏
	GAME_IP,//显示ip地址
	GAME_IP_FAILED//切换IP失败
};
const string ServerUrl = "https://dnf.utomarket.com:9055";
//const string ServerUrl = "http://127.0.0.1:9091";
//控制类型
enum ControllerType{
	CONTROL_WINDOWS_MESSAGE = 1,//windows消息
	DD = 2,//dd控件
};

#define WM_UPDATE_GAME_STATUS (WM_USER+1)
#define WM_DOWNLOAD (WM_USER+2)
