#pragma  once
enum RoleStatus {
	STATUS_INIT = 0,
	STATUS_SUCCESS = 1,
	STATUS_FAILED = 2,
};

enum GameStatus{
	GAME_INIT = 0,
	GAME_START,//��Ϸ��ʼ
	GAME_LOGIN,//��Ϸ��¼
	GAME_CREATE_ROLE,//������ɫ
	GAME_CREATE_ROLE_DONE,//������ɫ���
	GAME_ALL_ACCOUNT_DONE//�˻����Ѿ�������ɫ
};
#define WM_UPDATE_GAME_STATUS (WM_USER+1)
