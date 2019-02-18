#include "stdafx.h"
#include <Nb30.h>
#include "HttpClient.h"
#include "Global.h"
#include "Config.h"

#pragma comment(lib,"netapi32.lib")

namespace common{
	CString IntToCString(const int& i)
	{
		CString csText;
		csText.Format(_T("%d"), i);
		return csText;
	}

	CString stringToCString(const string& str)
	{
		CString ans;  
		ans.Format("%s", str.c_str());  
		return ans;
	}

	string CStringTostring(const CString& cs)
	{
		 return (LPSTR)(LPCTSTR)cs;
	}

	CStringArray* SplitString(const string& str,  char split)
	{
		CStringArray *strArray = new CStringArray;
		auto cs = stringToCString(str);
		int iIndex = 0;
		while (1)
		{
			iIndex = cs.Find(split);
			if(iIndex >= 0)
			{
				strArray->Add(cs.Left(iIndex));
				cs = cs.Right(cs.GetLength()-iIndex-1);
			}
			else
			{
				break;
			}
		}
		strArray->Add(cs);
		return strArray;
	}

	string getMAC()
	{
		string strMac;
		char * mac = new char[50];
		ZeroMemory(mac, 50);
		NCB ncb;
		typedef struct _ASTAT_
		{
			ADAPTER_STATUS   adapt;
			NAME_BUFFER   NameBuff[30];
		}ASTAT, *PASTAT;

		ASTAT Adapter;
		typedef struct _LANA_ENUM
		{
			UCHAR length;
			UCHAR lana[MAX_LANA];
		}LANA_ENUM;
		LANA_ENUM lana_enum;
		UCHAR uRetCode;
		memset(&ncb, 0, sizeof(ncb));
		memset(&lana_enum, 0, sizeof(lana_enum));

		ncb.ncb_command = NCBENUM;
		ncb.ncb_buffer = (unsigned char *)&lana_enum;
		ncb.ncb_length = sizeof(LANA_ENUM);
		uRetCode = Netbios(&ncb);
		if (uRetCode != NRC_GOODRET)
			return strMac;

		for (int lana = 0; lana<lana_enum.length; lana++)
		{
			ncb.ncb_command = NCBRESET;
			ncb.ncb_lana_num = lana_enum.lana[lana];
			uRetCode = Netbios(&ncb);
			if (uRetCode == NRC_GOODRET)
				break;
		}
		if (uRetCode != NRC_GOODRET)
			return strMac;

		memset(&ncb, 0, sizeof(ncb));
		ncb.ncb_command = NCBASTAT;
		ncb.ncb_lana_num = lana_enum.lana[0];
		strcpy_s((char*)ncb.ncb_callname,5, "*");
		ncb.ncb_buffer = (unsigned char *)&Adapter;
		ncb.ncb_length = sizeof(Adapter);
		uRetCode = Netbios(&ncb);
		if (uRetCode != NRC_GOODRET)
			return strMac;
		sprintf_s(mac,18, "%02X-%02X-%02X-%02X-%02X-%02X",
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]
		);
		strMac = mac;
		delete mac;
		mac = nullptr;
		return strMac;
	}

	int QueryUser()
	{
		CHttpClient client;
		string resp;
		client.HttpGet("http://47.106.111.213:9050/dnf/user/query",(string("mac:")+global_instance.getMac()).c_str(), NULL, resp);
		neb::CJsonObject pt_root;
		pt_root.Parse(resp);
		int status = 0;
		auto pt_data = pt_root["data"];
		pt_data.Get("status", status);
		return status;
	}

	int RegisterUser()
	{
		CHttpClient client;
		string resp;
		client.HttpPost("http://47.106.111.213:9050/dnf/user/register",(string("mac:")+global_instance.getMac()).c_str(), NULL, resp);
		neb::CJsonObject pt_root;
		pt_root.Parse(resp);
		int status = 0;
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		pt_data.Get("status", status);
		return status;
	}
	char* UnicodeToUtf8(const wchar_t* unicode)
	{
		int len;
		len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
		char *szUtf8 = (char*)malloc(len + 1);
		memset(szUtf8, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
		return szUtf8;
	}

#define IS_NUMBER(c)        ((c) && (((c) >= '0' && (c) <= '9') || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F')))

	CString TranslateString(const CString &csText)
	{
		wstring ret;
		USES_CONVERSION; 
		LPWSTR pwStr=new wchar_t[csText.GetLength()+1]; 
		wcscpy(pwStr,T2W((LPCTSTR)csText));
		LPCWSTR curChar = pwStr;

		while (*curChar)
		{
			if (curChar[0] == L'\\' &&
				(curChar[1] == L'u' || curChar[1] == L'U') &&
				IS_NUMBER(curChar[2]) &&
				IS_NUMBER(curChar[3]) &&
				IS_NUMBER(curChar[4]) &&
				IS_NUMBER(curChar[5]))
			{
				wchar_t hex[8] = {'0', 'x'};
				hex[2] = curChar[2];
				hex[3] = curChar[3];
				hex[4] = curChar[4];
				hex[5] = curChar[5];
				int i;
				StrToIntExW(hex, STIF_SUPPORT_HEX, &i);
				ret += (wchar_t)i;

				curChar += 6;
			}
			else
			{
				ret += (wchar_t)*curChar;
				curChar++;
			}
		}
		if(pwStr){
			delete pwStr;
			pwStr = NULL;
		}
		return CString(ret.c_str());
	}

	string GetDefines()
	{
		CHttpClient client;
		string resp;
		client.HttpGet("http://47.106.111.213:9050/dnf/config/defines", NULL, NULL, resp);
		neb::CJsonObject pt_root;
		pt_root.Parse(common::CStringTostring(TranslateString(common::stringToCString(resp))));
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		return pt_data.ToString();
	}

	void PostConfig(const CString& data)
	{
		CHttpClient client;
		string resp;
		client.HttpPost("http://127.0.0.1:9091/dnf/config/user",(string("mac:")+global_instance.getMac()).c_str(), 
			data, resp);
		return;
	}
}