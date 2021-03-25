



	
	struct Person		
	{		
		int age;	
		int level;	
		Person()	
		{	
			printf("Person对象创建了\n");
		}	
		Person(int age,int level)	
		{	
			this->age = age;
			this->level = level;
		}	
		Person(int x,int y,int z)	
		{	
			this->age = x;
			this->level = y + z;
		}	
		void Print()	
		{	
			printf("%d-%d\n",age,level);
		}	
	};		
	int main(int argc, char* argv[])					
	{					
						
		Person p(1,3);
		p.Print();
		Person p1(2,6,4);
		p1.Print();
		return 0;				
	}
	33:		
	34:       Person p(1,3);		
	0040D7B8   push        3		
	0040D7BA   push        1		
	0040D7BC   lea         ecx,[ebp-8]		
	0040D7BF   call        @ILT+0(Person::Person) (00401005)		
	35:       p.Print();		
	0040D7C4   lea         ecx,[ebp-8]		
	0040D7C7   call        @ILT+10(Person::Print) (0040100f)		
	36:       Person p1(2,6,4);		
	0040D7CC   push        4		
	0040D7CE   push        6		
	0040D7D0   push        2		
	0040D7D2   lea         ecx,[ebp-10h]		
	0040D7D5   call        @ILT+15(Person::Person) (00401014)		
	37:       p1.Print();		
	0040D7DA   lea         ecx,[ebp-10h]		
	0040D7DD   call        @ILT+10(Person::Print) (0040100f)	

构造函数的特点
=====							
						
>1、与类同名						
						
>2、没有返回值						
						
>3、创建对象的时候执行						
						
>4、主要用于初始化						
						
>5、可以有多个(最好有一个无参的),称为重载 其他函数也可以重载						
						
>6、编译器不要求必须提供						



	struct Person			
	{			
		int age;		
		int level;		
		char* arr;		
		Person()		
		{		
			printf("无参构造函数执行了...");	
		}		
		Person(int age,int level)		
		{		
			printf("有参构造函数执行了...");	
			this->age = age;	
			this->level = level;	
			arr = (char*)malloc(1024);	
		}		
		~Person()		
		{		
			printf("析构函数执行了...");	
			free(arr);	
			arr = NULL;	
		}		
		void Print()		
		{		
			printf("%d-%d\n",age,level);	
		}		
	};			
	void func()
	{
		Person p(1,3);	
	}		

析构函数的特点：	
===		
			
>1、只能有一个析构函数，不能重载			
			
>2、不能带任何参数			
			
>3、不能带返回值			
			
>4、主要用于清理工作			
			
>5、编译器不要求必须提供			

>6.析构函数名称与类同名

------

##继承									
										
	1、观察反汇编									
										
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
										
										
	Teacher t;								观察反汇编：					
										
	t.age = 1;								mov         dword ptr [ebp-10h],1					
	t.sex = 2;								mov         dword ptr [ebp-0Ch],2					
	t.level = 3;							mov         dword ptr [ebp-8],3					
	t.classId = 4;							mov         dword ptr [ebp-4],4					
											push        10h					
	printf("%d\n",sizeof(t));				push        offset string "%d\n" (0042201c)					
											call        printf (004010e0)					
											add         esp,8					
										
										
										
	Student s;								mov         dword ptr [ebp-10h],1					
											mov         dword ptr [ebp-0Ch],2					
	s.age = 1;								mov         dword ptr [ebp-8],3					
	s.sex = 2;								mov         dword ptr [ebp-4],4					
	s.code = 3;								push        10h					
	s.score = 4;							push        offset string "%d\n" (0042201c)					
											call        printf (004010e0)					
	printf("%d\n",sizeof(s));				add         esp,8					
										
										
										
	2、改变写法：									
										
										
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
											观察反汇编:					
										
	Teacher t;								mov         dword ptr [ebp-10h],1					
											mov         dword ptr [ebp-0Ch],2					
	t.age = 1;								mov         dword ptr [ebp-8],3					
	t.sex = 2;								mov         dword ptr [ebp-4],4					
	t.level = 3;							push        10h					
	t.classId = 4;							push        offset string "%d\n" (0042201c)					
											call        printf (004010e0)					
	printf("%d\n",sizeof(t));				add         esp,8					
										
										
										
										
	Student s;								mov         dword ptr [ebp-10h],1					
											mov         dword ptr [ebp-0Ch],2					
	s.age = 1;								mov         dword ptr [ebp-8],3					
	s.sex = 2;								mov         dword ptr [ebp-4],4					
	s.code = 3;								push        10h					
	s.score = 4;							push        offset string "%d\n" (0042201c)					
											call        printf (004010e0)					
	printf("%d\n",sizeof(s));				add         esp,8					
									
									
总结：	
===								
									
>1、什么是继承？									
									
>继承就是数据的复制									
									
>2、为什么要用继承？									
									
>减少重复代码的编写									
									
>3、Person 称为父类或者基类									
									
>4、Teacher、Student称为子类或者派生类									
									
>5、t和s可以称为对象或者实例.									
									
>6、可以用父类指针指向子类的对象.									
