#pragma once
//�ļ�������
class CFileParser
{
public:
	CFileParser(void);
	~CFileParser(void);
	//����txt�ı��ļ�
	void LoadData(const CString& path);
	//���ɱ����ļ�
	void SaveData();
};

