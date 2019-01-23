// MyQueue.h: interface for the MyQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYQUEUE_H__BAF3D453_11DB_4216_A28D_0B34AA3F6D84__INCLUDED_)
#define AFX_MYQUEUE_H__BAF3D453_11DB_4216_A28D_0B34AA3F6D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyList.h"

#define IN 
#define OUT 

template<typename T>
class MyQueue:private MyList<T>
{
public:
  MyQueue(){}
  virtual ~MyQueue(){}
public:
  MyList<T>::PrintAll;
  MyList<T>::isEmpty;
public:
  void EnQueue( IN const T& obj )
  {
    InsertElemTail(obj);
  }
  bool DeQueue( OUT T& obj )
  {
    if ( m_nLength == 0 )
      return false;
    
    if ( m_lpHead->m_lpNext != NULL )
    {
      obj = m_lpHead->m_lpNext->m_data;
      
      DeleteElem(m_lpHead->m_lpNext);
    }
     return true;
  }
};

#endif // !defined(AFX_MYQUEUE_H__BAF3D453_11DB_4216_A28D_0B34AA3F6D84__INCLUDED_)
