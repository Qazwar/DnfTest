#pragma  once
enum AccountStatus {
	STATUS_INIT = 0,
	STATUS_LOGIN_FAILED = 1,//��¼ʧ��
	STATUS_SUCCESS = 2,//�ɹ�������ɫ���˺�
	STATUS_EXCEPTION = 3//��Ϸ�쳣�˺�
};

enum GameStatus{
	GAME_INIT = 0,
	GAME_START,//��Ϸ��ʼ
	GAME_LOGIN,//��Ϸ��¼
	GAME_CREATE_ROLE,//������ɫ
	GAME_CREATE_ROLE_DONE,//������ɫ���
	GAME_ALL_ACCOUNT_DONE,//�˻����Ѿ�������ɫ
	GAME_STOP,//ֹͣ��Ϸ
	GAME_IP,//��ʾip��ַ
	GAME_IP_FAILED//�л�IPʧ��
};
const string ServerUrl = "https://dev.utomarket.com:9055";
//��������
enum ControllerType{
	CONTROL_WINDOWS_MESSAGE = 1,//windows��Ϣ
	DD = 2,//dd�ؼ�
};

#define WM_UPDATE_GAME_STATUS (WM_USER+1)
