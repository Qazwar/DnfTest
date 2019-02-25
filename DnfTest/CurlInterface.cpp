#include "StdAfx.h"
#include "CurlInterface.h"


#include "stdafx.h"
#include <iostream>
#include <sstream>
//json
using namespace std;
HWND CCurlInterface::parentHwnd = NULL;

wstring CCurlInterface::AsciiToUnicode(const string& str) 
{
    // Ԥ��-�������п��ֽڵĳ���  
    int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    // ��ָ�򻺳�����ָ����������ڴ�  
    wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
    // ��ʼ�򻺳���ת���ֽ�  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
    wstring ret_str = pUnicode;
    free(pUnicode);
    return ret_str;
}

string CCurlInterface::UnicodeToUtf8(const wstring& wstr) 
{
    // Ԥ��-�������ж��ֽڵĳ���  
    int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    // ��ָ�򻺳�����ָ����������ڴ�  
    char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
    // ��ʼ�򻺳���ת���ֽ�  
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
    string ret_str = pAssii;
    free(pAssii);
    return ret_str;
}


string CCurlInterface::AsciiToUtf8(const string& str) 
{
    return UnicodeToUtf8(AsciiToUnicode(str));
}

//�ص�����
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
		CURL *curl = curl_easy_init();
		CURLcode res;
		std::stringstream out;//HTTP����ͷ  
		struct curl_slist* headers = NULL;
		if(curl)
		{
			string jsonout;
			if (data)
			{
				jsonout = AsciiToUtf8(*data);
			}
			//����http���͵���������ΪJSON
			//����HTTP����ͷ  
			headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
			if(strHeader){
				headers = curl_slist_append(headers, (*strHeader).c_str());
			}
			//����url
			curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			if(m_Option==CURLOPT_POST){
				curl_easy_setopt(curl, m_Option, 1);
			}
			// ����ҪPOST��JSON����
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https  
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false  
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonout.c_str());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonout.size());//�����ϴ�json������,������ÿ��Ժ���
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CCurlInterface::write_data);//���ûص�����
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);//����д����
			res = curl_easy_perform(curl);//ִ��
			curl_slist_free_all(headers); /* free the list again */
			strResponse = out.str();//��������ֵ 
			auto Pos = strResponse.find('{');
			auto LastPos = strResponse.rfind('}');
			if (Pos!=-1&&LastPos!=-1)
			{
				strResponse = strResponse.substr(Pos, LastPos-Pos+1);
			}
			/* always cleanup */ 
			curl_easy_cleanup(curl);
		} 

		return 0;
}

static size_t write_file(void *ptr, size_t size, size_t nmemb, void *stream)  
{  
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);  
	return written;  
}

int CCurlInterface::ProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)  
{  
	if ( dltotal > -0.1 && dltotal < 0.1 )  
		return 0;  
	int nPos = (int) ( (dlnow/dltotal)*100 );  
	//֪ͨ�������������ؽ���  
	::PostMessage(parentHwnd, WM_DOWNLOAD, nPos, 0);
	return 0; 
}

int CCurlInterface::fileDownload(const string& fileName, HWND hwnd)
{
	CCurlInterface::parentHwnd = hwnd;
	auto filePath = common::GetFilePath(fileName);
	common::CreateDir(common::stringToCString(filePath));
	CURL* curl = curl_easy_init();
	FILE *pagefile = fopen(filePath.c_str(),"wb");
	do 
	{
		CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, ServerUrl+"/dnf/upgrade/download");
		if (res != CURLE_OK)  
		{   
			break;
		}  
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, (string("filename:")+fileName).c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		if (res != CURLE_OK)  
		{   
			break;
		} 
		// �����ض����������  
		res = curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);  
		if (res != CURLE_OK)  
		{   
			break;
		} 
		// ����301��302��ת����location  
		res = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);  
		if (res != CURLE_OK)  
		{   
			break;
		} 
		res = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);  
		if (res != CURLE_OK)  
		{   
			break;
		} 
		//���ý��Ȼص�����  
		res = curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
		if (res != CURLE_OK)  
		{   
			break;
		} 
		//��ʼִ������  

		res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file); 
		if (res != CURLE_OK)  
		{   
			break;
		} 
		if (pagefile) {
			/* write the page body to this file handle */   
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile); 
			/* get it! */   
			CURLcode retcCode = curl_easy_perform(curl);  
			//�鿴�Ƿ��г�����Ϣ  
			const char* pError = curl_easy_strerror(retcCode);  
			/* close the header file */
			fclose(pagefile);  
		}  
	} while (false);	
	//����curl����ǰ��ĳ�ʼ��ƥ��
	if(pagefile){
		fclose(pagefile);
	}
	curl_easy_cleanup(curl);  
	return 0; 
}

int CCurlInterface::getData( const string& strUrl, string &strResponse, const string* strHeader, const string* data)
{
	CURL *curl = curl_easy_init();  
	// res code  
	CURLcode res;  
	if (curl)  
	{  
		// set params  
		std::stringstream out;//HTTP����ͷ 
		struct curl_slist* headers = NULL;
		if(strHeader){
			headers = curl_slist_append(headers, (*strHeader).c_str());
		}
		//����url
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
		strResponse = out.str();//��������ֵ
		auto Pos = strResponse.find('{');
		auto LastPos = strResponse.rfind('}');
		if (Pos!=-1&&LastPos!=-1)
		{
			strResponse = strResponse.substr(Pos, LastPos-Pos+1);
		}
		curl_easy_cleanup(curl);  
	}
	return res;  
}
