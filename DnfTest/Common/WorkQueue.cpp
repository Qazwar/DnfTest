#include "stdafx.h"
#include "WorkQueue.h"
#include <assert.h>

//BOOL _bwork=FALSE;
typedef struct _THREAD_CONTEXT
{
   CWorkQueue* pWorkQueue;
   void*       pThreadData;
} THREAD_CONTEXT,*PTHREAD_CONTEXT;


PHANDLE				m_phThreads=NULL;

/*------------------------------------------------------------------------
Create

  ��ʼ����������
  const unsigned int  nNumberOfThreads      - Ҫ�����Ĺ����������̵߳���Ŀ
  void*         *ThreadData                 - �͸��̵߳���Ҫִ�еĹ�����Ŀ
		
------------------------------------------------------------------------*/

bool CWorkQueue::Create(const unsigned int  nNumberOfThreads, 
                              void*         *ThreadData      /*=NULL*/)
{
      
   //��ʼ����������
   m_pWorkItemQueue = new WorkItemQueue();
   
   if(NULL == m_pWorkItemQueue )
   {      
      return false;
   }
   
   //����Semaphore����   
   m_phSincObjectsArray[SEMAPHORE_INDEX] = CreateSemaphore(NULL,0,LONG_MAX,NULL);
   
   if(m_phSincObjectsArray[SEMAPHORE_INDEX] == NULL)
   {      
	   delete m_pWorkItemQueue;
	   m_pWorkItemQueue = NULL;
	   return false;
   }
   
   //����event �¼�����
   m_phSincObjectsArray[ABORT_EVENT_INDEX] = CreateEvent(NULL,TRUE,FALSE,NULL);

   
   if(m_phSincObjectsArray[ABORT_EVENT_INDEX]  == NULL)
   {      
      delete m_pWorkItemQueue;
      m_pWorkItemQueue = NULL;
      CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
      return false;
   }
   

   //�����ٽ����Ա�����������
   InitializeCriticalSection(&m_CriticalSection);

   //�����߳̾������
   m_phThreads = new HANDLE[nNumberOfThreads];

   if(m_phThreads == NULL)
   {    
      delete m_pWorkItemQueue;
      m_pWorkItemQueue = NULL;
      CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
	  CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
	  DeleteCriticalSection(&m_CriticalSection);		  
      return false;
   }

   unsigned int i;

   m_nNumberOfThreads = nNumberOfThreads;

   DWORD dwThreadId;
   PTHREAD_CONTEXT pThreadsContext ;
 
   //�������е��߳�
   for(i = 0 ; i < nNumberOfThreads ; i++ )
   {  
	   //��ʼ��ÿ���̵߳������ģ����ڴ��ݸ��̺߳���
	  pThreadsContext = new THREAD_CONTEXT;
	  pThreadsContext->pWorkQueue  = this;
	  pThreadsContext->pThreadData = ThreadData == NULL? NULL : ThreadData[i];	   
	  //�����߳�
	  m_phThreads[i] = CreateThread(NULL,
		  0,
		  CWorkQueue::ThreadFunc,
		  pThreadsContext,
		  0,
		  &dwThreadId);

      if(m_phThreads[i] == NULL)
      {  
		 delete pThreadsContext;
         m_nNumberOfThreads = i;
         Destroy();
         return false;
      }
   }

   return true;
}


/*------------------------------------------------------------------------
InsertWorkItem

  ���빤�����񵽹�������
------------------------------------------------------------------------*/
bool CWorkQueue::InsertWorkItem(WorkItemBase* pWorkItem)
{

   assert(pWorkItem != NULL);
   //��ס
   EnterCriticalSection(&m_CriticalSection);
   //�������
   m_pWorkItemQueue->push(pWorkItem);
   //����
   LeaveCriticalSection(&m_CriticalSection); 
   //�����߳�
   if (!ReleaseSemaphore(m_phSincObjectsArray[SEMAPHORE_INDEX],1,NULL)) 
   { 
      assert(false);
      return false;     
   } 
   
 return true;
 
}

/*------------------------------------------------------------------------
RemoveWorkItem()

 �ӹ���������ȡ�����񣬲����Ƴ�
------------------------------------------------------------------------*/
WorkItemBase*  CWorkQueue::RemoveWorkItem()
{
   
   WorkItemBase* pWorkItem;
   //��ס
   EnterCriticalSection(&m_CriticalSection);  
   //�Ӷ������Ƴ�����  
   pWorkItem = m_pWorkItemQueue->front();
   m_pWorkItemQueue->pop();
   //����
   LeaveCriticalSection(&m_CriticalSection);
   assert(pWorkItem != NULL);
   return pWorkItem;
}
/*------------------------------------------------------------------------
ThreadFunc

  �����̺߳����������ȴ����������¼��ĵ��������뿪�߳��¼��Ĵ���

------------------------------------------------------------------------*/
unsigned long __stdcall CWorkQueue::ThreadFunc( void*  pParam )
{ 
	try
	{
	
	   PTHREAD_CONTEXT       pThreadContext =  (PTHREAD_CONTEXT)pParam;//�̵߳Ĵ������ 
	   WorkItemBase*         pWorkItem      = NULL;
	   CWorkQueue*           pWorkQueue     = pThreadContext->pWorkQueue;//��������ָ��
	   void*                 pThreadData    = pThreadContext->pThreadData;
	   DWORD dwWaitResult; 
	   for(;;)
	   {
		  //�ȴ������¼�
		  dwWaitResult = WaitForMultipleObjects(NUMBER_OF_SYNC_OBJ,pWorkQueue->m_phSincObjectsArray,FALSE,INFINITE);

		  switch(dwWaitResult - WAIT_OBJECT_0)
		  {

		  case ABORT_EVENT_INDEX: //�뿪�߳��¼�
			 delete pThreadContext;
			 return 0; 
		  case SEMAPHORE_INDEX://���������¼�
			 //�õ��������еĵ�һ����������
			 pWorkItem = pWorkQueue->RemoveWorkItem();     
			 if(pWorkItem == NULL)
			 {
				assert(false);
				break;
			 }     
			 //������Ӧ�Ĺ�������
			 pWorkItem->DoWork(pThreadData);     
			 break;
		  default:
			  assert(false);
			  delete pThreadContext;
			  return 0; 

		  }      
	   }
	   delete pThreadContext;
	   return 1; 
	}
	catch (...)
	{ 	return 0;  }
	
}


/*------------------------------------------------------------------------
Destroy	

  �����߳��˳��¼����ȴ������̵߳Ľ���
------------------------------------------------------------------------*/
void CWorkQueue::Destroy()
{
  //�����˳��¼�	   
   if(!SetEvent(m_phSincObjectsArray[ABORT_EVENT_INDEX]))
   {     
      //assert(false);
      return;
   }
   //�ȴ����е��߳̽���
   WaitForMultipleObjects(m_nNumberOfThreads,m_phThreads,true,INFINITE);
         
   //�������
   while(!m_pWorkItemQueue->empty())
   {
      m_pWorkItemQueue->front()->Abort();
	  m_pWorkItemQueue->pop();
   }  
   delete m_pWorkItemQueue;
   m_pWorkItemQueue = NULL;   
   CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
   CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
   DeleteCriticalSection(&m_CriticalSection);
   //�ر����е��߳̾��
   for(int i = 0 ; i < (int)m_nNumberOfThreads ; i++)
	   CloseHandle(m_phThreads[i]);
   delete[] m_phThreads;
}