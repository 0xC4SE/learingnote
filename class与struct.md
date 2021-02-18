#class与struct

<font color="red"></font>
							
1、成员权限的区别：						
							
	class Base						
	{						
		int x;					
		int y;					
	};						
							
	int main(int argc, char* argv[])						
	{						
		Base base;					
							
		base.x = 10;					
		base.y = 20;					
							
		return 0;					
	}						
							
							
class与struct的区别：						
							
>编译器默认class中的成员为private 而struct中的成员为public	



						
2、继承中的区别：						
						
						
	class Base						
	{						
	public:						
		int x;					
		int y;					
	};						
	class Sub:Base							class Sub:private Base	
	{										{	
	public:									public:	
		int a;									int a;
		int b;									int b;
	};										};	
	int main(int argc, char* argv[])						
	{										//父类中的程序继承后变成private属性	
		Sub sub;					
											//如果不希望改变成员的属性：	
		sub.x = 1;  //无法访问					
		sub.y = 2;  //无法访问				class Sub:public Base	
		sub.a = 3;							{	
		sub.b = 4;							public:	
												int a;
		return 0;								int b;
	}										};	
					

private是否被继承			
--			
			
	class Base			
	{			
	public:			
		Base()		
		{		
			x = 11;	
			y = 12;	
		}		
	private:			
		int x;		
		int y;		
	};			
	class Sub:Base			
	{			
	public:			
		int a;		
		int b;		
	};			
				
	int main(int argc, char* argv[])			
	{			
		Sub sub;		
		sub.a = 1;		
		sub.b = 2;		
				
		int* p = (int*)&sub;		
				
				
		printf("%d\n",sizeof(sub));		
		printf("%d\n",*(p+0));		
		printf("%d\n",*(p+1));		
		printf("%d\n",*(p+2));		
		printf("%d\n",*(p+3));		
				
		return 0;	
	}	

以下是汇编代码:

	31:       Sub sub;
	0040D7B8 8D 4D F0             lea         ecx,[ebp-10h]
	0040D7BB E8 59 38 FF FF       call        @ILT+20(Test::Test) (00401019)
	32:       sub.a = 1;
	0040D7C0 C7 45 F8 01 00 00 00 mov         dword ptr [ebp-8],1
	33:       sub.b = 2;
	0040D7C7 C7 45 FC 02 00 00 00 mov         dword ptr [ebp-4],2
	34:
	35:       int* p = (int*)&sub;
	0040D7CE 8D 45 F0             lea         eax,[ebp-10h]
	0040D7D1 89 45 EC             mov         dword ptr [ebp-14h],eax
	36:
	37:
	38:       printf("%d\n",sizeof(sub));
	0040D7D4 6A 10                push        10h
	0040D7D6 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D7DB E8 40 FF FF FF       call        printf (0040d720)
	0040D7E0 83 C4 08             add         esp,8
	39:       printf("%d\n",*(p+0));
	0040D7E3 8B 4D EC             mov         ecx,dword ptr [ebp-14h]
	0040D7E6 8B 11                mov         edx,dword ptr [ecx]
	0040D7E8 52                   push        edx
	0040D7E9 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D7EE E8 2D FF FF FF       call        printf (0040d720)
	0040D7F3 83 C4 08             add         esp,8
	40:       printf("%d\n",*(p+1));
	0040D7F6 8B 45 EC             mov         eax,dword ptr [ebp-14h]
	0040D7F9 8B 48 04             mov         ecx,dword ptr [eax+4]
	0040D7FC 51                   push        ecx
	0040D7FD 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D802 E8 19 FF FF FF       call        printf (0040d720)
	0040D807 83 C4 08             add         esp,8
	41:       printf("%d\n",*(p+2));
	0040D80A 8B 55 EC             mov         edx,dword ptr [ebp-14h]
	0040D80D 8B 42 08             mov         eax,dword ptr [edx+8]
	0040D810 50                   push        eax
	0040D811 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D816 E8 05 FF FF FF       call        printf (0040d720)
	0040D81B 83 C4 08             add         esp,8
	42:       printf("%d\n",*(p+3));
	0040D81E 8B 4D EC             mov         ecx,dword ptr [ebp-14h]
	0040D821 8B 51 0C             mov         edx,dword ptr [ecx+0Ch]
	0040D824 52                   push        edx
	0040D825 68 9C 2F 42 00       push        offset string "%d %d\n" (00422f9c)
	0040D82A E8 F1 FE FF FF       call        printf (0040d720)
	0040D82F 83 C4 08             add         esp,8


			
<font color="red">			
总结：			
			
1、父类中的私有成员是会被继承的			
			
2、只是编译器不允许直接进行访问			
</font>

