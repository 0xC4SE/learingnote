#继承

##1、观察下面代码的反汇编	

	struct Person		
	{		
		int age;	
		int sex;	
	};		
	struct Teacher		
	{		
		int age;	
		int sex;	
		int level;	
		int classId;	
	};		
	struct Student		
	{		
		int age;	
		int sex;	
		int code;	
		int score;	
	};		
						
	Teacher t;		
			
	t.age = 1;		
	t.sex = 2;		
	t.level = 3;		
	t.classId = 4;		
			
	printf("%d\n",sizeof(t));		
	Student s;				
	s.age = 1;		
	s.sex = 2;		
	s.code = 3;		
	s.score = 4;		
			
	printf("%d\n",sizeof(s));		
			
					
观察反汇编：					
					
	mov         dword ptr [ebp-10h],1					
	mov         dword ptr [ebp-0Ch],2					
	mov         dword ptr [ebp-8],3					
	mov         dword ptr [ebp-4],4					
	push        10h					
	push        offset string "%d\n" (0042201c)					
	call        printf (004010e0)					
	add         esp,8					
						
						
						
	mov         dword ptr [ebp-10h],1					
	mov         dword ptr [ebp-0Ch],2					
	mov         dword ptr [ebp-8],3					
	mov         dword ptr [ebp-4],4					
	push        10h					
	push        offset string "%d\n" (0042201c)					
	call        printf (004010e0)					
	add         esp,8					
						
##2、改变写法：	


	struct Person		
	{		
		int age;	
		int sex;	
	};		
	struct Teacher:Person		
	{		
		int level;	
		int classId;	
	};		
	struct Student:Person		
	{		
		int code;	
		int score;	
	};		
			
			
	Teacher t;		
			
	t.age = 1;		
	t.sex = 2;		
	t.level = 3;		
	t.classId = 4;		
			
	printf("%d\n",sizeof(t));		
			
			
			
			
	Student s;		
			
	s.age = 1;		
	s.sex = 2;		
	s.code = 3;		
	s.score = 4;		
			
	printf("%d\n",sizeof(s));		

观察反汇编:	

	mov         dword ptr [ebp-10h],1					
	mov         dword ptr [ebp-0Ch],2					
	mov         dword ptr [ebp-8],3					
	mov         dword ptr [ebp-4],4					
	push        10h					
	push        offset string "%d\n" (0042201c)					
	call        printf (004010e0)					
	add         esp,8					
						
						
						
						
	mov         dword ptr [ebp-10h],1					
	mov         dword ptr [ebp-0Ch],2					
	mov         dword ptr [ebp-8],3					
	mov         dword ptr [ebp-4],4					
	push        10h					
	push        offset string "%d\n" (0042201c)					
	call        printf (004010e0)					
	add         esp,8					
					
###其实和上面一样，本质就是把父类元素复制到子元素的最前面
###这里也可以从汇编代码看出struct的汇编代码本质就是不同类型集合起来的数组

结论
--
1、什么是继承？				
				
>继承就是数据的复制				
				
2、为什么要用继承？				
				
>减少重复代码的编写				
				
3、Person 称为父类或者基类				
				
4、Teacher、Student称为子类或者派生类				
				
5、t和s可以称为对象或者实例.				
				
6、可以用父类指针指向子类的对象.				
				
