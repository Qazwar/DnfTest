#include "StdAfx.h"
#include "WaitForEvent.h"

//全局停止事件
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
		wsprintf(strEvent, "StopEvent_%d", nPID); // 随机事件名
		g_hStopEvent = CreateEvent(NULL,TRUE,FALSE,strEvent);
	}
}

//恢复全局停止事件
void CWaitForEvent::Start_StopEvent()
{
	ResetEvent(g_hStopEvent);
}

//全局停止事件
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