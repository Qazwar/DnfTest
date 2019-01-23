#include "StdAfx.h"
#include "WaitForEvent.h"

//ȫ��ֹͣ�¼�
HANDLE g_hStopEvent = NULL;


CWaitForEvent::CWaitForEvent(void)
{
}


CWaitForEvent::~CWaitForEvent(void)
{
}

void CWaitForEvent::CreateEventHandle()
{
	char	strEvent[50] = {0};
	int nPID = 0;
	nPID = GetCurrentProcessId();

	if (g_hStopEvent == NULL)
	{
		wsprintf(strEvent, "StopEvent_%d", nPID); // ����¼���
		g_hStopEvent = CreateEvent(NULL,TRUE,FALSE,strEvent);
	}
}

//�ָ�ȫ��ֹͣ�¼�
void CWaitForEvent::Start_StopEvent()
{
	ResetEvent(g_hStopEvent);
}

//ȫ��ֹͣ�¼�
void CWaitForEvent::Stop_StopEvent()
{
	SetEvent(g_hStopEvent);
}

bool CWaitForEvent::NeedRun(int nTimes)
{
	if (WaitForSingleObject(g_hStopEvent,nTimes) != WAIT_OBJECT_0)
	{
		return true;
	}
	return false;
}

void CWaitForEvent::CloseEventHandle()
{
	CloseHandle(g_hStopEvent);
	g_hStopEvent = NULL;
}