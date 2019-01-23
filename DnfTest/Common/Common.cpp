#include "stdafx.h"
#include "Common.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <Shlwapi.h>

#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma warning(disable:4996)

void __cdecl Output(LPCTSTR lpszFormat, ...)
{
#ifdef _DEBUG
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[sizeof(TCHAR) * 10240] ={TEXT("Debug:")};
#ifdef UNICODE
	nBuf = _vstprintf(szBuffer + wcslen(szBuffer)*sizeof(TCHAR), lpszFormat, args); 
#else
	nBuf = _vstprintf(szBuffer + strlen(szBuffer)*sizeof(TCHAR), lpszFormat, args); 
#endif
	// was there an error? was the expanded string too long?x
	//ASSERT(nBuf >= 0);
	va_end(args);
	OutputDebugString(szBuffer);
#endif // _DEBUG
}

/* 
  获取自己模块句柄 
*/  
HMODULE GetSelfModuleHandle()  
{  
    MEMORY_BASIC_INFORMATION mbi;  
    return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) ? (HMODULE) mbi.AllocationBase : NULL);  
}  

//获得模块当前路径,除了模块名称
string GetModulePath(HMODULE hModule, BOOL IsAll)
{
	string strPath;
	const int nBufSize = 512;
	TCHAR chBuf[nBufSize];
	ZeroMemory(chBuf,nBufSize);

	if (GetModuleFileName(hModule,chBuf,nBufSize))
	{
		if(IsAll == TRUE)
		{
			strPath = chBuf;
		}else
		{
			TCHAR* lpStrPath = chBuf;
			PathRemoveFileSpec(lpStrPath);
			strPath = lpStrPath;
		}
	}
	return strPath;
}

bool FileExist(std::string strFileName)
{
	std::fstream _file;
	_file.open(strFileName,std::ios::in);
	if(!_file)
	{
		return false;
	}
	_file.close();
	return true;
}

//字符串转A(HEX字符串)
string strToHexA(CHAR *lpStr)
{
	string strRet;
	if (lpStr == NULL)
	{
		return "";
	}
	int nLen = (int)strlen(lpStr);

	CHAR szTemp[10];
	strRet ="";
	for (int i = 0; i < nLen; i++)
	{
		sprintf(szTemp,"%02X",(BYTE)lpStr[i]);
		strRet += szTemp;
	}

	return strRet;
}

//字符串转W(HEX字符串)
wstring strToHexW(WCHAR *lpStr)
{
	LPBYTE lpByteStr = (LPBYTE)lpStr;
	wstring strRet;
	if (lpStr == NULL)
	{
		return L"";
	}
	int nLen = (int)wcslen(lpStr) * 2;
	Output(TEXT("[%s] nLen = %d"),lpStr,nLen);

	WCHAR szTemp[10];
	strRet = L"";
	for (int i = 0; i < nLen; i++)
	{
		wsprintfW(szTemp,L"%02X",(BYTE)lpByteStr[i]);
		strRet += szTemp;
	}
	Output(TEXT("%s"),strRet.c_str());
	return strRet;
}

//窄字符转宽字符
wstring ANSIToUnicode( const string& str )
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0 );  
	wchar_t *  pUnicode;  
	pUnicode = new  wchar_t[unicodeLen+1];  //分配内存空间
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen );  
	wstring  rt;  //字符串对象 
	rt = (wchar_t*)pUnicode;
	delete  pUnicode; 
	return  rt;  
}


//宽字符转窄字符
string UnicodeToANSI( const wstring& str )
{

	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset(( void* )pElementText,0,sizeof(char)*(iTextLen+1));
	::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	string strText;
	strText = pElementText;
	delete pElementText;
	return strText;
}


wchar_t * AToU (const char * str)
{
	int textlen;
	wchar_t * result;

	textlen=MultiByteToWideChar (CP_ACP,0,str,-1,NULL,0);
	result= (wchar_t*) malloc ((textlen+1) *sizeof (wchar_t));
	memset (result,0, (textlen+1) *sizeof (wchar_t));
	MultiByteToWideChar (CP_ACP,0,str,-1, (LPWSTR) result,textlen);
	return result;
}

char * UToA (const wchar_t * str)
{
	char * result;
	int textlen;

	// wide char to multi char
	textlen=WideCharToMultiByte (CP_ACP,0,str,-1,NULL,0,NULL,NULL);
	result= (char*) malloc ((textlen+1) *sizeof (char));
	memset (result,0,sizeof (char) * (textlen+1));
	WideCharToMultiByte (CP_ACP,0,str,-1,result,textlen,NULL,NULL);
	return result;
}

wchar_t * U8ToU (const char * str)
{
	int textlen;
	wchar_t * result;

	textlen=MultiByteToWideChar (CP_UTF8,0,str,-1,NULL,0);
	result= (wchar_t*) malloc ((textlen+1) *sizeof (wchar_t));
	memset (result,0, (textlen+1) *sizeof (wchar_t));
	MultiByteToWideChar (CP_UTF8,0,str,-1, (LPWSTR) result,textlen);
	return result;
}

char * UToU8 (const wchar_t * str)
{
	char * result;
	int textlen;

	// wide char to multi char
	textlen=WideCharToMultiByte (CP_UTF8,0,str,-1,NULL,0,NULL,NULL);
	result= (char*) malloc ((textlen+1) *sizeof (char));
	memset (result,0,sizeof (char) * (textlen+1));
	WideCharToMultiByte (CP_UTF8,0,str,-1,result,textlen,NULL,NULL);
	return result;
}

double Distance2D(POINT p1,POINT p2)//计算两点之间距离
{
	double lfDistance;
	lfDistance = sqrt(pow((double)(p1.x-p2.x),2)+pow((double)(p1.y-p2.y),2));
	return lfDistance;
}

float AnsiToFloat(char * szBuffer)//单个汉字转换成float
{
	char szBuffer1[2] = {0};
	for (int i = 0; i<2; i++)
	{
		szBuffer1[i] = szBuffer[i];
	}
	float f;
	f = (WORD)*(WORD *)szBuffer1;
	return f;
}
char * FloatToAnsi(float f)//float 转换成汉字
{
	WORD w;
	w = (WORD)f;
	char *szBuffer = (char *)malloc(sizeof(char)*4);
	ZeroMemory(szBuffer,sizeof(char)*4);
	*(WORD *)szBuffer = w;
	return szBuffer;
}
std::string md5_encode(std::string str)
{
	MD5 md5;
	std::string Result;
	md5.reset();
	md5.update(str);
	Result = md5.toString();
	transform(Result.begin(),Result.end(),Result.begin(),::toupper);
	return Result;
}

std::string md5_encode(void * lpByte,DWORD dwLenth)
{
	MD5 md5;
	std::string Result;
	md5.reset();
	md5.update(lpByte,dwLenth);
	Result = md5.toString();
	transform(Result.begin(),Result.end(),Result.begin(),::toupper);
	return Result;
}

string&  lTrim(string   &ss)   
{   
		string::iterator   p=find_if(ss.begin(),ss.end(),not1(ptr_fun(isspace)));   
		ss.erase(ss.begin(),p);   
		return  ss;   
}   
string&  rTrim(string   &ss)   
{   
	string::reverse_iterator  p=find_if(ss.rbegin(),ss.rend(),not1(ptr_fun(isspace)));   
	ss.erase(p.base(),ss.end());   
	return   ss;   
}   
string&   trim(string   &st)   
{   
	lTrim(rTrim(st));
	return   st;
}   

void trim2(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}


/*
string& str_replace(wchar_t src,wchar_t dst,string &str)
{
	string strret;
	const char* iter = str.c_str();
	for (int i=0; i<=str.size(); i++)
	{
		if (*iter==src)
		{
			strret += dst;
		}
		else
		{
			strret += iter;
		}

		iter++;
	}
	return str;
}*/

string str_replace(char src,char dst,string &str)
{
	string strret(str);
	while(true)
	{
		string::size_type pos = strret.find(src);
		if (pos == string::npos)
		{
			break;
		}

		strret.replace(pos, 1, &dst);
	}
	return strret;
}

string str_remove_enter(string &str)
{
	string strret(str);
	while(true)
	{
		string::size_type pos = strret.find("\r");
		if (pos == string::npos)
		{
			break;
		}

		strret.erase(pos, 1);
	}

	while(true)
	{
		string::size_type pos = strret.find("\n");
		if (pos == string::npos)
		{
			break;
		}

		strret.erase(pos, 1);
	}

	return strret;
}

std::vector<string> explode(const string &delimiter, const string &str)
{
	std::vector<string> arr;

	string strcopy(str);
	while(true)
	{
		string::size_type pos = strcopy.find(delimiter);
		if (pos == string::npos)
		{
			arr.push_back(strcopy);
			break;
		}

		//把前面的字符串取出来
		string strTmp = strcopy.substr(0, pos);
		if (strTmp.empty())
		{
			strTmp.resize(1);
		}

		arr.push_back(strTmp);

		strcopy.erase(0, min(pos+delimiter.size(), strcopy.size()));
	}

	return arr;
}


int rand_range(int ibegin, int iend)
{
	srand((unsigned) time(0));
	int i = (int)(ibegin+ rand()%(iend-ibegin+1));
	return i;
}

char dec2hexChar(short int n) {
	if ( 0 <= n && n <= 9 ) {
		return char( short('0') + n );
	} else if ( 10 <= n && n <= 15 ) {
		return char( short('A') + n - 10 );
	} else {
		return char(0);
	}
}
short int hexChar2dec(char c) {
	if ( '0'<=c && c<='9' ) {
		return short(c-'0');
	} else if ( 'a'<=c && c<='f' ) {
		return ( short(c-'a') + 10 );
	} else if ( 'A'<=c && c<='F' ) {
		return ( short(c-'A') + 10 );
	} else {
		return -1;
	}
}

string escapeURL(const string &URL)
{
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) {
		char c = URL[i];
		if (
			( '0'<=c && c<='9' ) ||
			( 'a'<=c && c<='z' ) ||
			( 'A'<=c && c<='Z' ) ||
			c=='/' || c=='.'
			) {
				result += c;
		} else {
			int j = (short int)c;
			if ( j < 0 ) {
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1*16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0);
		}
	}
	return result;
}
std::string deescapeURL(const std::string &URL) {
	string result = "";
	for ( unsigned int i=0; i<URL.size(); i++ ) {
		char c = URL[i];
		if ( c != '%' ) {
			result += c;
		} else {
			char c1 = URL[++i];
			char c0 = URL[++i];
			int num = 0;
			num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
			result += char(num);
		}
	}
	return result;
}

/************************************************************************/
/* 功能：提升权限                                                       */
/************************************************************************/
BOOL WINAPI EnableDebugPrivilege(BOOL bEnable)
{
	//附给本进程特权，用以取得系统进程的信息
	BOOL	bOk	= FALSE;
	HANDLE	hToken;

	//打开一个进程的访问令牌
	if ( OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken) )
	{
		TOKEN_PRIVILEGES	tp;
		tp.PrivilegeCount	= 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes	= bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk	= (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}
	return(bOk);
}

/*
    string to time_t
    时间格式  2009-3-24
*/
int API_StringToTime(const string &strDateStr,time_t &timeData)
{
    char *pBeginPos = (char*) strDateStr.c_str();
    char *pPos = strstr(pBeginPos,"-");
    if(pPos == NULL)
    {
        return -1;
    }
    int iYear = atoi(pBeginPos);
    int iMonth = atoi(pPos + 1);
 
    pPos = strstr(pPos + 1,"-");
    if(pPos == NULL)
    {
        return -1;
    }
 
    int iDay = atoi(pPos + 1);
 
    struct tm sourcedate;
    ZeroMemory((void*)&sourcedate,sizeof(sourcedate));
    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth - 1; 
    sourcedate.tm_year = iYear - 1900;
    
    timeData = mktime(&sourcedate);  
 
    return 0;
}
 
/*
    time_t to string
*/
int API_TimeToString(string &strDateStr,const time_t &timeData)
{
    char chTmp[15];
    ZeroMemory(chTmp,sizeof(chTmp));
 
    struct tm *p;
    p = localtime(&timeData);
 
    p->tm_year = p->tm_year + 1900;
 
    p->tm_mon = p->tm_mon + 1;
 
 
    _sntprintf(chTmp,sizeof(chTmp),"%04d-%02d-%02d",
        p->tm_year, p->tm_mon, p->tm_mday);
 
    strDateStr = chTmp;
    return 0;
}

/*
   string to time_t   
   时间格式 2009-3-24 0:00:08 或 2009-3-24
   */
int API_StringToTimeEX(const string &strDateStr,time_t &timeData)
{
    char *pBeginPos = (char*) strDateStr.c_str();
    char *pPos = strstr(pBeginPos,"-");
    if(pPos == NULL)
    {
        //printf("strDateStr[%s] err \n", strDateStr.c_str());
        return -1;
    }
    int iYear = atoi(pBeginPos);
    int iMonth = atoi(pPos + 1);
    pPos = strstr(pPos + 1,"-");
    if(pPos == NULL)
    {
        //printf("strDateStr[%s] err \n", strDateStr.c_str());
        return -1;
    }
    int iDay = atoi(pPos + 1);
    int iHour=0;
    int iMin=0;
    int iSec=0;
    pPos = strstr(pPos + 1," ");
    //为了兼容有些没精确到时分秒的
    if(pPos != NULL)
    {
        iHour=atoi(pPos + 1);
        pPos = strstr(pPos + 1,":");
        if(pPos != NULL)
        {
            iMin=atoi(pPos + 1);
            pPos = strstr(pPos + 1,":");
            if(pPos != NULL)
            {
                iSec=atoi(pPos + 1);
            }
        }
    }
 
    struct tm sourcedate;
    ZeroMemory((void*)&sourcedate,sizeof(sourcedate));
    sourcedate.tm_sec = iSec;
    sourcedate.tm_min = iMin; 
    sourcedate.tm_hour = iHour;
    sourcedate.tm_mday = iDay;
    sourcedate.tm_mon = iMonth - 1; 
    sourcedate.tm_year = iYear - 1900;
    timeData = mktime(&sourcedate);  
    return 0;
}
/*
   time_t to string 时间格式 2009-3-24 0:00:08 
   */
int API_TimeToStringEX(string &strDateStr,const time_t &timeData)
{
    char chTmp[100];
    ZeroMemory(chTmp,sizeof(chTmp));
    struct tm *p;
    p = localtime(&timeData);
    p->tm_year = p->tm_year + 1900;
    p->tm_mon = p->tm_mon + 1;
 

    _sntprintf(chTmp,sizeof(chTmp),"%04d-%02d-%02d %02d:%02d:%02d",
            p->tm_year, p->tm_mon, p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
    strDateStr = chTmp;
    return 0;
}

