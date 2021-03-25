#虚函数

<font color="red"></font>
<font color="blue">
观察特性，观察用对象内部访问和用指针访问的区别：
</font>

	class Base				
	{				
	public:				
	    void Function_1()				
	    {				
	        printf("Function_1...\n");				
	    }				
	    virtual void Function_2()				
	    {				
	        printf("Function_2...\n");				
	    }				
	};												
	void testfunc()
	{
		Base b;
		b.Function_1();
		b.Function_2();
		
		Base* pb = &b;
		pb->Function_1();
		pb->Function_2();
	}

查看反汇编：

	23:       Base b;
	0040D898 8D 4D FC             lea         ecx,[ebp-4]
	0040D89B E8 7E 37 FF FF       call        @ILT+25(Base::Base) (0040101e)
	24:       b.Function_1();
	0040D8A0 8D 4D FC             lea         ecx,[ebp-4]
	0040D8A3 E8 80 37 FF FF       call        @ILT+35(Test::Init) (00401028)
	25:       b.Function_2();
	0040D8A8 8D 4D FC             lea         ecx,[ebp-4]
	0040D8AB E8 73 37 FF FF       call        @ILT+30(Base::Function_2) (00401023)
	26:
	27:       Base* pb = &b;
	0040D8B0 8D 45 FC             lea         eax,[ebp-4]
	0040D8B3 89 45 F8             mov         dword ptr [ebp-8],eax
	28:       pb->Function_1();
	0040D8B6 8B 4D F8             mov         ecx,dword ptr [ebp-8]
	0040D8B9 E8 6A 37 FF FF       call        @ILT+35(Test::Init) (00401028)
	29:       pb->Function_2();
	0040D8BE 8B 4D F8             mov         ecx,dword ptr [ebp-8]
	0040D8C1 8B 11                mov         edx,dword ptr [ecx]
	0040D8C3 8B F4                mov         esi,esp
	0040D8C5 8B 4D F8             mov         ecx,dword ptr [ebp-8]
	0040D8C8 FF 12                call        dword ptr [edx]
	0040D8CA 3B F4                cmp         esi,esp
	0040D8CC E8 3F 38 FF FF       call        __chkesp (00401110)


<font color="red">  																																																总结：1、通过对象调用时，virtual函数与普通函数都是E8 Call												
2、通过指针调用时，virtual函数是FF Call,也就是间接Call						
</font>

深入虚函数
-----------------


1、当类中有虚函数时，观察大小的变化.				
				
Base base;				
				
printf("%x\n",sizeof(base));	
			
><font color="green">不管有几个virtual函数都是增加4字节</font>			

思考：为什么不管增加多少virtual函数，多出的这4个字节是什么?			

2、观察虚函数通过指针调用时的反汇编：							
														
	pb->Function_1();							
	0040D9E3 8B 4D F0             mov         ecx,dword ptr [ebp-10h]							
	0040D9E6 8B 11                mov         edx,dword ptr [ecx]							
	0040D9E8 8B F4                mov         esi,esp							
	0040D9EA 8B 4D F0             mov         ecx,dword ptr [ebp-10h]							
	0040D9ED FF 12                call        dword ptr [edx]							
															
	pb->Function_2();							
	0040D9F6 8B 45 F0             mov         eax,dword ptr [ebp-10h]							
	0040D9F9 8B 10                mov         edx,dword ptr [eax]							
	0040D9FB 8B F4                mov         esi,esp							
	0040D9FD 8B 4D F0             mov         ecx,dword ptr [ebp-10h]							
	0040DA00 FF 52 04             call        dword ptr [edx+4]			//为什么是FF call？	

<font color="red">
							
总结：
1、当类中有虚函数时，会多一个属性，4个字节							
							
2、多出的属性是一个地址，指向一张表，里面存储了所有虚函数的地址							
</font>
							
##打印虚函数表
	
	class Base				
	{				
	public:				
		int x;			
		int y;	
		Base()
		{
			this->x = 10;
			this->y = 10;
		}
	    virtual void Function_1()				
	    {				
	        printf("Function_1...\n");				
	    }				
	    virtual void Function_2()				
	    {				
	        printf("Function_2...\n");				
	    }				
	    virtual void Function_3()				
	    {				
	        printf("Function_3...\n");				
	    }				
	};																			
	void testfunc()
	{
		Base b;
		printf("base的虚函数表的地址为：%x\n",*(int*)&b);
		typedef void(*pfunc)(void);
		pfunc pf;
		for(int i = 0;i<3;i++)
		{
			int ptemp = *((int*)*(int*)&b + i);
			pf = (pfunc)ptemp;
			pf();
		}
	}
	34:       Base b;								
	0040D898 8D 4D F4             lea         ecx,[ebp-0Ch]								
	0040D89B E8 7E 37 FF FF       call        @ILT+25(Test::Test) (0040101e)								
	35:       printf("base的虚函数表的地址为：%x\n",*(int*)&b);								
	0040D8A0 8B 45 F4             mov         eax,dword ptr [ebp-0Ch]		//this指针第一个4字节存储的内容
	0040D8A3 50                   push        eax								
	0040D8A4 68 C4 2F 42 00       push        offset string "base\xb5\xc4\xd0\xe9\xba\xaf\xca\xfd\xb1\xed\xb5\xc4\xb5\xd8\xd6\xb7\xce\								
	0040D8A9 E8 72 FE FF FF       call        printf (0040d720)								
	0040D8AE 83 C4 08             add         esp,8								
	36:       typedef void(*pfunc)(void);								
	37:       pfunc pf;														//pf是[ebp-10]				
	38:       for(int i = 0;i<3;i++)								
	0040D8B1 C7 45 EC 00 00 00 00 mov         dword ptr [ebp-14h],0			//[ebp-14]是i
	0040D8B8 EB 09                jmp         testfunc+43h (0040d8c3)								
	0040D8BA 8B 4D EC             mov         ecx,dword ptr [ebp-14h]								
	0040D8BD 83 C1 01             add         ecx,1								
	0040D8C0 89 4D EC             mov         dword ptr [ebp-14h],ecx								
	0040D8C3 83 7D EC 03          cmp         dword ptr [ebp-14h],3								
	0040D8C7 7D 20                jge         testfunc+69h (0040d8e9)								
	39:       {								
	40:           int ptemp = *((int*)*(int*)&b + i);								
	0040D8C9 8B 55 EC             mov         edx,dword ptr [ebp-14h]								
	0040D8CC 8B 45 F4             mov         eax,dword ptr [ebp-0Ch]								
	0040D8CF 8B 0C 90             mov         ecx,dword ptr [eax+edx*4]								
	0040D8D2 89 4D E8             mov         dword ptr [ptemp],ecx								
	41:           pf = (pfunc)ptemp;								
	0040D8D5 8B 55 E8             mov         edx,dword ptr [ptemp]								
	0040D8D8 89 55 F0             mov         dword ptr [ebp-10h],edx								
	42:           pf();								
	0040D8DB 8B F4                mov         esi,esp								
	0040D8DD FF 55 F0             call        dword ptr [ebp-10h]								
	0040D8E0 3B F4                cmp         esi,esp								
	0040D8E2 E8 29 38 FF FF       call        __chkesp (00401110)								
	43:       }								
	0040D8E7 EB D1                jmp         testfunc+3Ah (0040d8ba)								


