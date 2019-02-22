#pragma once
//文件解析器
class CFileParser
{
public:
	CFileParser(void);
	~CFileParser(void);
	//加载txt文本文件
	void LoadData(const CString& path);
	//生成报表文件
	void SaveData();
};

