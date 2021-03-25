#c++析构函数

	struct Person
	{
		int age;
		int level;
		char* parr;
		Person()
		{
			printf("构造函数执行了...");
			age  = 12;
			level = 12;
			parr = (char*)malloc(10);
		}
		~Person()
		{
			printf("析构函数执行了...");
			free(parr);
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
		return 0;				//testfunc函数执行完毕后系统自动执行析构函数
	}


总结：
-------
析构函数的特点：			
			
1、只能有一个析构函数，不能重载			
			
2、不能带任何参数			
			
3、不能带返回值			
			
4、主要用于清理工作			
			
5、编译器不要求必须提供			
