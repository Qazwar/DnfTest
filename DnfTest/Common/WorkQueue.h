#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H
#include <queue>
#include <Windows.h>

class CWorkQueue;


/*------------------------------------------------------------------------
WorkItemBase	

this is the basic WorkItem that the Work Queue Use its interface
This class should be inherited and these virtual abstract functions 
implemented.

  DoWork()

  virtual abstract function is the function that is called when the
  work item turn has came to be poped out of the queue and be processed.


  Abort ()

  This function is called, when the Destroy function is called, for each of the WorkItems
  That are left in the queue.

  
------------------------------------------------------------------------*/
class WorkItemBase
{
   virtual void   DoWork(void* pThreadContext)    = 0;
   virtual void   Abort () = 0;
   friend CWorkQueue;
};


typedef std::queue<WorkItemBase*>           WorkItemQueue,*PWorkItemQueue;


/*------------------------------------------------------------------------
CWorkQueue  

  ����һ�����������࣬Ҳ�����̳߳أ��������Ҫ�Ǵ����̶߳��У�Ȼ��ȴ���ֱ��
  �й�����Ŀ������������ͻ�����Ӧ���߳�ִ����Ӧ������Ŀ�Ĵ��룬Ȼ���������
  �ȴ�״̬
------------------------------------------------------------------------*/

class  CWorkQueue 
{
public:

   virtual ~CWorkQueue(){};
   
   bool Create(const unsigned int		nNumberOfThreads, 
               void*					*pThreadDataArray             = NULL);
   
   bool InsertWorkItem(WorkItemBase* pWorkItem);
   
   void Destroy();
   

private:
   
   static unsigned long __stdcall ThreadFunc( void* pParam );
   WorkItemBase* RemoveWorkItem();
   
   
   enum{
      ABORT_EVENT_INDEX = 0,
      SEMAPHORE_INDEX,
      NUMBER_OF_SYNC_OBJ,
   };

   
   unsigned int				m_nNumberOfThreads;
   void*					m_pThreadDataArray;
   
   HANDLE					m_phSincObjectsArray[NUMBER_OF_SYNC_OBJ];

   CRITICAL_SECTION			m_CriticalSection;	
   PWorkItemQueue           m_pWorkItemQueue;
};
extern PHANDLE				m_phThreads;

//extern BOOL _bwork;

#endif // WORK_QUEUE_H
