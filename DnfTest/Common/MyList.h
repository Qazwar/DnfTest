// MyList.h: interface for the MyList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLIST_H__E5C268FE_41AB_457D_BBAC_41D5EEE2526E__INCLUDED_)
#define AFX_MYLIST_H__E5C268FE_41AB_457D_BBAC_41D5EEE2526E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <iostream>
#include "Common.h"

template< typename T >
class Node
{
public:
  Node( const T& stu = 0 )
    :m_data(stu)
  {
    m_lpNext = NULL;
    m_lpPrev = NULL;
  }
  ~Node()
  {
  }
  T m_data;
  Node<T>*   m_lpPrev;
  Node<T>*   m_lpNext;
};


//带表头结点
template< typename T >
class MyList  
{
public:
  class iterator
  {
  public:
    iterator( Node<T>* lpCurNode )
    {
      m_lpCurNode = lpCurNode;
    }
    iterator operator++(int)
    {
      iterator temp(*this);
      m_lpCurNode = m_lpCurNode->m_lpNext;
      return *this;
    }
    T& operator*()
    {
      return m_lpCurNode->m_data;
    }
    bool operator!=( const iterator& obj)
    {
      if ( obj.m_lpCurNode != NULL)
        return !(m_lpCurNode->m_data == obj.m_lpCurNode->m_data);
      else
        return m_lpCurNode != obj.m_lpCurNode;
    }
	public:
    Node<T>* m_lpCurNode;
  };
public:
  MyList( int n = 0 );
  virtual ~MyList();
public:
  iterator begin() const { return iterator(m_lpHead->m_lpNext); }
  iterator end()  const  { return iterator(NULL); }
  MyList( const MyList& obj) 
  {
    
    m_lpTail = m_lpHead = new Node<T>();
    m_nLength = 0;

    for ( iterator it = obj.begin(); it != obj.end(); it++ )
    {
      InsertElemTail(*it);
    }
  }
  MyList& operator= (const MyList& obj) 
  {
    if ( this == &obj )
      return *this;
    
    Clear();

    if ( m_lpHead  != NULL )
    {
      delete m_lpHead;
      m_lpHead  = m_lpTail = NULL;
    }

    m_lpTail = m_lpHead = new Node<T>();
    m_nLength = 0;
    
    for ( iterator it = obj.begin(); it != obj.end(); it++ )
      InsertElemTail(*it);

    return *this;
  }
  bool operator==( const MyList& obj ) const 
  {
    return (  m_lpHead == obj.m_lpHead &&
              m_lpTail == obj.m_lpTail &&
              m_nLength == obj.m_nLength);
  }
public:
  bool GetElem(int nIndex, T& OutStu);
  bool PriorElem(const T& stu,T& OutPreStu);
  void InsertElem(int nIndex, const T& InStu); //之前
  void InsertElemTail(const T& InStu);
  bool DeleteElem(int nIndex);
  bool DeleteElem(const T& InStu);
  bool DeleteElem(Node<T>* lpDelNode);
  void Clear();
  void PrintAll();
  bool isEmpty() const 
  {
    return m_nLength == 0;
  }
  int  GetLength() const
  {
    return m_nLength;
  }
  Node<T>* GetNode(const T& obj) const ;
  bool isFind( const T& obj )
  {
    return (GetNode(obj)!=NULL);
  } 
public:
  
  //双链表交换指针域
  void Swap(Node<T>* lpNode1,Node<T>* lpNode2);
  
  void Swap(const T& obj1,const T& obj2);
  
  void Sort();   
  
  public:
  Node<T>* m_lpHead;  //头指针
  Node<T>* m_lpTail;  //尾指针
  int   m_nLength;
};


template< typename T >
MyList<T>::MyList( int n )
{
  //表头结点
  Node<T>* lpTableHead = new Node<T>();

  m_lpHead  = m_lpTail = lpTableHead;
  m_nLength = 0;
}

template< typename T >
MyList<T>::~MyList()
{
  Clear();
  if ( m_lpHead != NULL )
    delete m_lpHead;
  m_lpHead  = m_lpTail = NULL;
  m_nLength = 0;
}
template< typename T >
bool MyList<T>::GetElem(int nIndex, T& OutStu)
{

  if ( nIndex <0 || nIndex > m_nLength-1 )
    throw "越界访问";
  
  Node<T>* lpNode = m_lpHead;
  
  for ( int i = 0 ; i<= nIndex ; i++ )
  {
    lpNode = lpNode->m_lpNext;
  }

  OutStu = lpNode->m_data;

  return true;
}
template< typename T >
bool MyList<T>::PriorElem(const T& stu,T& OutPreStu)
{
  Node<T>* lpNodePrev = m_lpHead; 
  Node<T>* lpNode = m_lpHead->m_lpNext;
  
  while ( lpNode )
  {
    if ( lpNode->m_data == stu )
    {
      OutPreStu = lpNodePrev->m_data;
      break;
    }
    lpNodePrev = lpNode;
    lpNode = lpNode->m_lpNext;
  }

  return true;
}
template< typename T >
void MyList<T>::InsertElem(int nIndex, const T& InStu)
{
  if ( nIndex <0 || nIndex > m_nLength-1 )
    throw "越界访问";

  Node<T>* lpNodePre = m_lpHead;
  Node<T>* lpNode    = m_lpHead;

  for ( int i = 0 ; i<= nIndex ; i++ )
  {
     lpNodePre = lpNode;
     lpNode = lpNode->m_lpNext;
  }
  
  //lpNode = &a[nIndex]

  //head headNode<T> 1 2 [3] 4 5 

  //A new InStu;
  //B 2.next = InStu;  InStu.Prev = 2;
  //C InStu.next = 3;  3.Prev = Instu;

  Node<T>* lpNewNode<T> = new Node<T>(InStu);
  
  if ( lpNewNode<T> == NULL )
    throw "new error";

  if ( lpNodePre!= NULL )
    lpNodePre->m_lpNext = lpNewNode<T>;

  if ( lpNewNode<T> != NULL )
    lpNewNode<T>->m_lpPrev = lpNodePre;


  if ( lpNewNode<T>!= NULL )
    lpNewNode<T>->m_lpNext = lpNode;

  if ( lpNode != NULL )
    lpNode->m_lpPrev = lpNewNode<T>;

  m_nLength++;
}
template< typename T >
void MyList<T>::InsertElemTail(const T& InStu)
{
  //head headNode<T> 1 2 3        [newNode<T>]
  Node<T>* lpNewNode = new Node<T>(InStu);

  if ( lpNewNode == NULL )
    throw "new error";

  //3.next = newNode
  if ( m_lpTail != NULL )
    m_lpTail->m_lpNext = lpNewNode;

  //newNode.prev = 3;
  if ( lpNewNode != NULL )
    lpNewNode->m_lpPrev = m_lpTail;

  m_lpTail = lpNewNode;

  m_nLength++;
}

template< typename T >
bool MyList<T>::DeleteElem( Node<T>* lpNode )
{
  if ( lpNode == NULL || lpNode == m_lpHead)
    return false;

  Node<T>* lpNodePre = lpNode->m_lpPrev;

  //1 lpNodePre  2 lpNode  3 lpNode->m_lpNext
  
  Node<T>* lpNodeNext = NULL;
  
  //head headNode<T> 1 2 [3]
  
  //3
  if ( lpNode != NULL )
    lpNodeNext = lpNode->m_lpNext;
  
  //1.next = 3
  if ( lpNodePre != NULL )
    lpNodePre->m_lpNext = lpNodeNext;   //file
  
  //删除尾部元素的处理
  if ( lpNodeNext == NULL )
    m_lpTail = lpNodePre;
  else
    lpNodeNext->m_lpPrev = lpNodePre;
  
  //delete 2
  if ( lpNode != NULL )
  {
    delete lpNode;              //file
    lpNode = NULL;
  }
  
  m_nLength--;                  //file  
  
  return true;
}

template< typename T >
bool MyList<T>::DeleteElem(int nIndex)
{
  //head headNode<T> 1 [2] 3

  if ( nIndex <0 || nIndex > m_nLength-1 )
    throw "越界访问";
 
  Node<T>* lpNode    = m_lpHead;
  
  for ( int i = 0 ; i<= nIndex ; i++ )
  {
    lpNode = lpNode->m_lpNext;
  }

  return DeleteElem(lpNode);
}


template< typename T >
bool MyList<T>::DeleteElem(const T& InStu)
{ 
  Node<T>* lpNode    = m_lpHead->m_lpNext;
  
  while ( lpNode )
  {
    if ( lpNode->m_data == InStu )
      break;
    
    lpNode = lpNode->m_lpNext;
  }
  
  if ( lpNode != NULL )
    return DeleteElem(lpNode);
  else
    return false;
}

//清空a[0]...a[n-1]
template< typename T >
void MyList<T>::Clear()
{
  //head headNode<T> 1 2 3
  Node<T>* lpNode = m_lpHead->m_lpNext;
  
  while ( lpNode )
  {
    Node<T>* lpTemp = lpNode->m_lpNext;

    delete lpNode;
    
    lpNode = lpTemp;
  }
  m_lpTail = m_lpHead;
  m_nLength = 0;
}

template< typename T >
void MyList<T>::PrintAll()
{
  //取得a[0]的位置
  Node<T>* lpNode = m_lpHead->m_lpNext;


  //Output("[%s]",lpNode->m_data);
  //lpNode = lpNode->m_lpNext;
  while ( lpNode )
  {

	  Output("[%s]",lpNode->m_data);
     //cout << lpNode->m_data;

	 if ( lpNode->m_lpPrev == NULL )
		 Output("[NULL]");
       //out "[NULL]";
	 else
		 Output("[%s]",lpNode->m_lpPrev->m_data);
       //cout << "[" << lpNode->m_lpPrev->m_data << "]";
     
	 if ( lpNode->m_lpNext == NULL )
		 Output("[NULL]");
       //cout << "[NULL]";  
	 else
		 Output("[%s]",lpNode->m_lpNext->m_data);
       //cout << "[" << lpNode->m_lpNext->m_data << "]";

	 Output(" ");

     //&a[i] ===> &a[i+1]
     lpNode = lpNode->m_lpNext;
  }

  cout << endl;
}

//双链表交换指针域
template< typename T >
void MyList<T>::Swap(Node<T>* lpNode1,Node<T>* lpNode2)
{
   //先一般  再特殊
  if ( lpNode1 == NULL || lpNode2 == NULL || lpNode1 == lpNode2 )
    return;

  // 1 [2] 3 4 [5] 6 

  //1
  Node<T>* lpNode1Prev = lpNode1->m_lpPrev;
  Node<T>* lpNode1Next = lpNode1->m_lpNext;
  Node<T>* lpNode2Prev = lpNode2->m_lpPrev;
  Node<T>* lpNode2Next = lpNode2->m_lpNext;

  //1.next = 5  5.prev = 1;
  if (lpNode1Prev != lpNode2 )
  {
    if ( lpNode1Prev != NULL )
      lpNode1Prev->m_lpNext = lpNode2;  ///?
  }
  if ( lpNode2 != lpNode1Prev)
    lpNode2->m_lpPrev = lpNode1Prev;
  else
    lpNode2->m_lpPrev = lpNode1;

  //5.next = 3  3.prev = 5
  if ( lpNode1Next != lpNode2)
    lpNode2->m_lpNext = lpNode1Next;
  else
    lpNode2->m_lpNext = lpNode1;

  if ( lpNode1Next != NULL )
  {
    if ( lpNode1Next != lpNode2)
      lpNode1Next->m_lpPrev = lpNode2;
  }
  else
     m_lpTail = lpNode2;

  //4.next = 2  2.prev = 4
  if ( lpNode2Prev != lpNode1)
    lpNode1->m_lpPrev = lpNode2Prev;
  else
    lpNode1->m_lpPrev = lpNode2;

  if ( lpNode2Prev != NULL )
  {
    if ( lpNode2Prev != lpNode1)
      lpNode2Prev->m_lpNext = lpNode1;
  }
  
  //2.next = 6  6.prev = 2
  if ( lpNode1 != lpNode2Next )
    lpNode1->m_lpNext = lpNode2Next;
  else
    lpNode1->m_lpNext = lpNode2;

  if ( lpNode2Next != lpNode1 )
  {
    if ( lpNode2Next != NULL )
      lpNode2Next->m_lpPrev = lpNode1;
  }
  else
    m_lpTail = lpNode1;
}

template< typename T >
Node<T>* MyList<T>::GetNode(const T& obj) const
{
  Node<T>* lpNode = m_lpHead->m_lpNext;
  
  while ( lpNode )
  {
    if ( lpNode->m_data == obj )
      return lpNode;
    lpNode = lpNode->m_lpNext;
  }
  
  return NULL;
}

template< typename T >
void MyList<T>::Swap(const T& obj1,
                  const T& obj2)
{
    Node<T>* lpNode1 = GetNode<T>(obj1);
    Node<T>* lpNode2 = GetNode<T>(obj2);
    Swap(lpNode1,lpNode2);
}

template< typename T >
void MyList<T>::Sort()
{
  if ( m_lpHead == NULL )
    return;
  
  Node<T>* lpCurNode1 = m_lpHead->m_lpNext;
  
  Node<T>* lpCurNode2 = NULL;
  
  while ( lpCurNode1 )
  {
    lpCurNode2 = lpCurNode1->m_lpNext;
    
    while ( lpCurNode2 )
    {
      Node<T>* lpNext = lpCurNode2->m_lpNext;
      
      if ( lpCurNode1->m_data > lpCurNode2->m_data )
      {
        Swap(lpCurNode1,lpCurNode2);
        
        lpCurNode1 = lpCurNode2;
      }
      
      lpCurNode2 = lpNext;
    }
    
    lpCurNode1 = lpCurNode1->m_lpNext;
  }
}

#endif // !defined(AFX_MYLIST_H__E5C268FE_41AB_457D_BBAC_41D5EEE2526E__INCLUDED_)
