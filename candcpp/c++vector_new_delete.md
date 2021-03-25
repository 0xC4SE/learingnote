#Vector-New-Delete

<font color="red"></font>

##1、malloc函数									
									
	int* pi = (int*)malloc(sizeof(int)*1);									
										
	free(pi);									
										
####malloc->_nh_malloc_dbg->_heap_alloc_dbg->_heap_alloc_base->HeapAlloc									
									
##2、free函数									
									
free->_free_dbg->_free_base->HeapFree									
									
									
##3、new 关键字									
									
	int* pi = new int;									
										
	int* pk = new int(5);	//啧是申请并赋值								
										
	Person* ps = new Person;									
										
	Person* pd = new Person(1,2);									
									
#####第一步：_nh_malloc->_nh_malloc_dbg->_heap_alloc_dbg->_heap_alloc_base->HeapAlloc									
									
#####第二步：调用构造函数									
									
##4、delete关键字									
									
	delete pi;									
										
	delete pk;									
									
#####第一步：调用析构函数									
									
#####第二步：_free_dbg->_free_base->HeapFree									
									
##5、new[] 与 delete[]的用法.									
									
	int* pi = new int[10];		//啧是申请10个int							
										
	delete[] pi;									
										
	Person* p = new Person[2];		//啧是申请俩个单位大小的结构体的堆内存							
										
	delete[] p;						//释放堆内存			
									
##6、vector

	#include "stdafx.h"		
	#include "malloc.h"		
	#include "string.h"		
	#include <Windows.h>		
			
	#define IN		
	#define OUT		
	#define SUCCESS        1 // 成功		
	#define ERROR         -1 // 失败		
	#define MALLOC_ERROR	-2 // 申请内存失败	
	#define INDEX_ERROR		-3 // 错误的索引号
			
	template <class T_ELE>		
	class Vector		
	{		
	public:		
		Vector();	
		Vector(DWORD dwSize);	
		~Vector();	
	public:		
		DWORD	at(DWORD dwIndex,OUT T_ELE* pEle);
	    DWORD   push_back(T_ELE Element);		
		VOID	pop_back();
		DWORD	insert(DWORD dwIndex, T_ELE Element);
		DWORD	capacity();
		VOID	clear();
		BOOL	empty();
		VOID	erase(DWORD dwIndex);
		DWORD	size();
	private:		
		BOOL	expand();
	private:		
		DWORD  m_dwIndex;	
		DWORD  m_dwIncrement;	
		DWORD  m_dwLen;	
		DWORD  m_dwInitSize;	
		T_ELE *m_pVector;	
	};		
			
	template <class T_ELE>		
	Vector<T_ELE>::Vector()		
	:m_dwInitSize(5),m_dwIncrement(5)		
	{		
		//1.创建长度为m_dwInitSize个T_ELE对象	
			m_pVector = (T_ELE*)new T_ELE[m_dwInitSize];
		//2.将新创建的空间初始化	
			memset(m_pVector,0,sizeof(T_ELE)*m_dwInitSize);
		//3.设置其他值	
			m_dwIndex = 0;
			m_dwLen = m_dwInitSize;
	}		
			
	template <class T_ELE>		
	Vector<T_ELE>::Vector(DWORD dwSize)		
	:m_dwIncrement(5)		
	{		
		//1.创建长度为dwSize个T_ELE对象	
			m_pVector = (T_ELE*)new T_ELE[dwSize];
		//2.将新创建的空间初始化	
			memset(m_pVector,0,sizeof(T_ELE)*dwSize);
		//3.设置其他值	
			m_dwIndex = 0;
			m_dwLen = dwSize; 
			m_dwInitSize = dwSize;
	}		
	template <class T_ELE>		
	Vector<T_ELE>::~Vector()		
	{		
			
		//释放空间 delete[]	
		delete[] m_pVector;	
		m_pVector = NULL;	
	}		
			
	template <class T_ELE>		
	BOOL Vector<T_ELE>::expand()		
	{		
		// 1. 计算增加后的长度	
			int templen;
			templen = m_dwLen;
			m_dwLen = m_dwLen + m_dwIncrement;
			
		// 2. 申请空间	
			T_ELE* ptemp;
			
			ptemp = (T_ELE*)new T_ELE[m_dwLen];
			memset(ptemp,0,sizeof(T_ELE)*m_dwLen);
		// 3. 将数据复制到新的空间	
			
			memcpy(ptemp,m_pVector,sizeof(T_ELE)*templen);
			
		// 4. 释放原来空间	
			delete[] m_pVector;
			m_pVector = NULL;
			m_pVector = ptemp;
			
		// 5. 为各种属性赋值	
			return SUCCESS;
	}		
			
	template <class T_ELE>		
	DWORD  Vector<T_ELE>::push_back(T_ELE Element)		
	{		
		//1.判断是否需要增容，如果需要就调用增容的函数	
		if(m_dwIndex >= m_dwLen)	
		{	
			expand();
		}	
		//2.将新的元素复制到容器的最后一个位置	
		memcpy(&m_pVector[m_dwIndex],&Element,sizeof(T_ELE));	
		//3.修改属性值	
		m_dwIndex = m_dwIndex + 1;	
		return SUCCESS;	
	}		
			
	template <class T_ELE>		
	DWORD  Vector<T_ELE>::insert(DWORD dwIndex, T_ELE Element)		
	{		
		//1.判断是否需要增容，如果需要就调用增容的函数	
		if(m_dwIndex >= m_dwLen)	
		{	
			expand();
		}	
		//2.判断索引是否在合理区间	
		if(dwIndex<0||dwIndex>=m_dwIndex)	
		{	
			return INDEX_ERROR;
		}	
			
		//3.将dwIndex只后的元素后移	
		for(int temp = m_dwIndex;temp>dwIndex;temp--)	
		{	
			memcpy(&m_pVector[temp],&m_pVector[temp - 1],sizeof(T_ELE));
		}	
			
		//4.将Element元素复制到dwIndex位置	
		memcpy(&m_pVector[dwIndex],&Element,sizeof(T_ELE));	
			
		//5.修改属性值	
		m_dwIndex = m_dwIndex + 1;	
		return SUCCESS;	
	}		
	template <class T_ELE>		
	DWORD Vector<T_ELE>::at(DWORD dwIndex,T_ELE* pEle)		
	{		
		//判断索引是否在合理区间	
		if(dwIndex<0||dwIndex>m_dwIndex)	
		{	
			return INDEX_ERROR;
		}	
			
		//将dwIndex的值复制到pEle指定的内存	
		memcpy(pEle,&m_pVector[dwIndex],sizeof(T_ELE));	
	}		
	//其他函数。。自己实现		
	template <class T_ELE>		
	void  Vector<T_ELE>::pop_back()		
	{	if(m_dwIndex == 0)	
		{	
			
		}	
		memset(&m_pVector[m_dwIndex-1],0,sizeof(T_ELE));	
		m_dwIndex--;	
	}		
	void func()		
	{		
		Vector<int>* pv = new Vector<int>;	
		pv->push_back(1);	
		pv->push_back(2);	
		pv->push_back(3);	
		pv->push_back(4);	
		pv->push_back(5);	
		pv->pop_back();	
		pv->insert(3,3);	
		int i;	
		pv->at(3,&i);	
		printf("%d ",i);	
	}		
			
	int main(int argc, char* argv[])		
	{		
		func();	
		return 0;	
	}		


下面是vector的返汇编代码：
---

主函数：	
								
	70:      Vector<int>* pv = new Vector<int>;									
	0040ED4D 6A 14                push        14h									
	0040ED4F E8 0C 25 FF FF       call        operator new (00401260)									
	0040ED54 83 C4 04             add         esp,4									
	0040ED57 89 45 E4             mov         dword ptr [ebp-1Ch],eax									
	0040ED5A C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0									
	0040ED61 83 7D E4 00          cmp         dword ptr [ebp-1Ch],0									
	0040ED65 74 0D                je          func+54h (0040ed74)									
	0040ED67 8B 4D E4             mov         ecx,dword ptr [ebp-1Ch]									
	0040ED6A E8 A5 22 FF FF       call        @ILT+15(Vector<int>::Vector<int>) (00401014)									
	0040ED6F 89 45 E0             mov         dword ptr [ebp-20h],eax									
	0040ED72 EB 07                jmp         func+5Bh (0040ed7b)									
	0040ED74 C7 45 E0 00 00 00 00 mov         dword ptr [ebp-20h],0									
	0040ED7B 8B 45 E0             mov         eax,dword ptr [ebp-20h]									
	0040ED7E 89 45 E8             mov         dword ptr [ebp-18h],eax									
	0040ED81 C7 45 FC FF FF FF FF mov         dword ptr [ebp-4],0FFFFFFFFh									
	0040ED88 8B 4D E8             mov         ecx,dword ptr [ebp-18h]									
	0040ED8B 89 4D F0             mov         dword ptr [ebp-10h],ecx									
	171:      pv->push_back(1);									
	0040ED8E 6A 01                push        1									
	0040ED90 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040ED93 E8 86 22 FF FF       call        @ILT+25(Vector<int>::push_back) (0040101e)									
	172:      pv->push_back(2);									
	0040ED98 6A 02                push        2									
	0040ED9A 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040ED9D E8 7C 22 FF FF       call        @ILT+25(Vector<int>::push_back) (0040101e)									
	173:      pv->push_back(3);									
	0040EDA2 6A 03                push        3									
	0040EDA4 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDA7 E8 72 22 FF FF       call        @ILT+25(Vector<int>::push_back) (0040101e)									
	174:      pv->push_back(4);									
	0040EDAC 6A 04                push        4									
	0040EDAE 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDB1 E8 68 22 FF FF       call        @ILT+25(Vector<int>::push_back) (0040101e)									
	175:      pv->push_back(5);									
	0040EDB6 6A 05                push        5									
	0040EDB8 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDBB E8 5E 22 FF FF       call        @ILT+25(Vector<int>::push_back) (0040101e)									
	176:      pv->pop_back();									
	0040EDC0 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDC3 E8 65 22 FF FF       call        @ILT+40(Vector<int>::pop_back) (0040102d)									
	177:      pv->insert(3,3);									
	0040EDC8 6A 03                push        3									
	0040EDCA 6A 03                push        3									
	0040EDCC 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDCF E8 4F 22 FF FF       call        @ILT+30(Vector<int>::insert) (00401023)									
	178:      int i;									
	179:      pv->at(3,&i);									
	0040EDD4 8D 55 EC             lea         edx,[ebp-14h]									
	0040EDD7 52                   push        edx									
	0040EDD8 6A 03                push        3									
	0040EDDA 8B 4D F0             mov         ecx,dword ptr [ebp-10h]									
	0040EDDD E8 46 22 FF FF       call        @ILT+35(Vector<int>::at) (00401028)									
	180:      printf("%d ",i);									
	0040EDE2 8B 45 EC             mov         eax,dword ptr [ebp-14h]									
	0040EDE5 50                   push        eax									
	0040EDE6 68 84 5A 42 00       push        offset string "%d " (00425a84)									
	0040EDEB E8 40 02 00 00       call        printf (0040f030)									
	0040EDF0 83 C4 08             add         esp,8									
	181:  }									
	0040EDF3 8B 4D F4             mov         ecx,dword ptr [ebp-0Ch]									
	0040EDF6 64 89 0D 00 00 00 00 mov         dword ptr fs:[0],ecx									
										
										
push_back函数反汇编	
								
	0040EAB9 59                   pop         ecx									
	0040EABA 89 4D FC             mov         dword ptr [ebp-4],ecx									
	107:      //1.判断是否需要增容，如果需要就调用增容的函数									
	108:      if(m_dwIndex >= m_dwLen)									
	0040EABD 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EAC0 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EAC3 8B 10                mov         edx,dword ptr [eax]									
	0040EAC5 3B 51 08             cmp         edx,dword ptr [ecx+8]									
	0040EAC8 72 08                jb          Vector<int>::push_back+32h (0040ead2)									
	109:      {									
	110:          expand();									
	0040EACA 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EACD E8 47 25 FF FF       call        @ILT+20(Vector<int>::expand) (00401019)									
	111:      }									
	112:      //2.将新的元素复制到容器的最后一个位置									
	113:      memcpy(&m_pVector[m_dwIndex],&Element,sizeof(T_ELE));									
	0040EAD2 6A 04                push        4									
	0040EAD4 8D 45 08             lea         eax,[ebp+8]									
	0040EAD7 50                   push        eax									
	0040EAD8 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EADB 8B 11                mov         edx,dword ptr [ecx]									
	0040EADD 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EAE0 8B 48 10             mov         ecx,dword ptr [eax+10h]									
	0040EAE3 8D 14 91             lea         edx,[ecx+edx*4]									
	0040EAE6 52                   push        edx									
	0040EAE7 E8 B4 CB FF FF       call        memcpy (0040b6a0)									
	0040EAEC 83 C4 0C             add         esp,0Ch									
	114:      //3.修改属性值									
	115:      m_dwIndex = m_dwIndex + 1;									
	0040EAEF 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EAF2 8B 08                mov         ecx,dword ptr [eax]									
	0040EAF4 83 C1 01             add         ecx,1									
	0040EAF7 8B 55 FC             mov         edx,dword ptr [ebp-4]									
	0040EAFA 89 0A                mov         dword ptr [edx],ecx									
	116:      return SUCCESS;									
	0040EAFC B8 01 00 00 00       mov         eax,1									
										
popback反汇编：	
								
	159:  template <class T_ELE>									
	160:  void  Vector<T_ELE>::pop_back()									
	161:  {   if(m_dwIndex == 0)									
	0040EC50 55                   push        ebp									
	0040EC51 8B EC                mov         ebp,esp									
	0040EC53 83 EC 44             sub         esp,44h									
	0040EC56 53                   push        ebx									
	0040EC57 56                   push        esi									
	0040EC58 57                   push        edi									
	0040EC59 51                   push        ecx									
	0040EC5A 8D 7D BC             lea         edi,[ebp-44h]									
	0040EC5D B9 11 00 00 00       mov         ecx,11h									
	0040EC62 B8 CC CC CC CC       mov         eax,0CCCCCCCCh									
	0040EC67 F3 AB                rep stos    dword ptr [edi]									
	0040EC69 59                   pop         ecx									
	0040EC6A 89 4D FC             mov         dword ptr [ebp-4],ecx									
	162:      {									
	163:									
	164:      }									
	165:      memset(&m_pVector[m_dwIndex-1],0,sizeof(T_ELE));									
	0040EC6D 6A 04                push        4									
	0040EC6F 6A 00                push        0									
	0040EC71 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EC74 8B 11                mov         edx,dword ptr [ecx]									
	0040EC76 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EC79 8B 48 10             mov         ecx,dword ptr [eax+10h]									
	0040EC7C 8D 54 91 FC          lea         edx,[ecx+edx*4-4]									
	0040EC80 52                   push        edx									
	0040EC81 E8 0A 2B FF FF       call        memset (00401790)									
	0040EC86 83 C4 0C             add         esp,0Ch									
	166:      m_dwIndex--;									
	0040EC89 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EC8C 8B 08                mov         ecx,dword ptr [eax]									
	0040EC8E 83 E9 01             sub         ecx,1									
	0040EC91 8B 55 FC             mov         edx,dword ptr [ebp-4]									
	0040EC94 89 0A                mov         dword ptr [edx],ecx									
										
insert函数反汇编		
							
	119:  template <class T_ELE>									
	120:  DWORD  Vector<T_ELE>::insert(DWORD dwIndex, T_ELE Element)									
	121:  {									
	0040EB20 55                   push        ebp									
	0040EB21 8B EC                mov         ebp,esp									
	0040EB23 83 EC 48             sub         esp,48h									
	0040EB26 53                   push        ebx									
	0040EB27 56                   push        esi									
	0040EB28 57                   push        edi									
	0040EB29 51                   push        ecx									
	0040EB2A 8D 7D B8             lea         edi,[ebp-48h]									
	0040EB2D B9 12 00 00 00       mov         ecx,12h									
	0040EB32 B8 CC CC CC CC       mov         eax,0CCCCCCCCh									
	0040EB37 F3 AB                rep stos    dword ptr [edi]									
	0040EB39 59                   pop         ecx									
	0040EB3A 89 4D FC             mov         dword ptr [ebp-4],ecx									
	122:      //1.判断是否需要增容，如果需要就调用增容的函数									
	123:      if(m_dwIndex >= m_dwLen)									
	0040EB3D 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EB40 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EB43 8B 10                mov         edx,dword ptr [eax]									
	0040EB45 3B 51 08             cmp         edx,dword ptr [ecx+8]									
	0040EB48 72 08                jb          Vector<int>::insert+32h (0040eb52)									
	124:      {									
	125:          expand();									
	0040EB4A 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EB4D E8 C7 24 FF FF       call        @ILT+20(Vector<int>::expand) (00401019)									
	126:      }									
	127:      //2.判断索引是否在合理区间									
	128:      if(dwIndex<0||dwIndex>=m_dwIndex)									
	0040EB52 83 7D 08 00          cmp         dword ptr [ebp+8],0									
	0040EB56 72 0A                jb          Vector<int>::insert+42h (0040eb62)									
	0040EB58 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EB5B 8B 4D 08             mov         ecx,dword ptr [ebp+8]									
	0040EB5E 3B 08                cmp         ecx,dword ptr [eax]									
	0040EB60 72 07                jb          Vector<int>::insert+49h (0040eb69)									
	129:      {									
	130:          return INDEX_ERROR;									
	0040EB62 B8 FD FF FF FF       mov         eax,0FFFFFFFDh									
	0040EB67 EB 6F                jmp         Vector<int>::insert+0B8h (0040ebd8)									
	131:      }									
	132:									
	133:      //3.将dwIndex只后的元素后移									
	134:      for(int temp = m_dwIndex;temp>dwIndex;temp--)									
	0040EB69 8B 55 FC             mov         edx,dword ptr [ebp-4]									
	0040EB6C 8B 02                mov         eax,dword ptr [edx]									
	0040EB6E 89 45 F8             mov         dword ptr [ebp-8],eax									
	0040EB71 EB 09                jmp         Vector<int>::insert+5Ch (0040eb7c)									
	0040EB73 8B 4D F8             mov         ecx,dword ptr [ebp-8]									
	0040EB76 83 E9 01             sub         ecx,1									
	0040EB79 89 4D F8             mov         dword ptr [ebp-8],ecx									
	0040EB7C 8B 55 F8             mov         edx,dword ptr [ebp-8]									
	0040EB7F 3B 55 08             cmp         edx,dword ptr [ebp+8]									
	0040EB82 76 27                jbe         Vector<int>::insert+8Bh (0040ebab)									
	135:      {									
	136:          memcpy(&m_pVector[temp],&m_pVector[temp - 1],sizeof(T_ELE));									
	0040EB84 6A 04                push        4									
	0040EB86 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EB89 8B 48 10             mov         ecx,dword ptr [eax+10h]									
	0040EB8C 8B 55 F8             mov         edx,dword ptr [ebp-8]									
	0040EB8F 8D 44 91 FC          lea         eax,[ecx+edx*4-4]									
	0040EB93 50                   push        eax									
	0040EB94 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EB97 8B 51 10             mov         edx,dword ptr [ecx+10h]									
	0040EB9A 8B 45 F8             mov         eax,dword ptr [ebp-8]									
	0040EB9D 8D 0C 82             lea         ecx,[edx+eax*4]									
	0040EBA0 51                   push        ecx									
	0040EBA1 E8 FA CA FF FF       call        memcpy (0040b6a0)									
	0040EBA6 83 C4 0C             add         esp,0Ch									
	137:      }									
	0040EBA9 EB C8                jmp         Vector<int>::insert+53h (0040eb73)									
	138:									
	139:      //4.将Element元素复制到dwIndex位置									
	140:      memcpy(&m_pVector[dwIndex],&Element,sizeof(T_ELE));									
	0040EBAB 6A 04                push        4									
	0040EBAD 8D 55 0C             lea         edx,[ebp+0Ch]									
	0040EBB0 52                   push        edx									
	0040EBB1 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EBB4 8B 48 10             mov         ecx,dword ptr [eax+10h]									
	0040EBB7 8B 55 08             mov         edx,dword ptr [ebp+8]									
	0040EBBA 8D 04 91             lea         eax,[ecx+edx*4]									
	0040EBBD 50                   push        eax									
	0040EBBE E8 DD CA FF FF       call        memcpy (0040b6a0)									
	0040EBC3 83 C4 0C             add         esp,0Ch									
	141:									
	142:      //5.修改属性值									
	143:      m_dwIndex = m_dwIndex + 1;									
	0040EBC6 8B 4D FC             mov         ecx,dword ptr [ebp-4]									
	0040EBC9 8B 11                mov         edx,dword ptr [ecx]									
	0040EBCB 83 C2 01             add         edx,1									
	0040EBCE 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	0040EBD1 89 10                mov         dword ptr [eax],edx									
	144:      return SUCCESS;									
	0040EBD3 B8 01 00 00 00       mov         eax,1									
