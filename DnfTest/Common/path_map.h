// 图1.cpp : Defines the entry point for the console application.
//

#include "MyQueue.h"
#include "MyStack.h"
#include <string.h>
#include <list>
#include "Common.h"

//邻接表 表示图
/*
见 图.bmp
*/
class Edge;

class vertex
{
public:
	vertex( const char* lpName = "" )
	{
		m_lpName = lpName;
		m_cost   = 0;
		m_costsource = NULL;
	}  
	vertex( const vertex& obj  )
		:m_EdgeList(obj.m_EdgeList)
	{
		m_lpName = obj.m_lpName;
		m_cost   = obj.m_cost;
		m_costsource = obj.m_costsource;
	} 
	vertex operator=( const vertex& obj  )
	{
		if ( this == &obj )
			return *this;

		m_EdgeList = obj.m_EdgeList;
		m_lpName   = obj.m_lpName;
		m_cost     = obj.m_cost;
		m_costsource = obj.m_costsource;
		return *this;
	}
	bool operator==( const vertex& obj ) const 
	{
		return ( strcmp(m_lpName,obj.m_lpName) == 0 );
	}
	const char*    m_lpName;    //名称
	MyList<Edge>   m_EdgeList;  //初度边集

	///////////////////////
	MyList<char*>  m_sPath;

	int            m_cost;     //到达此顶点的最小成本
	vertex*        m_costsource;
};

class Edge
{
public:
	Edge( vertex* lpSrcVertex = NULL,
		vertex* lpDestVertex = NULL,
		int nPower = 0)
	{
		m_lpSrcVertex  = lpSrcVertex;
		m_lpDestVertex = lpDestVertex;
		m_nPower = nPower;
	}
	bool operator==( const Edge& obj ) const 
	{
		return (m_lpSrcVertex == obj.m_lpSrcVertex &&
			m_lpDestVertex == obj.m_lpDestVertex &&
			m_nPower == obj.m_nPower);
	}
	bool operator<( const Edge& obj ) const 
	{
		return (m_nPower < obj.m_nPower);
	}
	bool operator>( const Edge& obj ) const 
	{
		return (m_nPower > obj.m_nPower);
	}
	vertex* m_lpSrcVertex;   //源顶点
	vertex* m_lpDestVertex;  //目标顶点
	int     m_nPower;        //权
};

class MyMap
{
	public:
	MyList<vertex> m_vertexlist;    //顶点链表
	MyList<Edge>   m_EdgeAlllist;   //边集
	
public:
	MyMap(){}
	~MyMap(){}
	void AddVertex(const char* lpName)
	{
		m_vertexlist.InsertElemTail(lpName);
	}
	vertex* GetVertex( const char* lpName )
	{
		for ( MyList<vertex>::iterator it =  m_vertexlist.begin();
			it != m_vertexlist.end() ; it++ )
		{
			if ( (*it) == vertex(lpName) )
				return &(*it);
		}
		return NULL;
	}
	void AddEdge(char* lpName1,char* lpName2,int nPower )
	{
		vertex* lpSrcV = GetVertex(lpName1);
		vertex* lpDestV = GetVertex(lpName2);

		Edge theEdge(lpSrcV,lpDestV,nPower);

		lpSrcV->m_EdgeList.InsertElemTail(theEdge);

		//加入边集合中
		m_EdgeAlllist.InsertElemTail(theEdge);
	}
	void DFS( vertex* lpSourceVertex)
	{
		//OutPut"深度优先遍历\r\n");
		//保存已访问过的顶点链表
		MyList<vertex*> theAccessedVerList;
		_DFS(theAccessedVerList,lpSourceVertex);
	} 
	//深度优先遍历
	void _DFS( MyList<vertex*>& thelist,vertex* lpSourceVertex )
	{
		if ( lpSourceVertex == NULL ||
			thelist.isFind(lpSourceVertex) )
			return;

		//Output("%s ",lpSourceVertex->m_lpName);
		thelist.InsertElemTail(lpSourceVertex);

		for ( MyList<Edge>::iterator it = lpSourceVertex->m_EdgeList.begin();
			it != lpSourceVertex->m_EdgeList.end() ; it++ )
		{
			_DFS(thelist,(*it).m_lpDestVertex);
		}
	}
	//广度优先遍历
	void BFS( vertex* lpSourceVertex )
	{

		//Output("广度优先遍历\r\n");

		//保存已访问过的顶点链表
		MyList<vertex*> theAccessedList;
		MyQueue<vertex*> theQueue;

		//Output("%s   ",lpSourceVertex->m_lpName);
		theAccessedList.InsertElemTail(lpSourceVertex);
		theQueue.EnQueue(lpSourceVertex);

		vertex* lpCurV = NULL;
		while ( theQueue.DeQueue(lpCurV) )
		{
			for ( MyList<Edge>::iterator it = lpCurV->m_EdgeList.begin();
				it != lpCurV->m_EdgeList.end() ; it++ )
			{
				if ( !theAccessedList.isFind((*it).m_lpDestVertex) )
				{
					theAccessedList.InsertElemTail((*it).m_lpDestVertex);
					theQueue.EnQueue((*it).m_lpDestVertex);
				}
			}
		}
	}

	//返回顶点lp1  所在集合
	MyList<vertex*>* GetSetByVertex( MyList< MyList<vertex*> >& theSetList,
		vertex* lp1)
	{
		for ( MyList< MyList<vertex*> >::iterator it = theSetList.begin();
			it != theSetList.end() ; it++ )
		{
			MyList<vertex*>& item = (*it);

			for ( MyList<vertex*>::iterator it1 = item.begin();
				it1 != item.end() ; it1++ )
			{
				if ( lp1 == (*it1) )
				{
					return &item;
				}
			}
		}
		return NULL;
	}


	void UnionSet(MyList< MyList<vertex*> >& theSetList,
		MyList<vertex*>* lpSet1,MyList<vertex*>* lpSet2)
	{
		for ( MyList<vertex*>::iterator it = lpSet2->begin();
			it != lpSet2->end() ; it++ )
		{
			lpSet1->InsertElemTail(*it);
		}

		theSetList.DeleteElem(*lpSet2);
	}


	//最小生成树  
	void Kruskal()
	{
		//Set 集合
		MyList< MyList<vertex*> > theSetList;

		for ( MyList<vertex>::iterator it = m_vertexlist.begin();
			it != m_vertexlist.end() ; it++ )
		{
			MyList<vertex*> theSet1;
			theSet1.InsertElemTail(&(*it));
			theSetList.InsertElemTail(theSet1);
		}

		//边排序
		m_EdgeAlllist.Sort();

		//while (取最小边 and 其两顶点不构成回路)
		//(边顶点所在树) 两树合并

		for ( MyList<Edge>::iterator it1 = m_EdgeAlllist.begin();
			it1 != m_EdgeAlllist.end() ; it1++ )
		{
			Edge& theE = (*it1);

			MyList<vertex*>* lpSet1 = GetSetByVertex(theSetList,
				theE.m_lpSrcVertex);

			MyList<vertex*>* lpSet2 = GetSetByVertex(theSetList,
				theE.m_lpDestVertex);

			//其两顶点不构成回路
			if ( lpSet1 != lpSet2 )
			{
				UnionSet(theSetList,lpSet1,lpSet2);
			}
		}
	}

	//找到剩余的点中找与这两点（其中之一）距离最小边
	bool Find_MinEdge_OnlyOneVertex_InTreeSet(MyList<vertex*>& treeSet,
		OUT Edge& MinEdge)
	{
		bool isfind = false;

		int  nMinPowerNum = 0;

		for ( MyList<vertex*>::iterator it2 = treeSet.begin();
			it2 != treeSet.end() ; it2++ )
		{
			//里面的某点
			vertex* lpA = *it2;

			for ( MyList<Edge>::iterator it1 = lpA->m_EdgeList.begin();
				it1 != lpA->m_EdgeList.end() ; it1++ )
			{
				if ( !treeSet.isFind((*it1).m_lpDestVertex) )
				{
					if ( !isfind )
					{
						nMinPowerNum = (*it1).m_nPower;
						MinEdge      = (*it1);
						isfind = true;
					}
					else
					{
						if ( nMinPowerNum > (*it1).m_nPower )
						{
							nMinPowerNum = (*it1).m_nPower;
							MinEdge      = (*it1);
							isfind = true;
						}
					}  
				}
			}
		}
		return isfind;
	}

	//适用 无向图
	//从一个点出发，列出这个点所有邻接的边，选择最小的边，将所连顶点到树中，再到剩余的点中找与这两点（其中之一）距离最小的点加入之中
	void Prim()
	{
		vertex firstvertex;

		if ( !m_vertexlist.GetElem(0,firstvertex) )
			return;

		MyList<vertex*> treeSet;

		//从一个点出发，列出这个点所有邻接的边，选择最小的边
		Edge MinEdge;
		int  nMinPowerNum = 0;
		bool isFind = false;

		for ( MyList<Edge>::iterator it1 = firstvertex.m_EdgeList.begin();
			it1 != firstvertex.m_EdgeList.end() ; it1++ )
		{
			if ( !isFind )
			{
				nMinPowerNum = (*it1).m_nPower;
				MinEdge      = (*it1);
				isFind       = true;
			}
			else
			{
				if ( nMinPowerNum > (*it1).m_nPower )
				{
					nMinPowerNum = (*it1).m_nPower;
					MinEdge      = (*it1);
					isFind       = true;
				}
			}
		}

		//Output("  %s->%s",MinEdge.m_lpSrcVertex->m_lpName,MinEdge.m_lpDestVertex->m_lpName);
		treeSet.InsertElemTail( MinEdge.m_lpSrcVertex );
		treeSet.InsertElemTail( MinEdge.m_lpDestVertex );


		//再到剩余的点中找与这两点（其中之一）距离最小边
		while ( Find_MinEdge_OnlyOneVertex_InTreeSet(treeSet,MinEdge) )
		{
			//Output("  %s->%s",MinEdge.m_lpSrcVertex->m_lpName,MinEdge.m_lpDestVertex->m_lpName);
			treeSet.InsertElemTail( MinEdge.m_lpSrcVertex );
			treeSet.InsertElemTail( MinEdge.m_lpDestVertex );
		}
	}
	void PrintAll2PointPath( const char* lpName1,
		const char* lpName2 )
	{
		
		vertex* lpS = GetVertex(lpName1);
		vertex* lpD = GetVertex(lpName2);
		//Output("%X %X",lpS,lpD);

		if ( lpS && lpD )
		{
			lpS->m_sPath.InsertElemTail((char*)lpS->m_lpName);
			_PrintAll2PointPath(lpS,lpD);
		}
	}
	//剩下的点中(BlackList之外的) 找一个成本最小的
	vertex* FindOutBlackListMinCostVertex(MyList<vertex*>& BlackList)
	{
		bool    isfind = false;
		int     nMinCost = 0;
		vertex* lpFindVertex = NULL;

		for ( MyList<vertex>::iterator it1 = m_vertexlist.begin();it1 != m_vertexlist.end() ; it1++ )
		{
			if ( !BlackList.isFind(&(*it1))  )
			{
				if ( !isfind )
				{
					isfind = true;
					nMinCost = (*it1).m_cost;
					lpFindVertex = &(*it1);
				}
				else
				{
					if (  (*it1).m_cost < nMinCost  )
					{
						nMinCost = (*it1).m_cost;
						lpFindVertex = &(*it1);
					}
				}
			}
		}
		return lpFindVertex;
	}

	//两点间最短路径 dijkstar
	void GetSinglePointMinPath(const char* lpName1,const char* lpName2, list<string>& MinPathlist)
	{
		vertex* lpS = GetVertex(lpName1);
		vertex* lpD = GetVertex(lpName2);
		//Output("%X %X",lpS,lpD);

		//O(v^2 + E)
		//O(v(logv) + E(logv))  //最小堆
		//O(v(logv) + E)        //斐波那契堆
		if ( lpS && lpD )
		{
			//所有顶点的成本数值变无穷大
			for ( MyList<vertex>::iterator it1 = m_vertexlist.begin();
				it1 != m_vertexlist.end() ; it1++ )
			{
				(*it1).m_cost = 99999;
			}

			MyList<vertex*> BlackList;
			lpS->m_cost = 0;
			vertex* lpCurV = lpS;

			do
			{
				BlackList.InsertElemTail(lpCurV);

				//对每条边 松弛操作
				for ( MyList<Edge>::iterator it = lpCurV->m_EdgeList.begin();
					it != lpCurV->m_EdgeList.end() ; it++ )
				{
					if ( lpCurV->m_cost + (*it).m_nPower < (*it).m_lpDestVertex->m_cost )
					{
						(*it).m_lpDestVertex->m_cost = lpCurV->m_cost + (*it).m_nPower;
						(*it).m_lpDestVertex->m_costsource = lpCurV;
					}
				}

				//剩下的点中(BlackList之外的) 找一个成本最小的
			} while ( lpCurV = FindOutBlackListMinCostVertex(BlackList));

			//Output("%d\r\n",lpD->m_cost);
			lpCurV = lpD;

			while ( lpCurV )
			{
				string strbuf;
				strbuf = lpCurV->m_lpName;
				MinPathlist.push_front(strbuf);
				//Output("[%s]\r\n",lpCurV->m_lpName);

				lpCurV = lpCurV->m_costsource;
			}
		}
	}
	//两点间所有路径
	void _PrintAll2PointPath( vertex* lpS,vertex* lpD )
	{
		
		if ( lpS == NULL )
			return;

		if (lpS == lpD)
		{
			//打印 lpS.路径
			for ( MyList<char*>::iterator it1 = lpS->m_sPath.begin();
				it1 != lpS->m_sPath.end() ; it1++ )
			{
				//Output(" %s",*it1);
			}
			//Output("\r\n");
			return;
		}

		//for 顶点每条边
		for ( MyList<Edge>::iterator it = lpS->m_EdgeList.begin();
			it != lpS->m_EdgeList.end() ; it++ )
		{
			Edge& theEdge = *it;

			// if ( !顶点.路径  是否有(边.目标顶点) ) 
			if ( ! lpS->m_sPath.isFind((char*)theEdge.m_lpDestVertex->m_lpName)   )
			{
				//边.目标顶点.路径 = 顶点.路径 + 边.目标顶点

				theEdge.m_lpDestVertex->m_sPath.Clear();
				for ( MyList<char*>::iterator it1 = lpS->m_sPath.begin();
					it1 != lpS->m_sPath.end() ; it1++ )
				{
					theEdge.m_lpDestVertex->m_sPath.InsertElemTail(*it1);
				}
				theEdge.m_lpDestVertex->m_sPath.InsertElemTail((char*)theEdge.m_lpDestVertex->m_lpName);


				_PrintAll2PointPath(theEdge.m_lpDestVertex,lpD);
			}
		}

	}
};
