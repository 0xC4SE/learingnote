#宽字节

## 打印的环境设置

想在win32程序中打印需要在头文件添加以下代码

	.h中添加
	#include <stdio.h>
	#ifdef _DEBUG  
	#define dbgprintf   OutputDebugStringF  
	#else  
	#define dbgprintf  
	#endif 
	.cpp添加以下代码
	void __cdecl OutputDebugStringF(const char *format, ...)  
	{  
	    va_list vlArgs;  
	    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);  
	
	    va_start(vlArgs, format);  
	    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);  
	    va_end(vlArgs);  
	    strcat(strBuffer, "\n");  
	    OutputDebugStringA(strBuffer);  
	    GlobalFree(strBuffer);  
	    return;  
	}  


使用时就需要按以下格式

	dbgprintf("this is error! %x",x);

## 需要打印unicode宽字符时

	#include <locale.h>
	int main(){
	setlocale(LC_ALL,"");
	char x[] = "中国";						
							
	wchar_t x1[] = L"中国";						
							
	printf("%s\n",x);			
							
	wprintf(L"%s\n",x1);
	}


首先 setlocale(...) 是啥？
setLocale(...) 是C库中的一个设置地域化信息的C函数。

函数原型为：

	char *setlocale(int category, const char *locale)

* 参数解释：

### category – 这是一个已命名的常量，指定了受区域设置影响的函数类别，可以设置为以下值。

	LC_ALL // 包括下面的所有选项。
	LC_COLLATE // 字符串比较。参见 strcoll()。
	LC_CTYPE // 字符分类和转换。例如 strtoupper()。
	LC_MONETARY // 货币格式，针对 localeconv()。
	LC_NUMERIC // 小数点分隔符，针对 localeconv()。
	LC_TIME // 日期和时间格式，针对 strftime()。
	LC_MESSAGES // 系统响应。

### locale – 如果 locale是 NULL 或空字符串 ""，则区域名称将根据环境变量值来设置，其名称与上述的类别名称相同。

* 一般程序遇到编码方面的问题，百度都会告诉你设置setlocale(LC_ALL,"")来解决，下面我们就了解一下这句代码做了什么.


* LC_ALL，LE _* 又是个啥？

实际上 LC_ALL，LE_* 是 区域 相关的环境变量。简而言之，LC_COLLATE，LC_TYPE等等都是环境变量。
很多程序的运行行为在全球不同的地域、语言环境中是不一样的，例如Linux 中的 date命令的行为就受LC_TIME的影响：

	$ LC_TIME=en_US.UTF-8 date
	Fri Oct 31 19:51:16 CST 2014
	$ LC_TIME=fi_FI.UTF-8 date
	pe 31.10.2014 19.52.00 +0800
	$ LC_TIME=zh_CN.UTF-8 date
	2014年 10月 31日 星期五 19:53:07 CST

### 因此自然需要一些环境变量来告诉计算机用户所处的地区、语言等等，而locale是用来设置、查询、管理这些变量的linux命令。

* 那么 LE_* 都包含啥？

首先来看一些都有哪些需要设置的变量，通过在 linux 系统命令行中输入locale 就可以打印所有相关的环境变量以及对应的值。

一般会包括：

	LC_CTYPE – 语言符号及其分类
	LC_NUMERIC – 数字
	LC_COLLATE – 比较和排序习惯
	LC_TIME – 时间显示格式
	LC_MONETARY – 货币单位
	LC_MESSAGES – 信息主要是提示信息,错误信息,状态信息,标题,标签,按钮和菜单等
	LC_NAME – 姓名书写方式
	LC_ADDRESS – 地址书写方式
	LC_TELEPHONE – 电话号码书写方式
	LC_MEASUREMENT – 度量衡表达方式
	LC_PAPER – 默认纸张尺寸大小
	LC_IDENTIFICATION – 对locale自身包含信息的概述

一般认为由于区域和语言导致的不同习惯大致就是这12类。但是你从 linux 系统命令中输入 locale 获取到的结果可能不止这12个，还有LANG，LANGUAGE和LC_ALL，那是这三个比较特殊。

	LANG – 是一个缺省值，所有没有显式设置值的LC_*变量都会取LANG的值。
	LANGUAGE – 的优先级比较高，会覆盖所有设置过的LC_*的值
	LC_ALL – 并不是一个环境变量，而是一个glibc中定义的一个宏，实际上则是调用了setlocale把所有的LC_*的变量设置了一遍


## C语言中的宽字符和多字符							
							
							
	char		wchar_t		//多字节字符类型   宽字符类型			
								
	printf		wprintf		//打印到控制台函数			
								
	strlen		wcslen		//获取长度			
								
	strcpy		wcscpy		//字符串复制			
								
	strcat		wcscat		//字符串拼接			
								
	strcmp		wcscmp		//字符串比较			
								
	strstr		wcsstr		//字符串查找			

3、Win32 API中的宽字符和多字节字符				
				
Windows是使用C语言开发的，Win32 API同时支持宽字符与多字节字符.				
				
 (1) 字符类型--------------- (2) 字符串指针	
				
	char 	CHAR		PSTR(LPSTR) 指向多字节字符串	
					
	wchar_t  WCHAR		PWSTR(LPWSTR) 指向宽字符串	
					
	宏  TCHAR			 宏 	PTSTR(LPTSTR)
		 				
				
字符数组赋值				
				
	CHAR cha[] = "中国";				
					
	WCHAR chw[] = L"中国";				
					
	TCHAR cht[] = TEXT("中国");				
				
				
为字符串指针赋值：				
				
	PSTR pszChar = "china";				//多字节字符
					
	PWSTR pszWChar = L"china";				//宽字符
					
	PTSTR pszTChar = TEXT("china");				//如果项目是ASCII的 相当于"china" UNICODE 相当于L"china"
				
				
4、各种版本的MessageBox				
				
	MessageBoxA(0,"内容多字节","标题",MB_OK);				
					
	MessageBoxW(0,L"内容宽字节",L"标题",MB_OK);				
					
	MessageBox(0,TEXT("根据项目字符集决定"),TEXT("标题"),MB_OK);				
					
	Windows提供的API 凡是需要传递字符串参数的函数，都会提供两个版本和一个宏.				
