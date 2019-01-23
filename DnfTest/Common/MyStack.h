// MyStack.h: interface for the MyStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSTACK_H__EE1D4766_7149_479A_AE2B_85AEAECEAB8B__INCLUDED_)
#define AFX_MYSTACK_H__EE1D4766_7149_479A_AE2B_85AEAECEAB8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyList.h"

#define IN 
#define OUT 

template<typename T>
class MyStack:private MyList<T>
{
public:
  MyStack(){}
  virtual ~MyStack(){}
public:
  MyList<T>::PrintAll;
  MyList<T>::isEmpty;
public:
  //ÈëÕ»
  void push( IN const T& obj )
  {
      InsertElemTail(obj);
  }
  bool GetTail( OUT T& obj )
  {
    if ( m_nLength == 0 )
      return false;
    
    if ( m_lpTail != NULL )
      obj = m_lpTail->m_data;
    return true;
  }
  bool GetTailPrev( OUT T& obj )
  { 
    bool isRet = false;

    if ( m_nLength <= 1 )
      return isRet;
    
    if ( m_lpTail != NULL && 
         m_lpTail->m_lpPrev != NULL &&
         m_lpTail->m_lpPrev != m_lpHead )
    {
      obj = m_lpTail->m_lpPrev->m_data;
      isRet = true;
    }
    return isRet;
  }
  //³öÕ»
  bool pop( OUT T& obj )
  {
     if ( m_nLength == 0 )
      return false;

     if ( m_lpTail != NULL )
     {
       obj = m_lpTail->m_data;
       
       DeleteElem(m_lpTail);
     }
     return true;
  }
};

#endif // !defined(AFX_MYSTACK_H__EE1D4766_7149_479A_AE2B_85AEAECEAB8B__INCLUDED_)
