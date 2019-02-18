#pragma once
#define CURL_STATICLIB

#include "curl\curl.h"

#ifdef _DEBUG
#pragma comment(lib,"libcurl-d_imp.lib")
#else
#pragma comment(lib,"libcurl.lib")
#endif

#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "wldap32.lib" )
#pragma comment(lib, "Advapi32.lib")


#include "stdafx.h"
#include <iostream>
#include <sstream>
using namespace std;


class CCurlInterface{
	wstring AsciiToUnicode(const string& str);
	string UnicodeToUtf8(const wstring& wstr);
	string AsciiToUtf8(const string& str) ;
	//»Øµ÷º¯Êý
	static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
	CURLoption m_Option;
	int httpRequest( const string& strUrl, string &strResponse, const string* strHeader = NULL, const string* data = NULL);
public:
	CCurlInterface();
	int postData( const string& strUrl, string &strResponse, const string* strHeader = NULL, const string* data = NULL);
	int getData( const string& strUrl, string &strResponse, const string* strHeader = NULL, const string* data = NULL);
};

