// Buffer.h: interface for the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_)
#define AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_

#include <Windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CServerBuffer  
{
// Attributes
protected:
	PBYTE	m_pBase;
	PBYTE	m_pPtr;
	UINT	m_nSize;


// Methods
protected:
	UINT ReAllocateBuffer(UINT nRequestedSize);
	UINT DeAllocateBuffer(UINT nRequestedSize);
	UINT GetMemSize();
public:
	void ClearBuffer();

	UINT Delete(UINT nSize);
	UINT Read(PBYTE pData, UINT nSize);
	BOOL Write(PBYTE pData, UINT nSize);
	//BOOL Write(CString& strData);
	UINT GetBufferLen();
	int Scan(PBYTE pScan,UINT nPos);
	BOOL Insert(PBYTE pData, UINT nSize);
	//BOOL Insert(CString& strData);

	void Copy(CServerBuffer& buffer);	

	PBYTE GetBuffer(UINT nPos=0);

	CServerBuffer();
	virtual ~CServerBuffer();

	//void FileWrite(const CString& strFileName);

};

class CClientBuffer
{
	// Attributes
protected:
	PBYTE	m_pBase;
	PBYTE	m_pPtr;
	UINT	m_nSize;


	// Methods
protected:
	UINT ReAllocateBuffer(UINT nRequestedSize);
	UINT DeAllocateBuffer(UINT nRequestedSize);
	UINT GetMemSize();
public:
	void ClearBuffer();

	UINT Delete(UINT nSize);
	UINT Read(PBYTE pData, UINT nSize);
	BOOL Write(PBYTE pData, UINT nSize);
	UINT GetBufferLen();
	int Scan(PBYTE pScan,UINT nPos);
	BOOL Insert(PBYTE pData, UINT nSize);

	void Copy(CClientBuffer& buffer);	

	PBYTE GetBuffer(UINT nPos=0);

	CClientBuffer();
	virtual ~CClientBuffer();
private:
	CRITICAL_SECTION m_cs;
};

#endif // !defined(AFX_BUFFER_H__829F6693_AC4D_11D2_8C37_00600877E420__INCLUDED_)
