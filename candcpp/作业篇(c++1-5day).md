#作业篇(构造函数析构函数虚表模板)

<font color="red"></font>


##题目1：
<font color="blue">

	4 设计一个结构叫做MyString，要求该结构能够完成以下功能：						
	  (1) 构造函数能够根据实际传入的参数分配实际存储空间；						
	  (2) 提供一个无参的构造函数，默认分配大小为1024个字节；						
	  (3) 析构函数释放该空间；						
	  (4) 编写成员函数SetString，可以将一个字符串赋值给该结构；						
	  (5) 编写成员函数PrintString，可以将该结构的内容打印到屏幕上；						
	  (6) 编写成员函数AppendString，用于向已有的数据后面添加数据；						
	  (7) 编写成员函数Size，用于得到当前数据的真实长度。						
	  编写测试程序，测试这个结构。		
</font>				

代码如下：

	#include "stdafx.h"
	#include "malloc.h"
	#include "string.h"
	
	struct Mystring
	{
	public:
		char* pmystring;
	public:
		Mystring()
		{
			pmystring = (char*)malloc(1024);
			memset(pmystring,0,1024);
		}
		Mystring(int size)
		{
			pmystring = (char*)malloc(size);
			memset(pmystring,0,size);
		}
		~Mystring()
		{
			free(pmystring);
			pmystring = NULL;
			printf("析构函数执行了");
		}
	public:
		void SetString(char* parr)
		{
			memcpy(pmystring,parr,strlen(parr));
		}
		void PrintString()
		{
			printf("%s ",pmystring);
		}
		void AppendString(char* parr2)
		{
			int ptemp  = 0;
			ptemp = (int)pmystring;
			ptemp = (int)pmystring + strlen(pmystring);
			memcpy((char*)ptemp,parr2,strlen(parr2));
		}
		void Size()
		{
			int i = strlen(pmystring);
			printf("%d ",i);
		}
	};
	void testfunc()
	{
		Mystring mystr(100);
		mystr.SetString("hello WORLD");
		mystr.PrintString();
		mystr.AppendString(" mr.wang!");
		mystr.PrintString();
		mystr.Size();
	}

主函数反汇编：

	53:       Mystring mystr(100);
	0040107D 6A 64                push        64h
	0040107F 8D 4D F0             lea         ecx,[ebp-10h]
	00401082 E8 8D FF FF FF       call        @ILT+15(Mystring::Mystring) (00401014)
	00401087 C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0
	54:       mystr.SetString("hello WORLD");
	0040108E 68 1C 50 42 00       push        offset string "hello" (0042501c)
	00401093 8D 4D F0             lea         ecx,[ebp-10h]
	00401096 E8 6A FF FF FF       call        @ILT+0(Mystring::SetString) (00401005)
	55:       mystr.PrintString();
	0040109B 8D 4D F0             lea         ecx,[ebp-10h]
	0040109E E8 67 FF FF FF       call        @ILT+5(Mystring::PrintString) (0040100a)
	56:       mystr.AppendString(" mr.wang!");
	004010A3 68 28 60 42 00       push        offset string " mr.wang!" (00426028)
	004010A8 8D 4D F0             lea         ecx,[ebp-10h]
	004010AB E8 73 FF FF FF       call        @ILT+30(Mystring::AppendString) (00401023)
	57:       mystr.PrintString();
	004010B0 8D 4D F0             lea         ecx,[ebp-10h]
	004010B3 E8 52 FF FF FF       call        @ILT+5(Mystring::PrintString) (0040100a)
	58:       mystr.Size();
	004010B8 8D 4D F0             lea         ecx,[ebp-10h]
	004010BB E8 68 FF FF FF       call        @ILT+35(Mystring::Size) (00401028)
	59:   }
	004010C0 C7 45 FC FF FF FF FF mov         dword ptr [ebp-4],0FFFFFFFFh
	004010C7 8D 4D F0             lea         ecx,[ebp-10h]
	004010CA E8 4A FF FF FF       call        @ILT+20(Mystring::~Mystring) (00401019)
	004010CF 8B 4D F4             mov         ecx,dword ptr [ebp-0Ch]
	004010D2 64 89 0D 00 00 00 00 mov         dword ptr fs:[0],ecx

构造函数反汇编：

	19:       {
	20:           pmystring = (char*)malloc(size);
	0040110D 8B 45 08             mov         eax,dword ptr [ebp+8]
	00401110 50                   push        eax
	00401111 E8 0A 07 00 00       call        malloc (00401820)
	00401116 83 C4 04             add         esp,4
	00401119 8B 4D FC             mov         ecx,dword ptr [ebp-4]
	0040111C 89 01                mov         dword ptr [ecx],eax
	21:           memset(pmystring,0,size);
	0040111E 8B 55 08             mov         edx,dword ptr [ebp+8]
	00401121 52                   push        edx
	00401122 6A 00                push        0
	00401124 8B 45 FC             mov         eax,dword ptr [ebp-4]
	00401127 8B 08                mov         ecx,dword ptr [eax]
	00401129 51                   push        ecx
	0040112A E8 91 06 00 00       call        memset (004017c0)
	0040112F 83 C4 0C             add         esp,0Ch

setstring函数的反汇编：

	004011D9 59                   pop         ecx
	004011DA 89 4D FC             mov         dword ptr [ebp-4],ecx
	32:           memcpy(pmystring,parr,strlen(parr));
	004011DD 8B 45 08             mov         eax,dword ptr [ebp+8]
	004011E0 50                   push        eax
	004011E1 E8 FA A9 00 00       call        strlen (0040bbe0)
	004011E6 83 C4 04             add         esp,4
	004011E9 50                   push        eax
	004011EA 8B 4D 08             mov         ecx,dword ptr [ebp+8]
	004011ED 51                   push        ecx
	004011EE 8B 55 FC             mov         edx,dword ptr [ebp-4]
	004011F1 8B 02                mov         eax,dword ptr [edx]
	004011F3 50                   push        eax
	004011F4 E8 A7 25 00 00       call        memcpy (004037a0)
	004011F9 83 C4 0C             add         esp,0Ch

appendstring函数反汇编：

	0040ED09 59                   pop         ecx
	0040ED0A 89 4D FC             mov         dword ptr [ebp-4],ecx
	40:           int ptemp  = 0;
	0040ED0D C7 45 F8 00 00 00 00 mov         dword ptr [ebp-8],0
	41:           ptemp = (int)pmystring;
	0040ED14 8B 45 FC             mov         eax,dword ptr [ebp-4]
	0040ED17 8B 08                mov         ecx,dword ptr [eax]
	0040ED19 89 4D F8             mov         dword ptr [ebp-8],ecx
	42:           ptemp = (int)pmystring + strlen(pmystring);
	0040ED1C 8B 55 FC             mov         edx,dword ptr [ebp-4]
	0040ED1F 8B 02                mov         eax,dword ptr [edx]
	0040ED21 50                   push        eax
	0040ED22 E8 B9 CE FF FF       call        strlen (0040bbe0)
	0040ED27 83 C4 04             add         esp,4
	0040ED2A 8B 4D FC             mov         ecx,dword ptr [ebp-4]
	0040ED2D 8B 11                mov         edx,dword ptr [ecx]
	0040ED2F 03 D0                add         edx,eax
	0040ED31 89 55 F8             mov         dword ptr [ebp-8],edx
	43:           memcpy((char*)ptemp,parr2,strlen(parr2));
	0040ED34 8B 45 08             mov         eax,dword ptr [ebp+8]
	0040ED37 50                   push        eax
	0040ED38 E8 A3 CE FF FF       call        strlen (0040bbe0)
	0040ED3D 83 C4 04             add         esp,4
	0040ED40 50                   push        eax
	0040ED41 8B 4D 08             mov         ecx,dword ptr [ebp+8]
	0040ED44 51                   push        ecx
	0040ED45 8B 55 F8             mov         edx,dword ptr [ebp-8]
	0040ED48 52                   push        edx
	0040ED49 E8 52 4A FF FF       call        memcpy (004037a0)
	0040ED4E 83 C4 0C             add         esp,0Ch

析构函数的反汇编：

	00401179 59                   pop         ecx
	0040117A 89 4D FC             mov         dword ptr [ebp-4],ecx
	25:           free(pmystring);
	0040117D 8B 45 FC             mov         eax,dword ptr [ebp-4]
	00401180 8B 08                mov         ecx,dword ptr [eax]
	00401182 51                   push        ecx
	00401183 E8 18 11 00 00       call        free (004022a0)
	00401188 83 C4 04             add         esp,4
	26:           pmystring = NULL;
	0040118B 8B 55 FC             mov         edx,dword ptr [ebp-4]
	0040118E C7 02 00 00 00 00    mov         dword ptr [edx],0
	27:           printf("析构函数执行了");
	00401194 68 34 60 42 00       push        offset string "\xce\xf6\xb9\xb9\xba\xaf\xca\xfd\xd6\xb4\xd0\xd0\xc1\xcb" (00426034)
	00401199 E8 42 29 00 00       call        printf (00403ae0)
	0040119E 83 C4 04             add         esp,4

##题目2：
<font color="blue">	

	1、定义一个父类：Base 有两个成员X,Y 有一个函数Print(非virtul)	
	能够打印X,Y的值.	
		
	定义3个子类：Sub1  有一个成员A	
		
		    Sub2  有一个成员B
		
		    Sub3  有一个成员C
		
		    
	每个子类有一个函数Print(非virtul)，打印所有成员.	
		
	Sub1:打印X Y A	
		
	Sub2:打印X Y B	
		
	Sub3:打印X Y C	
		
	2、定义一个数组，存储Base Sub1 Sub2 Sub3 使用一个循环语句	
	调用所有的Print函数。	
		
	3、将上面所有的Print函数改成virtul 继续观察效果.	
		
	4、思考题：为什么析构函数建议写成virtul的？	

</font>
代码如下：

	struct Base				
	{	
	public:
		int x;
		int y;
		Base()
		{
			x = 10;
			y = 20;
		}
	public:				
	    virtual void print()				
	    {				
	        printf("Base:x:%d y:%d\n",x,y);				
	    }				
					
	};				
	struct sub1:public Base			
	{	
	public:
		int a;
		sub1()
		{
			x =30;
			y =40;
			a =50;
		}
	public:				
		virtual void print()				
	    {				
	        printf("sub1:x:%d y:%d a:%d\n",x,y,a);				
	    }			
	};	
	struct sub2:public Base			
	{	
	public:
		int b;
		sub2()
		{
			x =60;
			y =70;
			b =80;
		}
	public:				
		virtual void print()				
	    {				
	        printf("sub2:x:%d y:%d b:%d\n",x,y,b);				
	    }			
	};
	struct sub3:public Base			
	{	
	public:
		int c;
		sub3()
		{
			x =90;
			y =100;
			c =110;
		}
	public:				
		virtual void print()				
	    {				
	        printf("sub3:x:%d y:%d c:%d\n",x,y,c);				
	    }			
	};				
	int main(int argc, char* argv[])
	{
		Base b;
		sub1 s1;
		sub2 s2;
		sub3 s3;
		Base* parr[4] = {&b,&s1,&s2,&s3};
		
		for(int i = 0;i<4;i++)
		{
			parr[i]->print();
		}
		return 0;
	}	

主函数反汇编：

	83:       Base b;
	0040139E 8D 4D F4             lea         ecx,[ebp-0Ch]
	004013A1 E8 64 FC FF FF       call        @ILT+5(Base::Base) (0040100a)
	84:       sub1 s1;
	004013A6 8D 4D E4             lea         ecx,[ebp-1Ch]
	004013A9 E8 7F FC FF FF       call        @ILT+40(sub1::sub1) (0040102d)
	85:       sub2 s2;
	004013AE 8D 4D D4             lea         ecx,[ebp-2Ch]
	004013B1 E8 59 FC FF FF       call        @ILT+10(sub2::sub2) (0040100f)
	86:       sub3 s3;
	004013B6 8D 4D C4             lea         ecx,[ebp-3Ch]
	004013B9 E8 6A FC FF FF       call        @ILT+35(sub3::sub3) (00401028)
	87:       Base* parr[4] = {&b,&s1,&s2,&s3};
	004013BE 8D 45 F4             lea         eax,[ebp-0Ch]
	004013C1 89 45 B4             mov         dword ptr [ebp-4Ch],eax
	004013C4 8D 4D E4             lea         ecx,[ebp-1Ch]
	004013C7 89 4D B8             mov         dword ptr [ebp-48h],ecx
	004013CA 8D 55 D4             lea         edx,[ebp-2Ch]
	004013CD 89 55 BC             mov         dword ptr [ebp-44h],edx
	004013D0 8D 45 C4             lea         eax,[ebp-3Ch]
	004013D3 89 45 C0             mov         dword ptr [ebp-40h],eax
	88:
	89:       for(int i = 0;i<4;i++)
	004013D6 C7 45 B0 00 00 00 00 mov         dword ptr [ebp-50h],0
	004013DD EB 09                jmp         main+68h (004013e8)
	004013DF 8B 4D B0             mov         ecx,dword ptr [ebp-50h]
	004013E2 83 C1 01             add         ecx,1
	004013E5 89 4D B0             mov         dword ptr [ebp-50h],ecx
	004013E8 83 7D B0 04          cmp         dword ptr [ebp-50h],4
	004013EC 7D 1D                jge         main+8Bh (0040140b)
	90:       {
	91:           parr[i]->print();
	004013EE 8B 55 B0             mov         edx,dword ptr [ebp-50h]
	004013F1 8B 4C 95 B4          mov         ecx,dword ptr [ebp+edx*4-4Ch]
	004013F5 8B 45 B0             mov         eax,dword ptr [ebp-50h]
	004013F8 8B 54 85 B4          mov         edx,dword ptr [ebp+eax*4-4Ch]
	004013FC 8B 02                mov         eax,dword ptr [edx]
	004013FE 8B F4                mov         esi,esp
	00401400 FF 10                call        dword ptr [eax]
	00401402 3B F4                cmp         esi,esp
	00401404 E8 B7 00 00 00       call        __chkesp (004014c0)
	92:       }
	00401409 EB D4                jmp         main+5Fh (004013df)
	93:       return 0;
	0040140B 33 C0                xor         eax,eax

	
##题目3：

<font color="blue">		
	
	本节练习：							
							
	1、使用模版实现swap(x,y)函数，功能：交换x,y的值.							
								
	2、冒泡排序：对结构体或者类进行排序，如果不能实现，找出问题所在.							
								
	3、观察下面两个Sort方法的反汇编代码(看内存地址和内容)：							
								
	int arr[] = {2,6,1,5,4};							
								
	char arr1[] = {2,6,1,5,4};							
								
	Sort(arr,5);							
								
	Sort(arr1,5);							
</font>

swap代码
				
	template<class T>				
	int swap(T* a,T* b)				
	{				
		T c;			
		c = *b;			
		*b = *a;			
		*a = c;			
		return 1;			
	}				
					
	int main(int argc, char* argv[])				
	{				
		int a;			
		int b;			
		a = 10;			
		b = 20;			
		swap(&a,&b);			
		printf(" a:%d  b:%d",a,b);			
		return 0;			
	}				
		

主函数反汇编
									
	20:       int a;									
	21:       int b;									
	22:       a = 10;									
	0040DB68 C7 45 FC 0A 00 00 00 mov         dword ptr [ebp-4],0Ah									
	23:       b = 20;									
	0040DB6F C7 45 F8 14 00 00 00 mov         dword ptr [ebp-8],14h									
	24:       swap(&a,&b);									
	0040DB76 8D 45 F8             lea         eax,[ebp-8]									
	0040DB79 50                   push        eax									
	0040DB7A 8D 4D FC             lea         ecx,[ebp-4]									
	0040DB7D 51                   push        ecx									
	0040DB7E E8 F5 34 FF FF       call        @ILT+115(swap) (00401078)									
	0040DB83 83 C4 08             add         esp,8																	
	26:       return 0;									
	0040DB9B 33 C0                xor         eax,eax		
								
swap函数反汇编	
								
	11:       T c;									
	12:       c = *b;									
	00401398 8B 45 0C             mov         eax,dword ptr [ebp+0Ch]									
	0040139B 8B 08                mov         ecx,dword ptr [eax]									
	0040139D 89 4D FC             mov         dword ptr [ebp-4],ecx									
	13:       *b = *a;									
	004013A0 8B 55 0C             mov         edx,dword ptr [ebp+0Ch]									
	004013A3 8B 45 08             mov         eax,dword ptr [ebp+8]									
	004013A6 8B 08                mov         ecx,dword ptr [eax]									
	004013A8 89 0A                mov         dword ptr [edx],ecx									
	14:       *a = c;									
	004013AA 8B 55 08             mov         edx,dword ptr [ebp+8]									
	004013AD 8B 45 FC             mov         eax,dword ptr [ebp-4]									
	004013B0 89 02                mov         dword ptr [edx],eax									
	15:       return 1;									
	004013B2 B8 01 00 00 00       mov         eax,1									
		


char和int类型数组的反汇编对比

	22:       int i;				char类型									22:       int i;				int类型				
	23:       int k;														23:       int k;								
	24:										24:								
	25:       for(i = 0;i<length-1;i++)										25:       for(i = 0;i<length-1;i++)								
	0040DBC8 C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0			0040DD68 C7 45 FC 00 00 00 00 mov         dword ptr [ebp-4],0								
	0040DBCF EB 09                jmp         func+2Ah (0040dbda)			0040DD6F EB 09                jmp         func+2Ah (0040dd7a)								
	0040DBD1 8B 45 FC             mov         eax,dword ptr [ebp-4]			0040DD71 8B 45 FC             mov         eax,dword ptr [ebp-4]								
	0040DBD4 83 C0 01             add         eax,1							0040DD74 83 C0 01             add         eax,1								
	0040DBD7 89 45 FC             mov         dword ptr [ebp-4],eax			0040DD77 89 45 FC             mov         dword ptr [ebp-4],eax								
	0040DBDA 8B 4D 0C             mov         ecx,dword ptr [ebp+0Ch]		0040DD7A 8B 4D 0C             mov         ecx,dword ptr [ebp+0Ch]								
	0040DBDD 83 E9 01             sub         ecx,1							0040DD7D 83 E9 01             sub         ecx,1								
	0040DBE0 39 4D FC             cmp         dword ptr [ebp-4],ecx			0040DD80 39 4D FC             cmp         dword ptr [ebp-4],ecx								
	0040DBE3 7D 63                jge         func+98h (0040dc48)			0040DD83 7D 65                jge         func+9Ah (0040ddea)								
	26:       {																26:       {								
	27:           for(k = 0;k<length-1-i;k++)								27:           for(k = 0;k<length-1-i;k++)								
	0040DBE5 C7 45 F8 00 00 00 00 mov         dword ptr [ebp-8],0			0040DD85 C7 45 F8 00 00 00 00 mov         dword ptr [ebp-8],0								
	0040DBEC EB 09                jmp         func+47h (0040dbf7)			0040DD8C EB 09                jmp         func+47h (0040dd97)								
	0040DBEE 8B 55 F8             mov         edx,dword ptr [ebp-8]			0040DD8E 8B 55 F8             mov         edx,dword ptr [ebp-8]								
	0040DBF1 83 C2 01             add         edx,1							0040DD91 83 C2 01             add         edx,1								
	0040DBF4 89 55 F8             mov         dword ptr [ebp-8],edx			0040DD94 89 55 F8             mov         dword ptr [ebp-8],edx								
	0040DBF7 8B 45 0C             mov         eax,dword ptr [ebp+0Ch]		0040DD97 8B 45 0C             mov         eax,dword ptr [ebp+0Ch]								
	0040DBFA 83 E8 01             sub         eax,1							0040DD9A 83 E8 01             sub         eax,1								
	0040DBFD 2B 45 FC             sub         eax,dword ptr [ebp-4]			0040DD9D 2B 45 FC             sub         eax,dword ptr [ebp-4]								
	0040DC00 39 45 F8             cmp         dword ptr [ebp-8],eax			0040DDA0 39 45 F8             cmp         dword ptr [ebp-8],eax								
	0040DC03 7D 41                jge         func+96h (0040dc46)			0040DDA3 7D 43                jge         func+98h (0040dde8)								
	28:           {															28:           {								
	29:               if(arr[k]>arr[k+1])									29:               if(arr[k]>arr[k+1])								
	0040DC05 8B 4D 08             mov         ecx,dword ptr [ebp+8]			0040DDA5 8B 4D F8         mov         ecx,dword ptr [ebp-8]	 k的值	
	0040DC08 03 4D F8             add         ecx,dword ptr [ebp-8]			0040DDA8 8B 55 08         mov         edx,dword ptr [ebp+8]	数组的地址	
	0040DC0B 0F BE 11             movsx       edx,byte ptr [ecx]			0040DDAB 8B 45 F8         mov         eax,dword ptr [ebp-8]				
	0040DC0E 8B 45 08             mov         eax,dword ptr [ebp+8]			0040DDAE 8B 75 08         mov         esi,dword ptr [ebp+8]				
	0040DC11 03 45 F8             add         eax,dword ptr [ebp-8]			0040DDB1 8B 0C 8A         mov         ecx,dword ptr [edx+ecx*4]	数组下标
	0040DC14 0F BE 48 01          movsx       ecx,byte ptr [eax+1]			0040DDB4 3B 4C 86 04      cmp         ecx,dword ptr [esi+eax*4+4]		
	0040DC18 3B D1                cmp         edx,ecx						0040DDB8 7E 2C            jle         func+96h (0040dde6)			
	0040DC1A 7E 28                jle         func+94h (0040dc44)			30:               {								
	30:               {														31:             T temp = arr[k];								
	31:                   T temp = arr[k];									0040DDBA 8B 55 F8         mov         edx,dword ptr [ebp-8]	下标的值
	0040DC1C 8B 55 08             mov         edx,dword ptr [ebp+8]			0040DDBD 8B 45 08         mov         eax,dword ptr [ebp+8]	arr的地址
	0040DC1F 03 55 F8             add         edx,dword ptr [ebp-8]			0040DDC0 8B 0C 90         mov         ecx,dword ptr [eax+edx*4]取出arr[k]
	0040DC22 8A 02                mov         al,byte ptr [edx]				0040DDC3 89 4D F4         mov         dword ptr [temp],ecx				
	0040DC24 88 45 F4             mov         byte ptr [temp],al			32:                   arr[k] = arr[k+1];								
	32:                   arr[k] = arr[k+1];								0040DDC6 8B 55 F8             mov         edx,dword ptr [ebp-8]								
	0040DC27 8B 4D 08             mov         ecx,dword ptr [ebp+8]			0040DDC9 8B 45 08             mov         eax,dword ptr [ebp+8]								
	0040DC2A 03 4D F8             add         ecx,dword ptr [ebp-8]			0040DDCC 8B 4D F8             mov         ecx,dword ptr [ebp-8]								
	0040DC2D 8B 55 08             mov         edx,dword ptr [ebp+8]			0040DDCF 8B 75 08             mov         esi,dword ptr [ebp+8]								
	0040DC30 03 55 F8             add         edx,dword ptr [ebp-8]			0040DDD2 8B 4C 8E 04          mov         ecx,dword ptr [esi+ecx*4+4]								
	0040DC33 8A 42 01             mov         al,byte ptr [edx+1]			0040DDD6 89 0C 90             mov         dword ptr [eax+edx*4],ecx								
	0040DC36 88 01                mov         byte ptr [ecx],al				33:                   arr[k+1] = temp;								
	33:                   arr[k+1] = temp;									0040DDD9 8B 55 F8             mov         edx,dword ptr [ebp-8]								
	0040DC38 8B 4D 08             mov         ecx,dword ptr [ebp+8]			0040DDDC 8B 45 08             mov         eax,dword ptr [ebp+8]								
	0040DC3B 03 4D F8             add         ecx,dword ptr [ebp-8]			0040DDDF 8B 4D F4             mov         ecx,dword ptr [temp]								
	0040DC3E 8A 55 F4             mov         dl,byte ptr [temp]			0040DDE2 89 4C 90 04          mov         dword ptr [eax+edx*4+4],ecx								
	0040DC41 88 51 01             mov         byte ptr [ecx+1],dl			34:               }								
	34:               }														35:           }								
	35:           }															0040DDE6 EB A6     			 jmp         func+3Eh (0040dd8e)			
	0040DC44 EB A8                jmp         func+3Eh (0040dbee)			36:       }								
	36:       }																0040DDE8 EB 87          jmp         func+21h (0040dd71)					
	0040DC46 EB 89                jmp         func+21h (0040dbd1)			37:       for(int l = 0;l<length;l++)								
	37:       for(int l = 0;l<length;l++)									0040DDEA C7 45 F0 00 00 00 00 mov         dword ptr [ebp-10h],0								
	0040DC48 C7 45 F0 00 00 00 00 mov         dword ptr [ebp-10h],0			0040DDF1 EB 09                jmp         func+0ACh (0040ddfc)								
	0040DC4F EB 09                jmp         func+0AAh (0040dc5a)			0040DDF3 8B 55 F0             mov         edx,dword ptr [ebp-10h]								
	0040DC51 8B 45 F0             mov         eax,dword ptr [ebp-10h]		0040DDF6 83 C2 01             add         edx,1								
	0040DC54 83 C0 01             add         eax,1							0040DDF9 89 55 F0             mov         dword ptr [ebp-10h],edx								
	0040DC57 89 45 F0             mov         dword ptr [ebp-10h],eax		0040DDFC 8B 45 F0             mov         eax,dword ptr [ebp-10h]								
	0040DC5A 8B 4D F0             mov         ecx,dword ptr [ebp-10h]		0040DDFF 3B 45 0C             cmp         eax,dword ptr [ebp+0Ch]								
	0040DC5D 3B 4D 0C             cmp         ecx,dword ptr [ebp+0Ch]		0040DE02 7D 19                jge         func+0CDh (0040de1d)								
	0040DC60 7D 19                jge         func+0CBh (0040dc7b)			38:       {								
	38:       {																39:           printf(" %d \n",arr[l]);								
	39:           printf(" %d \n",arr[l]);									0040DE04 8B 4D F0             mov         ecx,dword ptr [ebp-10h]
	0040DC62 8B 55 08             mov         edx,dword ptr [ebp+8]			0040DE07 8B 55 08             mov         edx,dword ptr [ebp+8]	
	0040DC65 03 55 F0             add         edx,dword ptr [ebp-10h]		0040DE0A 8B 04 8A             mov         eax,dword ptr [edx+ecx*4]		
	0040DC68 0F BE 02             movsx       eax,byte ptr [edx]			0040DE0D 50                   push        eax		
	0040DC6B 50                   push        eax							0040DE0E 68 B0 2F 42 00       push        offset string "x:				
	0040DC6C 68 B0 2F 42 00       push        offset string "x\n" 			0040DE13 E8 18 55 FF FF       call        printf (00403330)				
	0040DC71 E8 BA 56 FF FF       call        printf (00403330)				0040DE18 83 C4 08             add         esp,8					
	0040DC76 83 C4 08             add         esp,8							40:       }								
	40:       }																0040DE1B EB D6          	jmp         func+0A3h (0040ddf3)
	0040DC79 EB D6                jmp         func+0A1h (0040dc51)																		





