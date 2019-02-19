#pragma once

#include "curl\curl.h"

#ifdef _DEBUG
#pragma comment(lib, "curl/libcurl_lib/libcurl_MTd_VS2010.lib")
#pragma comment(lib, "curl/openssl_lib/ssleay32_MTd_VS2010.lib")
#pragma comment(lib, "curl/openssl_lib/libeay32_MTd_VS2010.lib")
#else
#pragma comment(lib, "curl/libcurl_lib/libcurl_MT_VS2010.lib")
#pragma comment(lib, "curl/openssl_lib/ssleay32_MT_VS2010.lib")
#pragma comment(lib, "curl/openssl_lib/libeay32_MT_VS2010.lib")
#endif

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

