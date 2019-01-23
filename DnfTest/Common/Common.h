#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include "md5.h"
using namespace std;

void __cdecl Output(char* lpszFormat, ...);


HMODULE GetSelfModuleHandle();
string GetModulePath(HMODULE hModule, BOOL IsAll=FALSE);
bool FileExist(std::string strFileName);
string strToHexA(CHAR *lpStr);
wstring strToHexW(WCHAR *lpStr);
wstring ANSIToUnicode( const string& str );
string UnicodeToANSI( const wstring& str );
wchar_t * AToU (const char * str);
char * UToA (const wchar_t * str);
wchar_t * U8ToU (const char * str);
char * UToU8 (const wchar_t * str);
double Distance2D(POINT p1,POINT p2);

float AnsiToFloat(char * szBuffer);//单个汉字转换成float
char * FloatToAnsi(float f);//float 转换成汉字

std::string md5_encode(std::string str);
std::string md5_encode(void * lpByte,DWORD dwLenth);

string&  lTrim(string   &ss);
string&  rTrim(string   &ss);
string&   trim(string   &st);
void trim2(string& str);

string str_replace(TCHAR src,TCHAR dst,string &str);
string str_remove_enter(string &str);
std::vector<string> explode(const string &delimiter, const string &str);
int rand_range(int ibegin, int iend);

string escapeURL(const string &URL);
std::string deescapeURL(const std::string &URL);

BOOL WINAPI EnableDebugPrivilege(BOOL bEnable);

int API_StringToTime(const string &strDateStr,time_t &timeData);
int API_TimeToString(string &strDateStr,const time_t &timeData);
int API_StringToTimeEX(const string &strDateStr,time_t &timeData);
int API_TimeToStringEX(string &strDateStr,const time_t &timeData);
//////////////////////////////////////////////////////////////////////////////////

#define ReleaseVectorMem(X,Y)\
	;\
##Y.clear();\
	std::vector<##X>().swap(##Y);



