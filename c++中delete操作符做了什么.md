#C++中delete操作符做了什么

我们知道C++中delete操作符会先调用析构函数在调用operator delete释放内存空间。那么delete操作符是如何知道要释放的空间是一个对象还是一个对象数组呢？这是因为当使用delete和delete[]编译器传入的参数是不同的。

具体情况是：1表示释放单个对象，3表示释放对象数组，0表示仅仅执行析构函数，不释放堆空间

 下面从汇编代码的角度分析一下不同情况的执行流程。

##1. delete单个对象

	class Derive
	{
	public:
	  Derive(){}
	  ~Derive(){}
	  int mem;
	};
	int _tmain(int argc, _TCHAR* argv[])
	{
	  Derive* pd=new Derive();
	  delete pd;
	}

-  对应汇编代码：

		int _tmain(int argc, _TCHAR* argv[])
		{
		004113C0 push ebp 
		004113C1 mov ebp,esp 
		004113C3 push 0FFFFFFFFh 
		004113C5 push offset __ehhandler$_wmain (4145FEh) 
		004113CA mov eax,dword ptr fs:[00000000h] 
		004113D0 push eax 
		004113D1 sub esp,100h 
		004113D7 push ebx 
		004113D8 push esi 
		004113D9 push edi 
		004113DA lea edi,[ebp-10Ch] 
		004113E0 mov ecx,40h 
		004113E5 mov eax,0CCCCCCCCh 
		004113EA rep stos dword ptr es:[edi] //初始化调试信息
		004113EC mov eax,dword ptr [___security_cookie (418000h)]//金丝雀用于检测栈溢出 
		004113F1 xor eax,ebp 
		004113F3 push eax 
		004113F4 lea eax,[ebp-0Ch] 
		004113F7 mov dword ptr fs:[00000000h],eax 
		    50: Derive* pd=new Derive();
		004113FD push 4 //new的参数
		004113FF call operator new (411190h) //调用operator new分配4字节空间
		00411404 add esp,4 //平衡栈
		00411407 mov dword ptr [ebp-0F8h],eax //new的返回值在eax中(即分配空间的首地址)
		0041140D mov dword ptr [ebp-4],0 
		00411414 cmp dword ptr [ebp-0F8h],0 //检查new的返回值 
		0041141B je wmain+70h (411430h) 
		0041141D mov ecx,dword ptr [ebp-0F8h] //new的返回值赋给ecx(即对象首地址)
		00411423 call Derive::Derive (4110DCh) //调用Derive()构造函数 
		00411428 mov dword ptr [ebp-10Ch],eax //ebx-10Ch存放对象首地址
		0041142E jmp wmain+7Ah (41143Ah) 
		00411430 mov dword ptr [ebp-10Ch],0 
		0041143A mov eax,dword ptr [ebp-10Ch] //对象首地址赋给eax
		00411440 mov dword ptr [ebp-104h],eax //ebx-104h存放对象首地址
		00411446 mov dword ptr [ebp-4],0FFFFFFFFh 
		0041144D mov ecx,dword ptr [ebp-104h] 
		00411453 mov dword ptr [ebp-14h],ecx //ebx-14h存放对象首地址
		    51: delete pd;
		00411456 mov eax,dword ptr [ebp-14h] 
		00411459 mov dword ptr [ebp-0E0h],eax 
		0041145F mov ecx,dword ptr [ebp-0E0h] //再次将对象首地址存入ecx
		00411465 mov dword ptr [ebp-0ECh],ecx 
		0041146B cmp dword ptr [ebp-0ECh],0 //检查对象首地址是否为空
		00411472 je wmain+0C9h (411489h) 
		00411474 push 1 //压入delete参数，1表示执行完析构函数后就会调用operator delete释放空间
		00411476 mov ecx,dword ptr [ebp-0ECh] //对象首地址存入ecx
		0041147C call Derive::'scalar deleting destructor' (4110BEh) //调用析构代理函数
		00411481 mov dword ptr [ebp-10Ch],eax //将返回值给ebp-10Ch
		00411487 jmp wmain+0D3h (411493h) 
		00411489 mov dword ptr [ebp-10Ch],0 
		 }
		00411493 xor eax,eax 
		00411495 mov ecx,dword ptr [ebp-0Ch] 
		00411498 mov dword ptr fs:[0],ecx 
		0041149F pop ecx 
		004114A0 pop edi 
		004114A1 pop esi 
		004114A2 pop ebx 
		004114A3 add esp,10Ch 
		004114A9 cmp ebp,esp 
		004114AB call @ILT+325(__RTC_CheckEsp) (41114Ah) 
		004114B0 mov esp,ebp 
		004114B2 pop ebp 
		004114B3 ret

说明：这里调用的是“析构代理函数”而不是析构函数，为什么不直接使用析构函数呢？其中一个原因是：在某些情况下释放的对象不止一个，如果直接调用析构函数，则无法完成对象的析构。

-  析构代理函数
Derive::'scalar deleting destructor':

		00411540 push ebp 
		00411541 mov ebp,esp 
		00411543 sub esp,0CCh 
		00411549 push ebx 
		0041154A push esi 
		0041154B push edi 
		0041154C push ecx 
		0041154D lea edi,[ebp-0CCh] 
		00411553 mov ecx,33h 
		00411558 mov eax,0CCCCCCCCh 
		0041155D rep stos dword ptr es:[edi] //初始化调试信息
		0041155F pop ecx //取出对象首地址到ecx中
		00411560 mov dword ptr [ebp-8],ecx 
		00411563 mov ecx,dword ptr [this] 
		00411566 call Derive::~Derive (41109Bh) //调用析构函数
		0041156B mov eax,dword ptr [ebp+8] 
		0041156E and eax,1 //测试析构代理函数的参数是否为1，如果为1则执行operator delete调用释放内存空间
		00411571 je Derive::'scalar deleting destructor'+3Fh (41157Fh) 
		00411573 mov eax,dword ptr [this] 
		00411576 push eax //压入对象首地址作为operator delete的参数
		00411577 call operator delete (411087h) //调用operator delete释放空间
		0041157C add esp,4 
		0041157F mov eax,dword ptr [this] 
		00411582 pop edi 
		00411583 pop esi 
		00411584 pop ebx 
		00411585 add esp,0CCh 
		0041158B cmp ebp,esp 
		0041158D call @ILT+325(__RTC_CheckEsp) (41114Ah) 
		00411592 mov esp,ebp 
		00411594 pop ebp 
		00411595 ret 4

- 析构函数 ~Derive(){}

		004115B0 push ebp 
		004115B1 mov ebp,esp 
		004115B3 sub esp,0CCh 
		004115B9 push ebx 
		004115BA push esi 
		004115BB push edi 
		004115BC push ecx 
		004115BD lea edi,[ebp-0CCh] 
		004115C3 mov ecx,33h 
		004115C8 mov eax,0CCCCCCCCh 
		004115CD rep stos dword ptr es:[edi] //初始化调试信息
		004115CF pop ecx //取出对象首地址到ecx
		004115D0 mov dword ptr [ebp-8],ecx 
		004115D3 pop edi 
		004115D4 pop esi 
		004115D5 pop ebx 
		004115D6 mov esp,ebp 
		004115D8 pop ebp 
		004115D9 ret

##2. 只调用析构函数不释放空间

		int _tmain(int argc, _TCHAR* argv[])
		{
		  Derive* pd=new Derive();
		  pd->~Derive();//只调用析构函数不释放空间
		}

- 对象汇编代码  pd->~Derive();
		
		
		00411456 push 0 //压入0表示只调用析构函数不调用operator delete释放空间
		00411458 mov ecx,dword ptr [ebp-14h] 
		0041145B call Derive::'scalar deleting destructor' (4110BEh)

##3. delete数组

		int _tmain(int argc, _TCHAR* argv[])
		{
		  Derive* pd=new Derive[2];
		  delete []pd;
		}

- 对应汇编代码
	
		int _tmain(int argc, _TCHAR* argv[])
		{
		004113C0 push ebp 
		004113C1 mov ebp,esp 
		004113C3 push 0FFFFFFFFh 
		004113C5 push offset __ehhandler$_wmain (4145FEh) 
		004113CA mov eax,dword ptr fs:[00000000h] 
		004113D0 push eax 
		004113D1 sub esp,100h 
		004113D7 push ebx 
		004113D8 push esi 
		004113D9 push edi 
		004113DA lea edi,[ebp-10Ch] 
		004113E0 mov ecx,40h 
		004113E5 mov eax,0CCCCCCCCh 
		004113EA rep stos dword ptr es:[edi] //加入调试信息
		004113EC mov eax,dword ptr [___security_cookie (418000h)] //金丝雀防止栈溢出修改
		004113F1 xor eax,ebp 
		004113F3 push eax 
		004113F4 lea eax,[ebp-0Ch] 
		004113F7 mov dword ptr fs:[00000000h],eax 
		    50: Derive* pd=new Derive[2];
		004113FD push 0Ch //传入operator new[]的参数12，即要分配12字节空间，首个4字节存放数组元素个数，后面八个字节存放两个对象
		004113FF call operator new[] (4111E5h) //调用operator new[] 
		00411404 add esp,4 
		00411407 mov dword ptr [ebp-0F8h],eax //eax存放的operator new[]的返回值 
		0041140D mov dword ptr [ebp-4],0 
		00411414 cmp dword ptr [ebp-0F8h],0 //检验返回值是否为0(内存是否分配成功) 
		0041141B je wmain+97h (411457h) 
		0041141D mov eax,dword ptr [ebp-0F8h] 
		00411423 mov dword ptr [eax],2 //在分配空间的首4个自己存放数组元素个数2
		00411429 push offset Derive::~Derive (41109Bh) //析构函数地址入栈
		0041142E push offset Derive::Derive (4110DCh) //构造函数地址入栈
		00411433 push 2 //入栈数组元素个数
		00411435 push 4 //入栈元素大小
		00411437 mov ecx,dword ptr [ebp-0F8h] 
		0041143D add ecx,4 
		00411440 push ecx //入栈数组首元素地址，因为开始4字节存放数组元素个数，所以首元素存放于分配返回地址+4处
		00411441 call 'eh vector constructor iterator' (4111EAh) //调用构造代理函数
		00411446 mov edx,dword ptr [ebp-0F8h] 
		0041144C add edx,4 
		0041144F mov dword ptr [ebp-10Ch],edx 
		00411455 jmp wmain+0A1h (411461h) 
		00411457 mov dword ptr [ebp-10Ch],0 
		00411461 mov eax,dword ptr [ebp-10Ch] 
		00411467 mov dword ptr [ebp-104h],eax 
		0041146D mov dword ptr [ebp-4],0FFFFFFFFh 
		00411474 mov ecx,dword ptr [ebp-104h] 
		0041147A mov dword ptr [ebp-14h],ecx //ecx存放数组元素首元素的内存位置(分配空间+4处)
		 delete []pd;
		0041147D mov eax,dword ptr [ebp-14h] 
		00411480 mov dword ptr [ebp-0E0h],eax 
		00411486 mov ecx,dword ptr [ebp-0E0h] 
		0041148C mov dword ptr [ebp-0ECh],ecx 
		00411492 cmp dword ptr [ebp-0ECh],0 
		00411499 je wmain+0F0h (4114B0h) 
		0041149B push 3 //3表示释放对象为数组
		0041149D mov ecx,dword ptr [ebp-0ECh] 
		004114A3 call Derive::'vector deleting destructor' (4111E0h) //调用数组析构代理函数
		004114A8 mov dword ptr [ebp-10Ch],eax 
		004114AE jmp wmain+0FAh (4114BAh) 
		004114B0 mov dword ptr [ebp-10Ch],0 
		 }
		004114BA xor eax,eax 
		004114BC mov ecx,dword ptr [ebp-0Ch] 
		004114BF mov dword ptr fs:[0],ecx 
		004114C6 pop ecx 
		004114C7 pop edi 
		004114C8 pop esi 
		004114C9 pop ebx 
		004114CA add esp,10Ch 
		004114D0 cmp ebp,esp 
		004114D2 call @ILT+325(__RTC_CheckEsp) (41114Ah) 
		004114D7 mov esp,ebp 
		004114D9 pop ebp 
		004114DA ret

##4. 释放数组过程分析

			分配空间起始位置           对象起始位置
					|						|
					v						v
					--------------------------------------------------------------------
					|						|						|					|
					|	对象个数2      		|		对象1			|		对象2			|
					-------------------------------------------------------------------
由于创建数组和释放数组涉及汇编代码较多，这里不再列出，只将new[]和delete[]的执行过程大致阐述如下。

如图1所示。当执行Derive* pd=new Derive[2]具体步骤如下：

(1) 调用operator new分配12字节空间，其中前4字节用于保存数组对象个数，后面两个4字节分别用于存放两个对象(主要Derive对象的大小为4字节)。

(2) 设置分配空间的前4个字节为对象个数。

(3) 调用堆对象构造代理函数，构造代理函数有5个参数：第一个对象空间的首地址，每个对象占用的内存空间大小，数组对象个数，对象构造函数地址，对象析构函数地址。代理构造函数依次对每个对象空间调用构造函数。（注意，堆对象只对图1中绿色部分进行操作）

注意：pd的值是对象起始位置而不是分配空间的起始位置。


当执行 delete []pd;时具体步骤如下：

(1) 压入标志3（表示要释放的是对象数组），压入要释放堆对象的首地址，即图中绿色部分开始部分。

(2) 调用堆对象释放函数，该函数中获取数组对象的个数，每个对象的大小，第一个对象的起始地址，然后:

 a. 调用析构代理函数，析构代理函数一次对图中绿色部分每个对象空间调用析构函数。

 b. 修正对空间首地址，由于之前析构代理函数操作的是图中绿色部分，使用的参数是对象起始位置，而接下来调用operator delete释放空间要包括之前的4自己空间，所以要将首地址调整到分配空间的起始位置。

 c. 调用operator delete释放堆空间。


说明：我们注意到delete[]执行中有向前调整4字节释放空间空间的操作，所以如果对象是new[]创建的，但是是用delete删除的，由于没有调要释放整堆空间就会造成错误。 有的人会问，为什么delete的结果不是只把对象1释放，这是因为操作系统在申请空间时在空间的前端记录了维护空间的信息，包括指向其他空间链表的指针。释放空间时也需要根据这些信息进行空间的合并等处理，所以如果释放时传入的地址不是申请空间得到的首地址就会无法找到这些记录信息，就会出错。


