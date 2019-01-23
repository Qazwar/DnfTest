//-------------------------------------------------------------------------
// 文件: DumpAndSend.h
// 时间: 2005-12-13 21:51
// 作者: 游戏行业很多兄弟奋斗的积累
// 名称: 用于打印堆栈并自动发送错误报告
// 版本:
// [v1.00]2005-12-13 21:51:初始版本
// [v1.01]2005-12-21 10:13:添加初始化配置设置对应的项目名称和人物名称
// [v1.02]2005-12-23 15:01:添加IsBinBuf，统计判断是否为二进制缓冲区
// [v1.03]2006-01-04 20:23:修改了WriteModules，可以完全支持UNICODE
// [v1.04]2006-01-06 16:48:修改了GetSendToDirCb，去掉了路径上的日期和时间
// [v1.05]2006-01-07 19:10:添加了GetFixedFileInfo可以获得固定版本信息
// [v1.06]2006-02-24 17:22:将GetFixedFileInfo更改为GetFileInfo
//                         更改了模块时间不正确的BUG
// [v1.07]2006-03-01 11:15:添加可以初始化服务器地址的设定
//                         从这个版本开始错误报告将以静态库发布
// [v1.08]2006-03-04 11:11:WriteModules增加一个参数，是否使用FullMemoryDump
// [v1.09]2006-03-07 16:31:Ftp文件发送是用被动发送
// [v1.10]2006-03-09 10:48:更改了WriteModules使其不用WriteMiniDump也能使用
//-------------------------------------------------------------------------
#pragma once
#ifndef DumpAndSend_h_
#define DumpAndSend_h_

#define GETINTSTR2(i) #i
#define GETINTSTR(i) GETINTSTR2(i)
#define SHOWMSG(text) message(__FILE__"("GETINTSTR(__LINE__)")"" : "#text)

namespace ErrorReport {

//-------------------------------------------------------------------------
// 函数: HRESULT GetSystemNameCb( OUT LPTSTR pszDest , IN size_t cbDest, 
//					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining  )
// 说明: 获得当前操作系统的名称
// 参数: pszDest				接收名称的字符串
//		 cbDest					字符缓冲区大小(字节数)
//		 ppszDestEnd			返回当前字符串的结尾(不需要可以为NULL)
//		 pcbRemaining			获得字符串的剩余长度(不需要可以为NULL)
// 返回: 字符串的长度
//-------------------------------------------------------------------------
HRESULT GetSystemNameCb( OUT LPTSTR pszDest, IN size_t cbDest, 
					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining );

//-------------------------------------------------------------------------
// 函数: HRESULT GetModuleNameCb( OUT LPTSTR pszDest, IN size_t cbDest,
//					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining )
// 说明: 获得所在模块的名字
// 参数: pszDest				获得文件名称
//		 cbDest					pszDest缓冲区的大小(字节数)
//		 ppszDestEnd			返回当前字符串的结尾(不需要可以为NULL)
//		 pcbRemaining			获得字符串的剩余长度(不需要可以为NULL)
// 返回: 成功代码
//-------------------------------------------------------------------------
HRESULT GetModuleNameCb( OUT LPTSTR pszDest, IN size_t cbDest,
					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining );

typedef struct VS_FILEINFO
{
	VS_FIXEDFILEINFO vsFixed;
	union {
		struct {
			WORD wLanguage;
			WORD wCodePage;
		};
		DWORD dwLangCode;
	};
	TCHAR szInternalName[256];
	TCHAR szProductName[256];
    TCHAR szComments[256];
} VS_FILEINFO;

//-------------------------------------------------------------------------
// 函数: BOOL GetFileInfo( IN LPCTSTR szFileName, IN OUT VS_FILEINFO * lpvsFixedFileInfo )
// 说明: 获得对应名称的程序模块的版本信息
// 参数: szFileName				获得程序模块的版本信息的文件名称
//		 lpvsFileInfo			获得版本信息的数据区指针
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL GetFileInfo( IN LPCTSTR szFileName, IN OUT VS_FILEINFO * lpvsFileInfo );

//-------------------------------------------------------------------------
// 函数: LPCTSTR GetFileNamePart( IN LPCTSTR lpszFileName )
// 说明: 获得文件名部分
// 参数: lpszFileName			源文件名
// 返回: 文件名部分
//-------------------------------------------------------------------------
LPCTSTR GetFileNamePart( IN LPCTSTR lpszFileName );

//-------------------------------------------------------------------------
// 函数: HRESULT StringAddCharCb( IN OUT LPTSTR pszDest, IN size_t cbDest,
//					IN TCHAR tAddChar, OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining)
// 说明: 在目标字符串头部写入字符并截断
// 参数: pszDest				添加的目标
//		 cbDest					目标的大小(字节数)
//		 tAddChar				添加的字符串
//		 ppszDestEnd			返回当前字符串的结尾(不需要可以为NULL)
//		 pcbRemaining			获得字符串的剩余长度(不需要可以为NULL)
// 返回: 是否成功
//-------------------------------------------------------------------------
HRESULT StringAddCharCb( IN OUT LPTSTR pszDest, IN size_t cbDest, IN TCHAR tAddChar, 
						OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining);

//-------------------------------------------------------------------------
// 函数: BOOL GetLastWriteFileTime( IN LPCTSTR pFileName, OUT FILETIME * pftFileTime )
// 说明: 获得一个文件的最后写入时间
// 参数: pFileName				检查的文件名
//		 pftFileTime			文件时间
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL GetLastWriteFileTime( IN LPCTSTR pFileName, OUT FILETIME * pftFileTime );

//-------------------------------------------------------------------------
// 函数: void WriteMiniDump( LPCTSTR szDumpFileName, PEXCEPTION_POINTERS pException,
//							IN BOOL bNeedFullMemory )
// 说明: 向一个文件输出内存转储
// 参数: szDumpFileName			转储文件名
//		 pException				异常指针
//		 bNeedFullMemory		需要全内存Dump，这将导致很大的Dump文件
// 返回: 是否成功 可以通过GetLastError获得失败原因
//-------------------------------------------------------------------------
BOOL WriteMiniDump( IN LPCTSTR szDumpFileName, IN PEXCEPTION_POINTERS pException,
				   IN BOOL bNeedFullMemory );

//-------------------------------------------------------------------------
// 函数: BOOL WriteModules( IN LPCTSTR szFileName )
// 说明: 向异常管理器输出所有模块信息
// 参数: szFileName				写入模块表的文件名
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL WriteModules( IN LPCTSTR szFileName );

//-------------------------------------------------------------------------
// 函数: BOOL SetCharacterName( IN LPCTSTR szCharacterName )
// 说明: 设置发送到文件夹路经上的人物名称，如果没有设置则没有这个路径层次
//		 这个函数提供了宽字符和多字节字符两种函数
// 参数: szCharacterName		人物名称
// 返回: 是否设置成功
//-------------------------------------------------------------------------
BOOL SetCharacterNameA( IN LPCSTR szCharacterName );
BOOL SetCharacterNameW( IN LPCWSTR szCharacterName );
#ifdef UNICODE
#define SetCharacterName SetCharacterNameW
#else//UNICODE
#define SetCharacterName SetCharacterNameA
#endif//UNICODE

//-------------------------------------------------------------------------
// 函数: BOOL SetCharacterID( IN DWORD dwCharacterID )
// 说明: 设置发送到文件夹路经上的人物ID，如果为0则使用IP地址
// 参数: dwCharacterID			人物ID
// 返回: 是否设置成功
//-------------------------------------------------------------------------
BOOL SetCharacterID( IN DWORD dwCharacterID );

//-------------------------------------------------------------------------
// 函数: BOOL SetProjectName( IN LPCTSTR szProjectName )
// 说明: 设置发送到文件夹路经上的项目名称，如果没有设置则使用出错模块名
// 参数: szProjectName			项目名称
// 返回: 是否设置成功
//-------------------------------------------------------------------------
BOOL SetProjectName( IN LPCTSTR szProjectName );

//-------------------------------------------------------------------------
// 函数: BOOL GetSendToDirCb( IN OUT LPTSTR pszDest, IN size_t cbDest )
// 说明: 产生发送到的FTP路径，这个路径在程序运行期间不会改变
// 参数: pszDest				目标字串
//		 cbDest					目标的大小(字节数)
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL GetSendToDirCb( IN OUT LPTSTR pszDest, IN size_t cbDest );

//-------------------------------------------------------------------------
// 函数: void SetSendToFtpServer( IN LPCTSTR lpszServerName, IN WORD wHostPort,
//						IN LPCTSTR lpszUserName, IN LPCTSTR lpszPassWord )
// 说明: 设置发送到服务器地址，这应该在所有的SendFile操作之前
// 参数: lpszServerName			服务器地址
//		 wHostPort				服务器端口
//		 lpszUserName			用户名
//		 lpszPassWord			密码
// 返回: 无
//-------------------------------------------------------------------------
void SetSendToFtpServer( IN LPCTSTR lpszServerName, IN WORD wHostPort,
						IN LPCTSTR lpszUserName, IN LPCTSTR lpszPassWord );

//-------------------------------------------------------------------------
// 函数: BOOL SendFile( IN LPCTSTR szFileName )
// 说明: 发送文件到服务器
// 参数: szFileName				要发送的文件名
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL SendFile( IN LPCTSTR szFileName );

//-------------------------------------------------------------------------
// 函数: BOOL GetHostInfoCb( OUT LPDWORD outIP, OUT LPTSTR outName, IN size_t cbName )
// 说明: 获得当前的主机信息
// 参数: outIP					返回本机的IP(不需要可以为NULL)
//		 outName				返回机器名(不需要可以为NULL)
//		 cbName					获得机器名字符串的大小(字节数)(不需要可以为NULL)
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL GetHostInfoCb( OUT LPDWORD outIP, OUT LPTSTR outName, IN size_t cbName );

//-------------------------------------------------------------------------
// 函数: BOOL DumpAndSend( IN PEXCEPTION_POINTERS pException )
// 说明: 生成DUMP文件和模块文件，并自动发送的集成函数
// 参数: pException				异常指针
// 返回: 是否成功
//-------------------------------------------------------------------------
BOOL DumpAndSend( IN PEXCEPTION_POINTERS pException );

//-------------------------------------------------------------------------
// 函数: BOOL IsBinBuf( IN LPBYTE lpBuf, IN DWORD dwNumberCharsToCheck, 
//						IN DWORD dwMaxIllegalCharacters);
// 说明: 判断一个缓冲区中的数据是否为二进制数据
// 参数: lpBuf					缓冲区
//		 dwNumberCharsToCheck	检查的字符数
//		 dwMinMatchChar			最少有几个二进制字符就当作二进制缓冲
//								(为0则检查dwNumberCharsToCheck的5%)
// 返回: 是否为二进制数据缓冲区
//-------------------------------------------------------------------------
BOOL IsBinBuf( IN LPBYTE lpBuf, IN DWORD dwNumberCharsToCheck, 
				IN DWORD dwMinMatchChar );

};

#endif//DumpAndSend_h_
