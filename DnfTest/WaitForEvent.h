#pragma once
#include "singleton.h"

class CWaitForEvent : public Pattern::Singleton<CWaitForEvent>
{
public:
	CWaitForEvent(void);
	~CWaitForEvent(void);
public:
	//创建事件
	void CreateEventHandle();
	void CloseEventHandle();

	//恢复全局停止事件
	void Start_StopEvent();
	//全局停止事件
	void Stop_StopEvent();

	bool NeedRun(int nTimes);
};

extern HANDLE g_hStopEvent;

#define WAIT_STOP_RETURN_VALUE(WAITTIME,RETURNVALUE)\
	if (WaitForSingleObject(g_hStopEvent,WAITTIME) == WAIT_OBJECT_0)\
{\
	return RETURNVALUE;\
}

#define WAIT_STOP_RETURN(WAITTIME)\
	if (WaitForSingleObject(g_hStopEvent,WAITTIME) == WAIT_OBJECT_0)\
{\
	return;\
}

#define WAIT_STOP_BREAK(WAITTIME)\
	if (WaitForSingleObject(g_hStopEvent,WAITTIME) == WAIT_OBJECT_0)\
{\
	break;\
}

#define WAIT_SLEEP(WAITTIME)\
	if (WaitForSingleObject(g_hStopEvent,WAITTIME) == WAIT_OBJECT_0)\
{}

