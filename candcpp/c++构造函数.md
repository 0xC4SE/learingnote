# 构造函数

>构造函数代码

	#include "stdafx.h"
	struct Person
	{
		int age;
		int level;
		Person()
		{
			age = 20;
			level = 12;
		}
		void Print()
		{
			printf("%d %d",age,level);
		}
	};
	void testfunc()
	{
		Person p;
		p.Print();
	}
	int main(int argc, char* argv[])
	{
		testfunc();
		return 0;
	}

>构造函数反汇编
	
	//主函数
	21:       Person p;
	00401048 8D 4D F8             lea         ecx,[ebp-8]
	0040104B E8 C4 FF FF FF       call        @ILT+15(Person::Person) (00401014)
	22:       p.Print();
	00401050 8D 4D F8             lea         ecx,[ebp-8]
	00401053 E8 B2 FF FF FF       call        @ILT+5(Person::Print) (0040100a)
	//构造函数的反汇编
	00401080 55                   push        ebp
	00401081 8B EC                mov         ebp,esp
	00401083 83 EC 44             sub         esp,44h
	00401086 53                   push        ebx
	00401087 56                   push        esi
	00401088 57                   push        edi
	00401089 51                   push        ecx
	0040108A 8D 7D BC             lea         edi,[ebp-44h]
	0040108D B9 11 00 00 00       mov         ecx,11h
	00401092 B8 CC CC CC CC       mov         eax,0CCCCCCCCh
	00401097 F3 AB                rep stos    dword ptr [edi]
	00401099 59                   pop         ecx
	0040109A 89 4D FC             mov         dword ptr [ebp-4],ecx
	10:       {
	11:           age = 20;
	0040109D 8B 45 FC             mov         eax,dword ptr [ebp-4]
	004010A0 C7 00 14 00 00 00    mov         dword ptr [eax],14h
	12:           level = 12;
	004010A6 8B 4D FC             mov         ecx,dword ptr [ebp-4]
	004010A9 C7 41 04 0C 00 00 00 mov         dword ptr [ecx+4],0Ch
	13:       }
	004010B0 8B 45 FC             mov         eax,dword ptr [ebp-4]

结论
---

- 与类同名
- 没有返回值
- 创建对象的时候执行
- 主要用于初始化									
- 可以有多个(最好有一个无参的),称为重载 其他函数也可以重载	
- 构造函数也会在调用前传入参数后传入一个this指针
- 内平栈	
- 构造函数结束时一定会把this指针传给eax		

