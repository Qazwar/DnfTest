#include "stdafx.h"
#include "RWLockImpl.h"

CRWLockImpl::CRWLockImpl(): m_readers(0), m_writersWaiting(0), m_writers(0)
{
	m_mutex = CreateMutex(NULL, FALSE, NULL);
	if (m_mutex == NULL)
		OutputDebugString("cannot ¡¾create¡¿ reader/writer lock");

	m_readEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (m_readEvent == NULL)
		OutputDebugString("cannot ¡¾create¡¿ reader/writer lock");

	m_writeEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
	if (m_writeEvent == NULL)
		OutputDebugString("cannot ¡¾create¡¿ reader/writer lock");
}

CRWLockImpl::~CRWLockImpl()
{
	CloseHandle(m_mutex);
	CloseHandle(m_readEvent);
	CloseHandle(m_writeEvent);
}

inline void CRWLockImpl::AddWriter()
{
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (++m_writersWaiting == 1) 
			ResetEvent(m_readEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		OutputDebugString("cannot lock reader/writer lock 1");
	}
}

inline void CRWLockImpl::RemoveWriter()
{
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		if (--m_writersWaiting == 0 && m_writers == 0) 
			SetEvent(m_readEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		OutputDebugString("cannot lock reader/writer lock 2");
	}
}

void CRWLockImpl::ReadLockImpl()
{
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_readEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_readers;
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		assert(m_writers == 0);
		break;
	default:
		OutputDebugString("cannot lock reader/writer lock 3");
	}
}

bool CRWLockImpl::TryReadLockImpl()
{
	for (;;)
	{
		if (m_writers != 0 || m_writersWaiting != 0)
			return false;

		DWORD result = TryReadLockOnce();
		switch (result)
		{
		case WAIT_OBJECT_0:
		case WAIT_OBJECT_0 + 1:
			return true;
		case WAIT_TIMEOUT:
			continue;
		default:
			OutputDebugString("cannot lock reader/writer lock 4");
		}
	}
}

void CRWLockImpl::WriteLockImpl()
{
	AddWriter();
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, INFINITE))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_writersWaiting;
		++m_readers;
		++m_writers;
		ResetEvent(m_readEvent);
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		assert(m_writers == 1);
		break;
	default:
		RemoveWriter();
		OutputDebugString("cannot lock reader/writer lock 5");
	}
}

bool CRWLockImpl::TryWriteLockImpl()
{
	AddWriter();
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_writeEvent;
	switch (WaitForMultipleObjects(2, h, TRUE, 1))
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		--m_writersWaiting;
		++m_readers;
		++m_writers;
		ResetEvent(m_readEvent);
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		assert(m_writers == 1);
		return true;
	case WAIT_TIMEOUT:
		RemoveWriter();
	default:
		RemoveWriter();
		OutputDebugString("cannot lock reader/writer lock 6");
	}
	return false;
}

void CRWLockImpl::UnlockImpl()
{
	switch (WaitForSingleObject(m_mutex, INFINITE))
	{
	case WAIT_OBJECT_0:
		m_writers = 0;
		if (m_writersWaiting == 0) SetEvent(m_readEvent);
		if (--m_readers == 0) SetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		break;
	default:
		OutputDebugString("cannot unlock reader/writer lock");
	}
}

DWORD CRWLockImpl::TryReadLockOnce()
{
	HANDLE h[2];
	h[0] = m_mutex;
	h[1] = m_readEvent;
	DWORD result = WaitForMultipleObjects(2, h, TRUE, 1); 
	switch (result)
	{
	case WAIT_OBJECT_0:
	case WAIT_OBJECT_0 + 1:
		++m_readers;
		ResetEvent(m_writeEvent);
		ReleaseMutex(m_mutex);
		assert(m_writers == 0);
		return result;
	case WAIT_TIMEOUT:
	default:
		OutputDebugString("cannot lock reader/writer lock 7");
	}
	return result;
}