#C++中堆对象的构造函数和析构函数逆向分析

- 实验环境：
- 操作系统：Windows XP Professional Service Pack 3
- 集成开发环境：Microsoft Visual C++ 6.0
- 构建版本：Debug版本


实验代码：

	 1 #include <stdio.h>
	 2 
	 3 class People
	 4 {
	 5 public:
	 6     People() { printf("Constructor!\n"); }
	 7     ~People() { printf("Destructor!\n"); }
	 8 };
	 9 
	10 int main(int argc, char **argv, const char **envp)
	11 {
	12     People *p = new People;
	13     delete p;
	14 
	15     return 0;
	16 } 

下面先来看在堆上构造一个People对象的汇编代码，这里有一个需要注意的是，虽然People类的定义中没有定义数据成员，表象上占用空间为0，但是实际上需要分配1个字节的空间，否则就无法获取对象实例的地址了，this指针也会失效，导致不能被实例化，所以在调用new函数的时候push了1个字节，用来表征分配1个字节的堆空间。

	12:       People *p = new People;
	           ; 压入待分配的类对象所需要的空间　　
	0040105D   push        1
	           ; 调用new函数进行分配堆空间
	0040105F   call        operator new (004012b0)
	           ; 由于new函数是__cdecl调用类型，所以有调用者负责恢复堆栈平衡
	00401064   add         esp,4
	           ; 将分配的堆地址存放到临时变量[ebp - 18h]中
	00401067   mov         dword ptr [ebp-18h],eax
	           ; [ebp - 4]保存申请堆空间的次数
	0040106A   mov         dword ptr [ebp-4],0
	           ; 检测堆空间是否申请成功
	00401071   cmp         dword ptr [ebp-18h],0
	           ; 如果申请失败的话，就跳过构造函数，跳转到0x401084将存放this指针的临时变量赋值为0
	00401075   je          main+54h (00401084)
	           ; 将分配的堆空间的首地址传入ecx中
	00401077   mov         ecx,dword ptr [ebp-18h]
	           ; 调用构造函数
	0040107A   call        @ILT+15(People::People) (00401014)
	           ; 构造函数调用返回this指针，将this指针传入eax中，此处又将this指针传入临时变量[ebp - 24h]中
	0040107F   mov         dword ptr [ebp-24h],eax
	           ; 跳过new操作分配堆空间失败的赋值操作
	00401082   jmp         main+5Bh (0040108b)
	           ; 如果分配失败的话，就会把临时变量[ebp - 24h]赋值为0
	00401084   mov         dword ptr [ebp-24h],0
	           ; 将[ebp - 24h]重新传入eax中，如果为0，则代表堆空间分配失败，否则分配成功
	0040108B   mov         eax,dword ptr [ebp-24h]
	           ; 下面几步操作最终将this指针保存到了变量[ebp - 14h]中，也就是我们的变量p
	0040108E   mov         dword ptr [ebp-14h],eax
	00401091   mov         dword ptr [ebp-4],0FFFFFFFFh
	00401098   mov         ecx,dword ptr [ebp-14h]
	0040109B   mov         dword ptr [ebp-10h],ecx

接下来再看一下单个对象析构函数的调用的汇编代码，这里有一个需要注意的是，delete一个堆对象的时候不是简单的调用析构函数，而是调用了一个析构代理函数，原因有很多，其中一个原因是有时候并不仅仅只销毁一个对象，还有可能使用delete[]销毁多个对象。

	13:       delete p;
	           ; 下面几步操作将变量p的内容传入了临时变量[ebp - 1Ch]中
	0040109E   mov         edx,dword ptr [ebp-10h]
	004010A1   mov         dword ptr [ebp-20h],edx
	004010A4   mov         eax,dword ptr [ebp-20h]
	004010A7   mov         dword ptr [ebp-1Ch],eax
	           ; 判断变量p的内容是否为0
	004010AA   cmp         dword ptr [ebp-1Ch],0
	           ; 如果为0, 则直接跳过析构函数的调用
	004010AE   je          main+8Fh (004010bf)
	           ; 压入释放对象类型标志, 1为单个对象, 3为释放对象数组, 0表示仅仅执行析构函数, 不释放堆空间(与多重继承有关)
	004010B0   push        1
	           ; 将对象指针传入ecx中
	004010B2   mov         ecx,dword ptr [ebp-1Ch]
	           ; 这里不是直接调用析构函数, 而是调用了一个叫做'scalar deleting destructor'的析构代理函数, 下面将要介绍
	004010B5   call        @ILT+0(People::`scalar deleting destructor') (00401005)
	           ; 将被销毁的对象的首地址传入临时变量[ebp - 28h]中
	004010BA   mov         dword ptr [ebp-28h],eax
	           ; 处理成功, 跳过失败处理
	004010BD   jmp         main+96h (004010c6)
	           ; 如果析构失败, 则将临时变量[ebp - 28h]赋值为0
	004010BF   mov         dword ptr [ebp-28h],0 

跟进析构代理函数看看，看上去好像也仅仅是在delete函数的基础上加了调用析构函数，不过析构代理函数在析构多个对象的时候会有很大的不同。

	People::`scalar deleting destructor':
	           ; 下面几步操作属于函数的入口操作, 和我们的分析无关, 忽略
	00401160   push        ebp
	00401161   mov         ebp,esp
	00401163   sub         esp,44h
	00401166   push        ebx
	00401167   push        esi
	00401168   push        edi
	00401169   push        ecx
	0040116A   lea         edi,[ebp-44h]
	0040116D   mov         ecx,11h
	00401172   mov         eax,0CCCCCCCCh
	00401177   rep stos    dword ptr [edi]
	           ; 将this指针从栈中弹到ecx中
	00401179   pop         ecx
	           ; 将this指针存放到临时变量[ebp - 4]中
	0040117A   mov         dword ptr [ebp-4],ecx
	           ; 将this指针又赋值给了ecx, 重复操作
	0040117D   mov         ecx,dword ptr [ebp-4]
	           ; 调用析构函数, 这里使用寄存器ecx进行传递this指针
	00401180   call        @ILT+10(People::~People) (0040100f)
	           ; 获取释放标志
	00401185   mov         eax,dword ptr [ebp+8]
	           ; 检查是否释放堆空间
	00401188   and         eax,1
	0040118B   test        eax,eax
	0040118D   je          People::`scalar deleting destructor'+3Bh (0040119b)
	           ; ecx保存了对象的首地址
	0040118F   mov         ecx,dword ptr [ebp-4]
	           ; 压入this指针
	00401192   push        ecx
	           ; 调用delete函数
	00401193   call        operator delete (00401220)
	           ; 由于delete函数也是__cdecl调用类型, 所以也是由调用者进行堆栈平衡操作
	00401198   add         esp,4
	           ; 将已经被销毁的对象的首地址传入eax中
	0040119B   mov         eax,dword ptr [ebp-4]
	           ; 函数离开的收尾工作, 和我们的分析无关, 忽略
	0040119E   pop         edi
	0040119F   pop         esi
	004011A0   pop         ebx
	004011A1   add         esp,44h
	004011A4   cmp         ebp,esp
	004011A6   call        __chkesp (004016b0)
	004011AB   mov         esp,ebp
	004011AD   pop         ebp
	004011AE   ret         4

至于析构函数本身，倒是没有什么需要特别注意的点。

	7:        ~People() { printf("Destructor!\n"); }
	           ; 函数入口操作, 忽略
	004011D0   push        ebp
	004011D1   mov         ebp,esp
	004011D3   sub         esp,44h
	004011D6   push        ebx
	004011D7   push        esi
	004011D8   push        edi
	004011D9   push        ecx
	004011DA   lea         edi,[ebp-44h]
	004011DD   mov         ecx,11h
	004011E2   mov         eax,0CCCCCCCCh
	004011E7   rep stos    dword ptr [edi]
	           ; 将this指针存放到ecx寄存器中
	004011E9   pop         ecx
	           ; 将this指针传入临时变量[ebp - 4]
	004011EA   mov         dword ptr [ebp-4],ecx
	           ; 压入格式化字符串
	004011ED   push        offset string "Destructor!\n" (0042502c)
	           ; 调用printf函数
	004011F2   call        printf (004017e0)
	           ; 由于不定参数, 所以printf必须得是__cdecl调用类型, 由调用者进行堆栈平衡
	004011F7   add         esp,4
	           ; 函数收尾工作, 忽略
	004011FA   pop         edi
	004011FB   pop         esi
	004011FC   pop         ebx
	004011FD   add         esp,44h
	00401200   cmp         ebp,esp
	00401202   call        __chkesp (004016b0)
	00401207   mov         esp,ebp
	00401209   pop         ebp
	0040120A   ret 

##下面在看看多个对象的构造和析构情况，

实验代码：

	 1 #include <stdio.h>
	 2 
	 3 class People
	 4 {
	 5 public:
	 6     People() {}
	 7     ~People() {}
	 8 };
	 9 
	10 int main(int argc, char **argv, const char **envp)
	11 {
	12     People *p = new People[5];
	13     delete[] p;
	14 
	15     return 0;
	16 } 

下面看看分配多个对象的汇编代码，这里调用了一个构造代理函数。

	12:       People *p = new People[5];
	           ; 每个待分配的对象占用1个字节, 再加上堆空间的首地址处的4字节用于保存对象的个数
	0040105D   push        9
	           ; 调用new函数
	0040105F   call        operator new (004012b0)
	           ; 调用方负责堆栈平衡
	00401064   add         esp,4
	           ; eax保存分配的堆空间的首地址
	00401067   mov         dword ptr [ebp-18h],eax
	           ; [ebp - 4]保存申请堆空间的次数
	0040106A   mov         dword ptr [ebp-4],0
	           ; 判断是否分配成功, 如果分配失败的话就跳过构造代理函数
	00401071   cmp         dword ptr [ebp-18h],0
	00401075   je          main+75h (004010a5)
	           ; 压入析构函数的地址
	00401077   push        offset @ILT+10(People::~People) (0040100f)
	           ; 压入构造函数的地址
	0040107C   push        offset @ILT+15(People::People) (00401014)
	           ; 将分配的堆空间的首地址传入eax中
	00401081   mov         eax,dword ptr [ebp-18h]
	           ; 堆空间的首地址处的4字节用于存放申请的对象的个数
	00401084   mov         dword ptr [eax],5
	           ; 压入对象个数
	0040108A   push        5
	           ; 压入每个对象的大小
	0040108C   push        1
	           ; 将堆空间的首地址传入ecx中
	0040108E   mov         ecx,dword ptr [ebp-18h]
	           ; 跳过首地址处的4个用于存放大小的字节
	00401091   add         ecx,4
	           ; 压入ecx
	00401094   push        ecx
	           ; 调用构造代理函数
	00401095   call        `eh vector constructor iterator' (0040ee90)
	           ; 将首地址传入edx中
	0040109A   mov         edx,dword ptr [ebp-18h]
	           ; 跳过首地址处的4个用于存放大小的字节
	0040109D   add         edx,4
	           ; 将edx传入[ebp - 24h]中
	004010A0   mov         dword ptr [ebp-24h],edx
	           ; 跳过分配失败处理
	004010A3   jmp         main+7Ch (004010ac)
	           ; 分配失败处理
	004010A5   mov         dword ptr [ebp-24h],0
	           ; 最终将对象首地址存入[ebp - 10h]中
	004010AC   mov         eax,dword ptr [ebp-24h]
	004010AF   mov         dword ptr [ebp-14h],eax
	004010B2   mov         dword ptr [ebp-4],0FFFFFFFFh
	004010B9   mov         ecx,dword ptr [ebp-14h]
	004010BC   mov         dword ptr [ebp-10h],ecx

跟进构造代理函数看看

	??_L@YGXPAXIHP6EX0@Z1@Z:
	           ; 以下代码是函数入口的初始化和异常链的处理
	0040EE90   push        ebp
	0040EE91   mov         ebp,esp
	0040EE93   push        0FFh
	0040EE95   push        offset string "stream != NULL"+30h (00426068)
	0040EE9A   push        offset __except_handler3 (00407590)
	0040EE9F   mov         eax,fs:[00000000]
	0040EEA5   push        eax
	0040EEA6   mov         dword ptr fs:[0],esp
	0040EEAD   add         esp,0F0h
	0040EEB0   push        ebx
	0040EEB1   push        esi
	0040EEB2   push        edi
	0040EEB3   mov         dword ptr [ebp-20h],0
	0040EEBA   mov         dword ptr [ebp-4],0
	           ; 通过对下面代码的分析可以知道, [ebp - 1Ch]存放的是循环次数
	0040EEC1   mov         dword ptr [ebp-1Ch],0
	0040EEC8   jmp         `eh vector constructor iterator'+43h (0040eed3)
	           ; 将循环次数存入eax
	0040EECA   mov         eax,dword ptr [ebp-1Ch]
	           ; 将循环次数加1
	0040EECD   add         eax,1
	           ; 重新存回[ebp - 1Ch]中
	0040EED0   mov         dword ptr [ebp-1Ch],eax
	           ; 循环代码开始
	           ; 将循环次数传入ecx
	0040EED3   mov         ecx,dword ptr [ebp-1Ch]
	           ; 与对象个数进行比较, 即进行ecx - dword ptr [ebp + 10h]操作
	0040EED6   cmp         ecx,dword ptr [ebp+10h]
	           ; 如果大于等于0, 就离开循环代码块
	0040EED9   jge         `eh vector constructor iterator'+5Ch (0040eeec)
	           ; 将对象的首地址(已经跳过用于存放大小的4个字节)传入
	0040EEDB   mov         ecx,dword ptr [ebp+8]
	           ; 调用构造函数
	0040EEDE   call        dword ptr [ebp+14h]
	           ; 将对象的首地址传入eax
	0040EEE1   mov         edx,dword ptr [ebp+8]
	           ; 在上一个对象的首地址的基础上加上对象的大小(此处为1个字节)
	0040EEE4   add         edx,dword ptr [ebp+0Ch]
	           ; 将新的待调用构造函数的对象首地址存回[ebp + 8]中
	0040EEE7   mov         dword ptr [ebp+8],edx
	           ; 跳转回循环块首
	0040EEEA   jmp         `eh vector constructor iterator'+3Ah (0040eeca)
	           ; 剩余代码与我们的分析无关, 忽略
	0040EEEC   mov         dword ptr [ebp-20h],1
	0040EEF3   mov         dword ptr [ebp-4],0FFFFFFFFh
	0040EEFA   call        `eh vector constructor iterator'+71h (0040ef01)
	0040EEFF   jmp         `eh vector constructor iterator'+8Dh (0040ef1d)
	0040EF01   cmp         dword ptr [ebp-20h],0
	0040EF05   jne         `eh vector constructor iterator'+8Ch (0040ef1c)
	0040EF07   mov         eax,dword ptr [ebp+18h]
	0040EF0A   push        eax
	0040EF0B   mov         ecx,dword ptr [ebp-1Ch]
	0040EF0E   push        ecx
	0040EF0F   mov         edx,dword ptr [ebp+0Ch]
	0040EF12   push        edx
	0040EF13   mov         eax,dword ptr [ebp+8]
	0040EF16   push        eax
	0040EF17   call        __ArrayUnwind (004011a0)
	0040EF1C   ret
	0040EF1D   mov         ecx,dword ptr [ebp-10h]
	0040EF20   mov         dword ptr fs:[0],ecx
	0040EF27   pop         edi
	0040EF28   pop         esi
	0040EF29   pop         ebx
	0040EF2A   mov         esp,ebp
	0040EF2C   pop         ebp
	0040EF2D   ret         14h

调用析构代理函数处的代码与析构1个对象的代码基本一致，只是push的标志是3而不再是1了。
接下来到多个对象析构代理函数的内部去看看，这里调用了一个'eh vector destructor iterator'函数进行循环释放对象数组。

	People::`vector deleting destructor':
	           ; 函数入口代码, 与我们的分析无关, 忽略
	0040ED10   push        ebp
	0040ED11   mov         ebp,esp
	0040ED13   sub         esp,44h
	0040ED16   push        ebx
	0040ED17   push        esi
	0040ED18   push        edi
	0040ED19   push        ecx
	0040ED1A   lea         edi,[ebp-44h]
	0040ED1D   mov         ecx,11h
	0040ED22   mov         eax,0CCCCCCCCh
	0040ED27   rep stos    dword ptr [edi]
	           ; 将对象首地址弹到ecx中
	0040ED29   pop         ecx
	           ; 将首地址存放到[ebp - 4]中
	0040ED2A   mov         dword ptr [ebp-4],ecx
	           ; 取得标志
	0040ED2D   mov         eax,dword ptr [ebp+8]
	           ; 做按位与操作
	0040ED30   and         eax,2
	           ; 如果为1, 就跳转到0x0040ED6F处只执行一次析构函数
	0040ED33   test        eax,eax
	0040ED35   je          People::`vector deleting destructor'+5Fh (0040ed6f)
	           ; 否则压入析构函数的地址
	0040ED37   push        offset @ILT+10(People::~People) (0040100f)
	           ; 将对象首地址传入ecx中
	0040ED3C   mov         ecx,dword ptr [ebp-4]
	           ; 将对象的个数存入edx中
	0040ED3F   mov         edx,dword ptr [ecx-4]
	           ; 压入对象的个数
	0040ED42   push        edx
	           ; 压入对象的大小
	0040ED43   push        1
	           ; 将对象的首地址传入eax中
	0040ED45   mov         eax,dword ptr [ebp-4]
	           ; 压入对象首地址
	0040ED48   push        eax
	           ; 调用函数进行循环析构多个对象
	0040ED49   call        `eh vector destructor iterator' (0040edb0)
	           ; 获取释放标志
	0040ED4E   mov         ecx,dword ptr [ebp+8]
	           ; 检查是否释放堆空间
	0040ED51   and         ecx,1
	0040ED54   test        ecx,ecx
	0040ED56   je          People::`vector deleting destructor'+57h (0040ed67)
	           ; edx中保留了对象数组的首地址
	0040ED58   mov         edx,dword ptr [ebp-4]
	           ; 修正为正确的包含对象个数的首地址, 因为即将调用delete函数, 而delete函数并没有记录对象的个数
	0040ED5B   sub         edx,4
	0040ED5E   push        edx
	           ; 调用delete函数
	0040ED5F   call        operator delete (00401220)
	           ; 堆栈平衡
	0040ED64   add         esp,4
	           ; 修正为正确的包含对象个数的首地址, 并存入eax中
	0040ED67   mov         eax,dword ptr [ebp-4]
	0040ED6A   sub         eax,4
	0040ED6D   jmp         People::`vector deleting destructor'+80h (0040ed90)
	           ; 这里是只有一个对象的析构过程
	           ; ecx中存放该对象的首地址
	0040ED6F   mov         ecx,dword ptr [ebp-4]
	           ; 调用析构函数
	0040ED72   call        @ILT+10(People::~People) (0040100f)
	           ; 获取释放标志
	0040ED77   mov         eax,dword ptr [ebp+8]
	           ; 检查是否释放堆空间
	0040ED7A   and         eax,1
	0040ED7D   test        eax,eax
	           ; 如果释放就跳转
	0040ED7F   je          People::`vector deleting destructor'+7Dh (0040ed8d)
	           ; 否则压入单个对象的首地址
	0040ED81   mov         ecx,dword ptr [ebp-4]
	0040ED84   push        ecx
	           ; 调用delete函数
	0040ED85   call        operator delete (00401220)
	           ; 堆栈平衡
	0040ED8A   add         esp,4
	           ; eax中存放该对象的首地址
	0040ED8D   mov         eax,dword ptr [ebp-4]
	           ; 函数收尾代码, 不做分析
	0040ED90   pop         edi
	0040ED91   pop         esi
	0040ED92   pop         ebx
	0040ED93   add         esp,44h
	0040ED96   cmp         ebp,esp
	0040ED98   call        __chkesp (004016b0)
	0040ED9D   mov         esp,ebp
	0040ED9F   pop         ebp
	0040EDA0   ret         4

跟进'eh vector destructor iterator'函数中。

	??_M@YGXPAXIHP6EX0@Z@Z:
	           ; 以下是函数入口的初始化和异常链的处理
	0040EDB0   push        ebp
	0040EDB1   mov         ebp,esp
	0040EDB3   push        0FFh
	0040EDB5   push        offset string "stream != NULL"+10h (00426048)
	0040EDBA   push        offset __except_handler3 (00407590)
	0040EDBF   mov         eax,fs:[00000000]
	0040EDC5   push        eax
	0040EDC6   mov         dword ptr fs:[0],esp
	0040EDCD   add         esp,0F4h
	0040EDD0   push        ebx
	0040EDD1   push        esi
	0040EDD2   push        edi
	           ; 通过对下面代码的分析可以知道, [ebp - 1Ch]中用于存放循环的次数
	0040EDD3   mov         dword ptr [ebp-1Ch],0
	           ; eax中存放每个对象的大小
	0040EDDA   mov         eax,dword ptr [ebp+0Ch]
	           ; 乘上对象的个数, eax中存放总共的字节大小
	0040EDDD   imul        eax,dword ptr [ebp+10h]
	           ; ecx中存放对象的首地址
	0040EDE1   mov         ecx,dword ptr [ebp+8]
	           ; ecx中存放最后一个对象的下一个地址
	0040EDE4   add         ecx,eax
	           ; 存放到[ebp + 8]中
	0040EDE6   mov         dword ptr [ebp+8],ecx
	           ; 保存申请堆空间的次数
	0040EDE9   mov         dword ptr [ebp-4],0
	           ; edx中存放对象的个数
	0040EDF0   mov         edx,dword ptr [ebp+10h]
	           ; edx减1
	0040EDF3   sub         edx,1
	           ; 重新放回[ebp + 10h]中
	0040EDF6   mov         dword ptr [ebp+10h],edx
	           ; 检测是否小于0, 如果小于0, 就结束循环
	0040EDF9   cmp         dword ptr [ebp+10h],0
	0040EDFD   jl          `eh vector destructor iterator'+60h (0040ee10)
	           ; 以下两步操作将eax指向即将被析构的对象的首地址(从后往前进行析构)
	0040EDFF   mov         eax,dword ptr [ebp+8]
	0040EE02   sub         eax,dword ptr [ebp+0Ch]
	           ; 以下两步操作将待析构的对象首地址存入ecx中, 并且更新[ebp + 8]中的地址, 用于析构下一个对象
	0040EE05   mov         dword ptr [ebp+8],eax
	0040EE08   mov         ecx,dword ptr [ebp+8]
	           ; 调用析构函数
	0040EE0B   call        dword ptr [ebp+14h]
	           ; 重新跳回析构循环中
	0040EE0E   jmp         `eh vector destructor iterator'+40h (0040edf0)
	           ; 剩余代码与我们的分析无关, 这里不考究
	0040EE10   mov         dword ptr [ebp-1Ch],1
	0040EE17   mov         dword ptr [ebp-4],0FFFFFFFFh
	0040EE1E   call        `eh vector destructor iterator'+75h (0040ee25)
	0040EE23   jmp         `eh vector destructor iterator'+91h (0040ee41)
	0040EE25   cmp         dword ptr [ebp-1Ch],0
	0040EE29   jne         `eh vector destructor iterator'+90h (0040ee40)
	0040EE2B   mov         ecx,dword ptr [ebp+14h]
	0040EE2E   push        ecx
	0040EE2F   mov         edx,dword ptr [ebp+10h]
	0040EE32   push        edx
	0040EE33   mov         eax,dword ptr [ebp+0Ch]
	0040EE36   push        eax
	0040EE37   mov         ecx,dword ptr [ebp+8]
	0040EE3A   push        ecx
	0040EE3B   call        __ArrayUnwind (004011a0)
	0040EE40   ret
	0040EE41   mov         ecx,dword ptr [ebp-10h]
	0040EE44   mov         dword ptr fs:[0],ecx
	0040EE4B   pop         edi
	0040EE4C   pop         esi
	0040EE4D   pop         ebx
	0040EE4E   mov         esp,ebp
	0040EE50   pop         ebp
	0040EE51   ret         10h  