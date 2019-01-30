#include "StdAfx.h"
#include "DD.h"

CDD::CDD(void)
{
	m_hModule = NULL;
	DD_btn = NULL;
	DD_whl = NULL;
	DD_key = NULL;
	DD_mov = NULL;
	DD_str = NULL;
	DD_todc = NULL;
	DD_movR = NULL;
}

CDD::~CDD(void)
{
	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
	}
}

int CDD::GetFunAddr(CString dllfile)
{
	if ( -1 == ::GetFileAttributes(dllfile))
	{
		return -11; //�ļ�������
	}
	if(m_hModule!=NULL){
		return 0;
	}
	m_hModule = ::LoadLibrary(dllfile);

	if ( m_hModule==NULL)
	{
		return -12; // ��������
	}

	// ����ʹ��˵�� g or b
	DD_btn = (pDD_btn )GetProcAddress(m_hModule,"DD_btn"); 
	DD_whl = (pDD_whl )GetProcAddress(m_hModule,"DD_whl"); 
	DD_key = (pDD_key )GetProcAddress(m_hModule,"DD_key"); 
	DD_mov = (pDD_mov)GetProcAddress(m_hModule,"DD_mov"); 
	DD_str  = (pDD_str)GetProcAddress(m_hModule,"DD_str"); 
	DD_todc  = (pDD_todc)GetProcAddress(m_hModule,"DD_todc"); 
	DD_movR = (pDD_movR)GetProcAddress(m_hModule,"DD_movR"); 

	//�ж�һ�£��Ƿ���ȷ��ȡ�˺�����ַ
	if ( DD_btn && DD_whl && DD_key && DD_mov && DD_str  && DD_todc && DD_movR)
	{
		return 1; // �ɹ�
	}
	else
	{
		return -13; // ��ȡ������ַ����
	}
}

