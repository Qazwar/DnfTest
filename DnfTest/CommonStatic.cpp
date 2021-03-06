#include "stdafx.h"
#include <Nb30.h>
#include "Global.h"
#include "Config.h"
#include "CurlInterface.h"

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

	vector<CString> SplitCString(CString strSource, CString ch)
	{
		vector <CString> vecString;
		int iPos = 0;
		CString strTmp;
		strTmp = strSource.Tokenize(ch,iPos);
		while(strTmp.Trim() != _T(""))
		{
			vecString.push_back(strTmp);
			strTmp = strSource.Tokenize(ch,iPos);
		}
		return vecString;
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
		CCurlInterface client;
		string resp;
		client.getData(ServerUrl+"/dnf/user/query", resp, &(string("mac:")+global_instance.getMac()), NULL);
		neb::CJsonObject pt_root;
		pt_root.Parse(resp);
		int status = 0;
		auto pt_data = pt_root["data"];
		pt_data.Get("status", status);
		return status;
	}

	int RegisterUser()
	{
		CCurlInterface client;
		string resp;
		client.postData(ServerUrl+"/dnf/user/register", resp,&(string("mac:")+global_instance.getMac()), NULL);
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
		CCurlInterface client;
		string resp;
		client.getData(ServerUrl+"/dnf/config/defines", resp,NULL, NULL );
		neb::CJsonObject pt_root;
		pt_root.Parse(common::CStringTostring(TranslateString(common::stringToCString(resp))));
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		return pt_data.ToString();
	}

	void PostConfig(const CString& data)
	{
		CCurlInterface client;
		string resp;
		client.postData(ServerUrl+"/dnf/config/user", resp,&(string("mac:")+global_instance.getMac()), 
			&common::CStringTostring(data));
		return;
	}

	string GetConfig()
	{
		CCurlInterface client;
		string resp;
		client.getData(ServerUrl+"/dnf/config/user", resp, &(string("mac:")+global_instance.getMac()), NULL);
		neb::CJsonObject pt_root;
		pt_root.Parse(common::CStringTostring(TranslateString(common::stringToCString(resp))));
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		return pt_data.ToString();
	}

	string GetVersion()
	{
		CCurlInterface client;
		string resp;
		client.getData(ServerUrl+"/dnf/upgrade/version", resp, NULL, NULL);
		neb::CJsonObject pt_root;
		pt_root.Parse(common::CStringTostring(TranslateString(common::stringToCString(resp))));
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		neb::CJsonObject pt_version;
		pt_data.Get("version", pt_version);
		return pt_version.ToString();
	}

	int Upgrade()
	{
		auto version = GetVersion();
		HRSRC hsrc=FindResource(0, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		HGLOBAL hgbl=LoadResource(0,hsrc);
		BYTE *pBt=(BYTE *)LockResource(hgbl);
		VS_FIXEDFILEINFO* pFinfo = (VS_FIXEDFILEINFO*)(pBt+40);    
		CString valStr;   
		valStr.Format(_T("\"%d.%d.%d.%d\""),
			(pFinfo->dwFileVersionMS >> 16) & 0xFF,
			(pFinfo->dwFileVersionMS) & 0xFF,
			(pFinfo->dwFileVersionLS >> 16) & 0xFF,
			(pFinfo->dwFileVersionLS) & 0xFF );
		auto currentVersion = common::CStringTostring(valStr);
		LOG_DEBUG<<"file version "<<currentVersion.c_str();
		return version.compare(currentVersion);
	}

	list<string> getFileList()
	{
		CCurlInterface client;
		string resp;
		client.getData(ServerUrl+"/dnf/upgrade/files", resp, NULL, NULL);
		neb::CJsonObject pt_root;
		pt_root.Parse(common::CStringTostring(TranslateString(common::stringToCString(resp))));
		neb::CJsonObject pt_data;
		pt_root.Get("data", pt_data);
		neb::CJsonObject pt_files;
		pt_data.Get("files", pt_files);
		list<string> listString;
		for(auto index(0); index < pt_files.GetArraySize(); index++)
		{
			string fileName;
			pt_files.Get(index, fileName);
			listString.push_back(fileName);
		}
		return  listString;
	}

	string GetFilePath(const string& path)
	{
		auto csPath = stringToCString(path);
		auto pos = csPath.Find(_T("static/version/"));
		auto filePath = csPath.Right(csPath.GetLength()-(pos+15));
		filePath = GetModuleDir()+_T("/downloads/")+filePath;
		return CStringTostring(filePath);
	}

	CString GetFileName(CString path)
	{
		auto pos = path.ReverseFind(_T('\\'));
		if(pos==-1){
			pos = path.ReverseFind(_T('/'));
		}
		path = path.Right(path.GetLength()-(pos+1));
		return path;
	}

	CString GetModuleDir()
	{
		HMODULE module = GetModuleHandle(0); 
		char pFileName[MAX_PATH]; 
		GetModuleFileName(module, pFileName, MAX_PATH); 

		CString csFullPath(pFileName); 
		int nPos = csFullPath.ReverseFind( _T('\\') ); 
		if( nPos > 0 ) 
		{
			return csFullPath.Left( nPos ); 
		}			
		return CString(""); 
	}

	void CreateDir(CString path)
	{
		path.Replace(_T('/'), _T('\\'));
		int currentPos = 0;
		CString currentPath;
		while(true){
			int nPos = path.Find( _T('\\'), currentPos);
			if( nPos > 0 ) 
			{
				currentPos = nPos+1;
				currentPath = path.Left(nPos); 
			}else{
				break;
			}
			if (!currentPath.IsEmpty()&&!DirectoryExist(currentPath))
			{
				CreateDirectory(currentPath);
			}
		}
	}

	BOOL DirectoryExist(const CString&path)
	{
		WIN32_FIND_DATA fd;
		BOOL ret = FALSE;
		HANDLE hFind = FindFirstFile(path, &fd);
		if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//Ŀ¼����
			ret = TRUE;
		}
		FindClose(hFind);
		return ret;
	}

	BOOL CreateDirectory(const CString&path)
	{
		SECURITY_ATTRIBUTES attrib;
		attrib.bInheritHandle = FALSE;
		attrib.lpSecurityDescriptor = NULL;
		attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
		return ::CreateDirectory( path, &attrib);
	}

}