#ifndef _RWLockImpl_Header
#define _RWLockImpl_Header

#include <assert.h>
#include <iostream>
#include <Windows.h>
#include <process.h>

using namespace std;

/*
 ��д������ǰ�Ķ�����û����ʱ�����Դ����ֻ����һ��д���߷��ʱ�����Դ
*/

//-----------------------------------------------------------------
class CRWLockImpl
{
protected:
	CRWLockImpl();
	~CRWLockImpl();
	void ReadLockImpl();
	bool TryReadLockImpl();
	void WriteLockImpl();
	bool TryWriteLockImpl();
	void UnlockImpl();

private:
	void AddWriter();
	void RemoveWriter();
	DWORD TryReadLockOnce();

	HANDLE   m_mutex;
	HANDLE   m_readEvent;
	HANDLE   m_writeEvent;
	unsigned m_readers;
	unsigned m_writersWaiting;
	unsigned m_writers;
};

//-----------------------------------------------------------------

class CMyRWLock: private CRWLockImpl
{
public:

	//������/д��
	CMyRWLock(){};

	//���ٶ�/д��
	~CMyRWLock(){};

	//��ȡ����
	//�������һ���߳�ռ��д������ǰ�̱߳���ȴ�д�����ͷţ����ܶԱ�����Դ���з���
	void ReadLock();

	//���Ի�ȡһ������
	//�����ȡ�ɹ�������������true��������һ���߳�ռ��д�����򷵻�false
	bool TryReadLock();

	//��ȡд��
	//���һ��������߳�ռ�ж����������ȴ����������ͷ�
	//�����ͬ��һ���߳��Ѿ�ռ��һ��������д�����򷵻ؽ����ȷ��
	void WriteLock();

	//���Ի�ȡһ��д��
	//�����ȡ�ɹ�������������true������һ������������߳�ռ�ж���������false
	//�����ͬ��һ���߳��Ѿ�ռ��һ��������д�����򷵻ؽ����ȷ��
	bool TryWriteLock();

	//�ͷ�һ��������д��
	void Unlock();

private:
	CMyRWLock(const CMyRWLock&);
	CMyRWLock& operator = (const CMyRWLock&);
};

inline void CMyRWLock::ReadLock()
{
	ReadLockImpl();
}

inline bool CMyRWLock::TryReadLock()
{
	return TryReadLockImpl();
}

inline void CMyRWLock::WriteLock()
{
	WriteLockImpl();
}

inline bool CMyRWLock::TryWriteLock()
{
	return TryWriteLockImpl();
}

inline void CMyRWLock::Unlock()
{
	UnlockImpl();
}


#endif