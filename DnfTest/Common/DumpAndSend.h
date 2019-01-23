//-------------------------------------------------------------------------
// �ļ�: DumpAndSend.h
// ʱ��: 2005-12-13 21:51
// ����: ��Ϸ��ҵ�ܶ��ֵܷܶ��Ļ���
// ����: ���ڴ�ӡ��ջ���Զ����ʹ��󱨸�
// �汾:
// [v1.00]2005-12-13 21:51:��ʼ�汾
// [v1.01]2005-12-21 10:13:��ӳ�ʼ���������ö�Ӧ����Ŀ���ƺ���������
// [v1.02]2005-12-23 15:01:���IsBinBuf��ͳ���ж��Ƿ�Ϊ�����ƻ�����
// [v1.03]2006-01-04 20:23:�޸���WriteModules��������ȫ֧��UNICODE
// [v1.04]2006-01-06 16:48:�޸���GetSendToDirCb��ȥ����·���ϵ����ں�ʱ��
// [v1.05]2006-01-07 19:10:�����GetFixedFileInfo���Ի�ù̶��汾��Ϣ
// [v1.06]2006-02-24 17:22:��GetFixedFileInfo����ΪGetFileInfo
//                         ������ģ��ʱ�䲻��ȷ��BUG
// [v1.07]2006-03-01 11:15:��ӿ��Գ�ʼ����������ַ���趨
//                         ������汾��ʼ���󱨸潫�Ծ�̬�ⷢ��
// [v1.08]2006-03-04 11:11:WriteModules����һ���������Ƿ�ʹ��FullMemoryDump
// [v1.09]2006-03-07 16:31:Ftp�ļ��������ñ�������
// [v1.10]2006-03-09 10:48:������WriteModulesʹ�䲻��WriteMiniDumpҲ��ʹ��
//-------------------------------------------------------------------------
#pragma once
#ifndef DumpAndSend_h_
#define DumpAndSend_h_

#define GETINTSTR2(i) #i
#define GETINTSTR(i) GETINTSTR2(i)
#define SHOWMSG(text) message(__FILE__"("GETINTSTR(__LINE__)")"" : "#text)

namespace ErrorReport {

//-------------------------------------------------------------------------
// ����: HRESULT GetSystemNameCb( OUT LPTSTR pszDest , IN size_t cbDest, 
//					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining  )
// ˵��: ��õ�ǰ����ϵͳ������
// ����: pszDest				�������Ƶ��ַ���
//		 cbDest					�ַ���������С(�ֽ���)
//		 ppszDestEnd			���ص�ǰ�ַ����Ľ�β(����Ҫ����ΪNULL)
//		 pcbRemaining			����ַ�����ʣ�೤��(����Ҫ����ΪNULL)
// ����: �ַ����ĳ���
//-------------------------------------------------------------------------
HRESULT GetSystemNameCb( OUT LPTSTR pszDest, IN size_t cbDest, 
					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining );

//-------------------------------------------------------------------------
// ����: HRESULT GetModuleNameCb( OUT LPTSTR pszDest, IN size_t cbDest,
//					OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining )
// ˵��: �������ģ�������
// ����: pszDest				����ļ�����
//		 cbDest					pszDest�������Ĵ�С(�ֽ���)
//		 ppszDestEnd			���ص�ǰ�ַ����Ľ�β(����Ҫ����ΪNULL)
//		 pcbRemaining			����ַ�����ʣ�೤��(����Ҫ����ΪNULL)
// ����: �ɹ�����
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
// ����: BOOL GetFileInfo( IN LPCTSTR szFileName, IN OUT VS_FILEINFO * lpvsFixedFileInfo )
// ˵��: ��ö�Ӧ���Ƶĳ���ģ��İ汾��Ϣ
// ����: szFileName				��ó���ģ��İ汾��Ϣ���ļ�����
//		 lpvsFileInfo			��ð汾��Ϣ��������ָ��
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL GetFileInfo( IN LPCTSTR szFileName, IN OUT VS_FILEINFO * lpvsFileInfo );

//-------------------------------------------------------------------------
// ����: LPCTSTR GetFileNamePart( IN LPCTSTR lpszFileName )
// ˵��: ����ļ�������
// ����: lpszFileName			Դ�ļ���
// ����: �ļ�������
//-------------------------------------------------------------------------
LPCTSTR GetFileNamePart( IN LPCTSTR lpszFileName );

//-------------------------------------------------------------------------
// ����: HRESULT StringAddCharCb( IN OUT LPTSTR pszDest, IN size_t cbDest,
//					IN TCHAR tAddChar, OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining)
// ˵��: ��Ŀ���ַ���ͷ��д���ַ����ض�
// ����: pszDest				��ӵ�Ŀ��
//		 cbDest					Ŀ��Ĵ�С(�ֽ���)
//		 tAddChar				��ӵ��ַ���
//		 ppszDestEnd			���ص�ǰ�ַ����Ľ�β(����Ҫ����ΪNULL)
//		 pcbRemaining			����ַ�����ʣ�೤��(����Ҫ����ΪNULL)
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
HRESULT StringAddCharCb( IN OUT LPTSTR pszDest, IN size_t cbDest, IN TCHAR tAddChar, 
						OUT LPTSTR* ppszDestEnd, OUT size_t* pcbRemaining);

//-------------------------------------------------------------------------
// ����: BOOL GetLastWriteFileTime( IN LPCTSTR pFileName, OUT FILETIME * pftFileTime )
// ˵��: ���һ���ļ������д��ʱ��
// ����: pFileName				�����ļ���
//		 pftFileTime			�ļ�ʱ��
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL GetLastWriteFileTime( IN LPCTSTR pFileName, OUT FILETIME * pftFileTime );

//-------------------------------------------------------------------------
// ����: void WriteMiniDump( LPCTSTR szDumpFileName, PEXCEPTION_POINTERS pException,
//							IN BOOL bNeedFullMemory )
// ˵��: ��һ���ļ�����ڴ�ת��
// ����: szDumpFileName			ת���ļ���
//		 pException				�쳣ָ��
//		 bNeedFullMemory		��Ҫȫ�ڴ�Dump���⽫���ºܴ��Dump�ļ�
// ����: �Ƿ�ɹ� ����ͨ��GetLastError���ʧ��ԭ��
//-------------------------------------------------------------------------
BOOL WriteMiniDump( IN LPCTSTR szDumpFileName, IN PEXCEPTION_POINTERS pException,
				   IN BOOL bNeedFullMemory );

//-------------------------------------------------------------------------
// ����: BOOL WriteModules( IN LPCTSTR szFileName )
// ˵��: ���쳣�������������ģ����Ϣ
// ����: szFileName				д��ģ�����ļ���
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL WriteModules( IN LPCTSTR szFileName );

//-------------------------------------------------------------------------
// ����: BOOL SetCharacterName( IN LPCTSTR szCharacterName )
// ˵��: ���÷��͵��ļ���·���ϵ��������ƣ����û��������û�����·�����
//		 ��������ṩ�˿��ַ��Ͷ��ֽ��ַ����ֺ���
// ����: szCharacterName		��������
// ����: �Ƿ����óɹ�
//-------------------------------------------------------------------------
BOOL SetCharacterNameA( IN LPCSTR szCharacterName );
BOOL SetCharacterNameW( IN LPCWSTR szCharacterName );
#ifdef UNICODE
#define SetCharacterName SetCharacterNameW
#else//UNICODE
#define SetCharacterName SetCharacterNameA
#endif//UNICODE

//-------------------------------------------------------------------------
// ����: BOOL SetCharacterID( IN DWORD dwCharacterID )
// ˵��: ���÷��͵��ļ���·���ϵ�����ID�����Ϊ0��ʹ��IP��ַ
// ����: dwCharacterID			����ID
// ����: �Ƿ����óɹ�
//-------------------------------------------------------------------------
BOOL SetCharacterID( IN DWORD dwCharacterID );

//-------------------------------------------------------------------------
// ����: BOOL SetProjectName( IN LPCTSTR szProjectName )
// ˵��: ���÷��͵��ļ���·���ϵ���Ŀ���ƣ����û��������ʹ�ó���ģ����
// ����: szProjectName			��Ŀ����
// ����: �Ƿ����óɹ�
//-------------------------------------------------------------------------
BOOL SetProjectName( IN LPCTSTR szProjectName );

//-------------------------------------------------------------------------
// ����: BOOL GetSendToDirCb( IN OUT LPTSTR pszDest, IN size_t cbDest )
// ˵��: �������͵���FTP·�������·���ڳ��������ڼ䲻��ı�
// ����: pszDest				Ŀ���ִ�
//		 cbDest					Ŀ��Ĵ�С(�ֽ���)
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL GetSendToDirCb( IN OUT LPTSTR pszDest, IN size_t cbDest );

//-------------------------------------------------------------------------
// ����: void SetSendToFtpServer( IN LPCTSTR lpszServerName, IN WORD wHostPort,
//						IN LPCTSTR lpszUserName, IN LPCTSTR lpszPassWord )
// ˵��: ���÷��͵���������ַ����Ӧ�������е�SendFile����֮ǰ
// ����: lpszServerName			��������ַ
//		 wHostPort				�������˿�
//		 lpszUserName			�û���
//		 lpszPassWord			����
// ����: ��
//-------------------------------------------------------------------------
void SetSendToFtpServer( IN LPCTSTR lpszServerName, IN WORD wHostPort,
						IN LPCTSTR lpszUserName, IN LPCTSTR lpszPassWord );

//-------------------------------------------------------------------------
// ����: BOOL SendFile( IN LPCTSTR szFileName )
// ˵��: �����ļ���������
// ����: szFileName				Ҫ���͵��ļ���
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL SendFile( IN LPCTSTR szFileName );

//-------------------------------------------------------------------------
// ����: BOOL GetHostInfoCb( OUT LPDWORD outIP, OUT LPTSTR outName, IN size_t cbName )
// ˵��: ��õ�ǰ��������Ϣ
// ����: outIP					���ر�����IP(����Ҫ����ΪNULL)
//		 outName				���ػ�����(����Ҫ����ΪNULL)
//		 cbName					��û������ַ����Ĵ�С(�ֽ���)(����Ҫ����ΪNULL)
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL GetHostInfoCb( OUT LPDWORD outIP, OUT LPTSTR outName, IN size_t cbName );

//-------------------------------------------------------------------------
// ����: BOOL DumpAndSend( IN PEXCEPTION_POINTERS pException )
// ˵��: ����DUMP�ļ���ģ���ļ������Զ����͵ļ��ɺ���
// ����: pException				�쳣ָ��
// ����: �Ƿ�ɹ�
//-------------------------------------------------------------------------
BOOL DumpAndSend( IN PEXCEPTION_POINTERS pException );

//-------------------------------------------------------------------------
// ����: BOOL IsBinBuf( IN LPBYTE lpBuf, IN DWORD dwNumberCharsToCheck, 
//						IN DWORD dwMaxIllegalCharacters);
// ˵��: �ж�һ���������е������Ƿ�Ϊ����������
// ����: lpBuf					������
//		 dwNumberCharsToCheck	�����ַ���
//		 dwMinMatchChar			�����м����������ַ��͵��������ƻ���
//								(Ϊ0����dwNumberCharsToCheck��5%)
// ����: �Ƿ�Ϊ���������ݻ�����
//-------------------------------------------------------------------------
BOOL IsBinBuf( IN LPBYTE lpBuf, IN DWORD dwNumberCharsToCheck, 
				IN DWORD dwMinMatchChar );

};

#endif//DumpAndSend_h_
