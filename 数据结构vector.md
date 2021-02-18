


vector
===


	#define SUCCESS   1 // 成功								
	#define ERROR	 -1 // 失败								
	#define MALLOC_ERROR -2 // 申请内存失败								
	#define INDEX_ERROR	-3 // 错误的索引号								
	
	#define DWORD int
	#define WORD short
	#define BYTE char
	#define VOID void
	#define BOOL bool
	#define FALSE false
	#define TRUE true
	#define IN 											
	#define OUT											
	template <class T>											
	class vector											
	{											
	public:											
		vector();										
		vector(int dwsize);										
		~vector();										
	
	public:											
		int	 at(int dwindex,OUT T* pele);					//根据给定的索引得到元素				
	    int  push_back(T* element);						//将元素存储到容器最后一个位置				
		void pop_back();					//删除最后一个元素				
		int	 insert(int dwindex, T* element);					//向指定位置新增一个元素				
		int	 capacity();					//返回在不增容的情况下，还能存储多少元素				
		void clear();					//清空所有元素				
		bool empty();					//判断Vector是否为空 返回true时为空				
		void erase(int dwindex);					//删除指定元素				
		int	 size();					//返回Vector元素数量的大小				
	
	
	private:											
		bool expand();									
	private:											
		int  m_dwindex;						//下一个可用索引				
		int  m_dwincrement;						//每次增容的大小				
		int  m_dwlen;						//当前容器的长度				
		int  m_dwinitsize;						//默认初始化大小				
		T*   m_pvector;						//容器指针				
	};		
	
	#include "stdafx.h"
	#include "vector.h"
	#include <malloc.h>
	# include <string.h>
	
	template <class T>
	vector<T>::vector()
	:m_dwinitsize(10),m_dwincrement(5)
	{
		m_pvector = new T[m_dwinitsize];	
	
		memset(m_pvector,0,sizeof(T)*m_dwinitsize);
	
		m_dwindex = 0;
	
		m_dwlen = m_dwinitsize;
	
	}
	template <class T>
	vector<T>::vector(int dwsize)					
	:m_dwincrement(5)					
	{			
		//1.创建长度为dwSize个T_ELE对象				
		m_pvector = new T[dwsize];			
		//2.将新创建的空间初始化				
		memset(m_pvector,0,sizeof(T)*dwsize);
		//3.设置其他值
		m_dwindex = 0;
		m_dwinitsize = dwsize;
		m_dwlen = dwsize;
	}
	template <class T>			
	vector<T>::~vector()			
	{			
		delete[] m_pvector;	
		
		m_pvector = NULL;	
		printf("~vector执行了");
	}			
	template <class T>				
	bool vector<T>::expand()				
	{	
		int dwtemp = 0;
	
		// 1. 计算增加后的长度			
		dwtemp = m_dwlen + m_dwincrement;		
		// 2. 申请空间			
		T* new_pv = new T[dwtemp];
		if(new_pv == 00)
		{
			return false;
		}
		memset(new_pv,0,dwtemp*sizeof(T));		
		// 3. 将数据复制到新的空间			
		memcpy(new_pv,m_pvector,m_dwlen*sizeof(T));							
		// 4. 释放原来空间			
		delete[] m_pvector;			
		m_pvector = new_pv;			
		// 5. 为各种属性赋值			
		m_dwlen = dwtemp;
		
		return true;
	}				
	template <class T>						
	DWORD  vector<T>::push_back(T* element)						
	{						
		//1.判断是否需要增容，如果需要就调用增容的函数	
		if(m_dwindex >= m_dwlen)
		{
			expand();
		}			
		memcpy(&m_pvector[m_dwindex],element,sizeof(T));					
		//2.将新的元素复制到容器的最后一个位置					
		m_dwindex++;			
		//3.修改属性值					
		return true;		
	}						
	template <class T>						
	DWORD  vector<T>::insert(DWORD dwindex, T* element)						
	{						
		//1.判断是否在合理区间	0 1 2 3 4				
		if(dwindex<0 || dwindex>m_dwindex)
		{
			return ERROR;
		}											
		//2.判断索引是否在合理区间					
		if(m_dwindex >= m_dwlen)
		{
			expand();
		}					
		//1 2 3 4 5 6 dwindex 7 8 9 m_dwindex					
		//3.将dwIndex只后的元素后移					
		for(DWORD i = m_dwindex;i>dwindex;i--)
		{
			memcpy(&m_pvector[i],&m_pvector[i-1],sizeof(T));
		}											
		//4.将Element元素复制到dwIndex位置					
		memcpy(&m_pvector[dwindex],element,sizeof(T));										
		//5.修改属性值
		m_dwindex++;
		return true;
	}						
	template <class T>					
	DWORD vector<T>::at(DWORD dwindex,T* pele)					
	{					
		//判断索引是否在合理区间				
		if(dwindex<0||dwindex>= m_dwindex)
		{
			return ERROR;
		}								
		//将dwIndex的值复制到pEle指定的内存				
		memcpy(pele,&m_pvector[dwindex],sizeof(T));
		return SUCCESS;			
	}
	//template <class T>					
	//DWORD  vector<T>::capacity()
	//{
	//	DWORD capacity = 0;
	//	capacity = m_dwlen - m_dwindex;
	//	return capacity;
	//}
	struct person
	{
		int x;
		int y;
		int z;
	};
	void func()
	{
		vector<int>* pvector = new vector<int>(5);
		int i = 1;
		int j = 2;
		int k = 3;
		pvector->push_back(&i);
		pvector->push_back(&j);
		pvector->push_back(&i);
		pvector->push_back(&j);
		pvector->push_back(&i);
		pvector->push_back(&j);
		delete pvector;						//释放堆空间
	}









	template <class T>
	vector<T>::vector(int dwsize)					
	:m_dwincrement(5)					
	{			
		//1.创建长度为dwSize个T_ELE对象				
		m_pvector = new T[dwsize];			
		//2.将新创建的空间初始化				
		memset(m_pvector,0,sizeof(T)*dwsize);
		//3.设置其他值
		m_dwindex = 0;
		m_dwinitsize = dwsize;
		m_dwlen = dwsize;
	}
