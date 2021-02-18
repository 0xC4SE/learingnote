

>c语言中结构体和函数的使用

	struct std
	{
		int x;
		int y;
	
	};
	int func(int x,int y)
	{
		if(x>y)
		{
			return x;
		}
		else
		{
			return y;
		}
	}
	
	31:       std s;
	32:       s.x = 2;
	00401078   mov         dword ptr [ebp-8],2
	33:       s.y = 5;
	0040107F   mov         dword ptr [ebp-4],5
	34:       func(s.x,s.y);
	00401086   mov         eax,dword ptr [ebp-4]
	00401089   push        eax
	0040108A   mov         ecx,dword ptr [ebp-8]
	0040108D   push        ecx
	0040108E   call        @ILT+0(func) (00401005)
	00401093   add         esp,8


-------------------

	struct std
	{
		int x;
		int y;
		int func(int x1,int y1)
		{
		if(x1>y1)
		{
			return x1;
		}
		else
		{
			return y1;
		}
		}
	};
	30:       std s;
	31:       s.x = 2;
	0040D4C8   mov         dword ptr [ebp-8],2
	32:       s.y = 5;
	0040D4CF   mov         dword ptr [ebp-4],5
	33:       s.func(s.x,s.y);
	0040D4D6   mov         eax,dword ptr [ebp-4]
	0040D4D9   push        eax
	0040D4DA   mov         ecx,dword ptr [ebp-8]
	0040D4DD   push        ecx
	0040D4DE   lea         ecx,[ebp-8]
	0040D4E1   call        @ILT+10(std::func) (0040100f)

----------------------

	struct std
	{
		int x;
		int y;
		int func()
		{
		if(x>y)
		{
			return x;
		}
		else
		{
			return y;
		}
		}
	};
	30:       std s;
	31:       s.x = 2;
	0040D4C8   mov         dword ptr [ebp-8],2
	32:       s.y = 5;
	0040D4CF   mov         dword ptr [ebp-4],5
	33:       s.func();
	0040D4D6   lea         ecx,[ebp-8]						//this指针，即结构体的首地址
	0040D4D9   call        @ILT+15(std::func) (00401014)

---

	struct std
	{
		int x;
		int y;
		int func(int x,int y)
		{
		if(x>y)
		{
			return x + this->x;
		}
		else
		{
			return y + this->y;
		}
		}
	};
	30:       std s;
	31:       s.x = 2;
	0040D4C8   mov         dword ptr [ebp-8],2
	32:       s.y = 5;
	0040D4CF   mov         dword ptr [ebp-4],5
	33:       s.func(2,2);
	0040D4D6   push        2
	0040D4D8   push        2
	0040D4DA   lea         ecx,[ebp-8]
	0040D4DD   call        @ILT+20(std::func) (00401019)




	struct sclass			
	{			
		int a;		
		int b;		
				
		void Init(int a,int b)		
		{		
			this->a = a;	
			this->b = b;	
		}		
		void Print()		
		{		
			printf("%d %d",a,b);	
		}		
				
	};
	0040D798   push        2
	0040D79A   push        1
	0040D79C   lea         ecx,[ebp-8]				//每一个结构体成员都有一个this指针
	0040D79F   call        @ILT+40(sclass::Init) (0040102d)
	57:
	58:       s.Print();
	0040D7A4   lea         ecx,[ebp-8]				//每一个结构体成员都有一个this指针
	0040D7A7   call        @ILT+35(sclass::Print) (00401028)
