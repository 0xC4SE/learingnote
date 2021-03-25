#public与private的区别

##为什么会出现public与private？
							
>当几个人共同开发一个项目时，如果一个人改变了struct成员变量，会导致使用变得不安全。						
							
>public的意思是，这个成员哪里都可以用，不用担心被修改，所以，一旦发布成public的成员，是不能够改名字的.							
							
>private的意思是，这个成员只用于内部使用，不要在其他的地方使用.							


使用上
	
		探测一：观察private成员是否有空间			
					
		printf("%d\n",sizeof(Test));			
					
		探测二：直接使用private成员			
					
		Test t;			
		t.x = 10;			
		t.y = 20;


	struct Test	
	{	
	private:	
		int x;
	public:	
		int y;
	};	
	
	void testfunc()
	{
		Test t;
		t.x = 10;
		t.y = 20;
	}

探测一
	sizeof(t)的大小是8，内存中有private的大小

探测二
	//加上private struct外部不能进行赋值，会报错
	error C2248: 'x' : cannot access private member declared in class 'Test'

public与private汇编代码的区别

	struct Test	
	{	
	private:	
		int x;
	public:	
		int y;
		Test()
		{
			this->x = 10;
			this->y = 20;
		}
	};	
	
	void testfunc()
	{
		Test t;
	
		t.y = 11;
	}
	//查看反汇编，发现给y赋值的地址上面有一个A(10)，说明反汇编与public的代码相同
	
								
总结：							
--							
1、对外提供的函数或者变量，发布成public的 但不能随意改动.							
							
2、可能会变动的函数或者变量，定义成private的 这样编译器会在使用的时候做检测.							
							
3、只有结构体内部的函数才可以访问private的成员.							
							
4、public/private可以修饰函数也可以修饰变量.							

				
##private真的不能访问吗？				
					
					
	struct Test				
	{				
	private:				
		int x;			
	public:				
		int y;			
		void Init(int x,int y)			
		{			
			this->x = x;		
			this->y = y;		
		}			
	};				
					
				
	Test t;				
	t.Init(1,2);				
										
	int* p = (int*)&t;				
					
	int n = *p;				
					
	int m = *(p+1);				
					
	printf("%d %d\n",n,m);				
				
下面是主函数汇编代码：

	25:       Test t;
	26:       t.Init(1,2);
	0040D7B8 6A 02                push        2
	0040D7BA 6A 01                push        1
	0040D7BC 8D 4D F8             lea         ecx,[ebp-8]
	0040D7BF E8 50 38 FF FF       call        @ILT+15(Test::Test) (00401014)
	27:
	28:
	29:       int* p = (int*)&t;
	0040D7C4 8D 45 F8             lea         eax,[ebp-8]
	0040D7C7 89 45 F4             mov         dword ptr [ebp-0Ch],eax
	30:
	31:       int n = *p;
	0040D7CA 8B 4D F4             mov         ecx,dword ptr [ebp-0Ch]
	0040D7CD 8B 11                mov         edx,dword ptr [ecx]
	0040D7CF 89 55 F0             mov         dword ptr [ebp-10h],edx
	32:
	33:       int m = *(p+1);
	0040D7D2 8B 45 F4             mov         eax,dword ptr [ebp-0Ch]
	0040D7D5 8B 48 04             mov         ecx,dword ptr [eax+4]
	0040D7D8 89 4D EC             mov         dword ptr [ebp-14h],ecx
	34:
	35:       printf("%d %d\n",n,m);
	0040D7DB 8B 55 EC             mov         edx,dword ptr [ebp-14h]
	0040D7DE 52                   push        edx
	0040D7DF 8B 45 F0             mov         eax,dword ptr [ebp-10h]
	0040D7E2 50                   push        eax
	0040D7E3 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D7E8 E8 33 FF FF FF       call        printf (0040d720)
	0040D7ED 83 C4 0C             add         esp,0Ch

构造函数反汇编：

	00401099 59                   pop         ecx
	0040109A 89 4D FC             mov         dword ptr [ebp-4],ecx
	17:           this->x = x;
	0040109D 8B 45 FC             mov         eax,dword ptr [ebp-4]
	004010A0 8B 4D 08             mov         ecx,dword ptr [ebp+8]
	004010A3 89 08                mov         dword ptr [eax],ecx
	18:           this->y = y;
	004010A5 8B 55 FC             mov         edx,dword ptr [ebp-4]
	004010A8 8B 45 0C             mov         eax,dword ptr [ebp+0Ch]
	004010AB 89 42 04             mov         dword ptr [edx+4],eax

总结：						
---						
private修饰的成员与普通的成员没有区别 只是编译器会检测.						
						
private修饰的成员只要自己的其他成员才能访问						

