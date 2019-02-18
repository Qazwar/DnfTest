#include "StdAfx.h"
#include "CurlInterface.h"


#include "stdafx.h"
#include <iostream>
#include <sstream>
//json
using namespace std;


wstring CCurlInterface::AsciiToUnicode(const string& str) 
{
    // 预算-缓冲区中宽字节的长度  
    int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    // 给指向缓冲区的指针变量分配内存  
    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
    // 开始向缓冲区转换字节  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
    wstring ret_str = pUnicode;
    free(pUnicode);
    return ret_str;
}

string CCurlInterface::UnicodeToUtf8(const wstring& wstr) 
{
    // 预算-缓冲区中多字节的长度  
    int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    // 给指向缓冲区的指针变量分配内存  
    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
    // 开始向缓冲区转换字节  
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
    string ret_str = pAssii;
    free(pAssii);
    return ret_str;
}


string CCurlInterface::AsciiToUtf8(const string& str) 
{
    return UnicodeToUtf8(AsciiToUnicode(str));
}

//回调函数
size_t CCurlInterface::write_data(void *ptr, size_t size, size_t nmemb, void *stream) 
{
    string data((const char*) ptr, (size_t) size * nmemb);
    *((stringstream*) stream) << data << endl;
    return size * nmemb;
}

int CCurlInterface::postData( const string& strUrl, string &strResponse, const string* strHeader, const string* data)
{
	m_Option = CURLOPT_POST;
	return httpRequest( strUrl, strResponse, strHeader, data);
}

CCurlInterface::CCurlInterface()
{
	m_Option = CURLOPT_POST;
}

int CCurlInterface::httpRequest( const string& strUrl, string &strResponse, const string* strHeader, const string* data)
{
	{
		CURL *curl = curl = curl_easy_init();
		CURLcode res;
		std::stringstream out;//HTTP报文头  
		struct curl_slist* headers = NULL;
		if(curl)
		{
			string jsonout;
			if (data)
			{
				jsonout = AsciiToUtf8(*data);
			}
			//设置http发送的内容类型为JSON
			//构建HTTP报文头  
			headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
			if(strHeader){
				headers = curl_slist_append(headers, (*strHeader).c_str());
			}
			//设置url
			curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			if(m_Option==CURLOPT_POST){
				curl_easy_setopt(curl, m_Option, 1);
			}
			// 设置要POST的JSON数据
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonout.size());//设置上传json串长度,这个设置可以忽略
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CCurlInterface::write_data);//设置回调函数
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//设置写数据
			res = curl_easy_perform(curl);//执行
			curl_slist_free_all(headers); /* free the list again */
			strResponse = out.str();//返回请求值 
			/* always cleanup */ 
			curl_easy_cleanup(curl);
		} 

		return 0;
	}
}

int CCurlInterface::getData( const string& strUrl, string &strResponse, const string* strHeader, const string* data)
{
	CURL *curl = curl_easy_init();  
	// res code  
	CURLcode res;  
	if (curl)  
	{  
		// set params  
		std::stringstream out;//HTTP报文头 
		struct curl_slist* headers = NULL;
		if(strHeader){
			headers = curl_slist_append(headers, (*strHeader).c_str());
		}
		//设置url
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str()); // url  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CCurlInterface::write_data);  
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);  
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
		curl_easy_setopt(curl, CURLOPT_HEADER, 1);  
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time  
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
		// start req  
		res = curl_easy_perform(curl);
		strResponse = out.str();//返回请求值
		auto Pos = strResponse.find('\{');
		auto LastPos = strResponse.rfind('\}');
		if (Pos!=-1&&LastPos!=-1)
		{
			strResponse = strResponse.substr(Pos, LastPos-Pos+1);
		}
	}  
	// release curl  
	curl_easy_cleanup(curl);  
	return res;  
}
