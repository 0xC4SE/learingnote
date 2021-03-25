#消息

代码

	#include "stdafx.h"
	#include <Windows.h>
	
	
	//LRESULT就是long类型 CALLBACK其实就是_stdcall
	LRESULT CALLBACK WindowProc(  										
								IN  HWND hwnd,  		//窗口句柄	
								IN  UINT uMsg,  		//消息的类型ID	
								IN  WPARAM wParam,  	//每种消息的附加说明		
								IN  LPARAM lParam  		//每种消息的附加说明	
								)  			
	{  										
		//在此函数处理我们想要处理的消息
		switch(uMsg)
		{
			//消息的类型 创建
			case WM_CREATE: 							
			{						
			//	dbgprintf("WM_CREATE %d %d\n",wParam,lParam);//第一个wparam没有用，第二个lparam指向一个struct				
			//	CREATESTRUCT* createst = (CREATESTRUCT*)lParam;					
			//	dbgprintf("CREATESTRUCT %s\n",createst->lpszClass);	//window的名称				
				dbgprintf(" WM_CREATE.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);
				return 0;					
			}						
			case WM_MOVE:							
			{						
			//	dbgprintf("WM_MOVE %d %d\n",wParam,lParam);//第一个wparam没有用，第二个lparam是坐标					
			//	POINTS points = MAKEPOINTS(lParam);//MAKEPOINTS是一个函数将lparam分解x，y坐标的POINTSstruct				
			//	dbgprintf("X Y %d %d\n",points.x,points.y);					
				dbgprintf(" WM_MOVE.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);	
			}
			case WM_SIZE:							
			{						
			//	dbgprintf("WM_SIZE %d %d\n",wParam,lParam);					
			//	int newWidth  = (int)(short) LOWORD(lParam);    					
			//	int newHeight  = (int)(short) HIWORD(lParam);   					
			//	dbgprintf("WM_SIZE %d %d\n",newWidth,newHeight);					
				dbgprintf(" WM_SIZE.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);					
				return 0;					
			}						
			case WM_DESTROY:							
			{						
			//	dbgprintf("WM_DESTROY %d %d\n",wParam,lParam);					
			//	PostQuitMessage(0);					
				dbgprintf(" WM_DESTROY.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);					
				return 0;					
			}						
			case WM_KEYUP:								
			{							
			//	dbgprintf("WM_KEYUP %d %d\n",wParam,lParam);						
				dbgprintf(" WM_KEYUP.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);						
				return 0;						
			}							
			case WM_KEYDOWN:								
			{							
			//	dbgprintf("WM_KEYDOWN %d %d\n",wParam,lParam);						
				dbgprintf(" WM_KEYDOWN.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);						
				return 0;						
			}							
			//鼠标消息							
			case WM_LBUTTONDOWN:								
			{							
			//	dbgprintf("WM_LBUTTONDOWN %d %d\n",wParam,lParam);						
			//	POINTS points = MAKEPOINTS(lParam);						
			//	dbgprintf("WM_LBUTTONDOWN %d %d\n",points.x,points.y);						
				dbgprintf(" WM_LBUTTONDOWN.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);						
				return 0;						
			}							
			case WM_LBUTTONUP:								
			{							
			//	dbgprintf("WM_LBUTTONDOWN %d %d\n",wParam,lParam);						
			//	POINTS points = MAKEPOINTS(lParam);						
			//	dbgprintf("WM_LBUTTONDOWN %d %d\n",points.x,points.y);						
				dbgprintf(" WM_LBUTTONUP.uMsg:%x\n ",uMsg);
				dbgprintf(" hwnd:%x\n ",hwnd);						
				return 0;						
			}
		}
	
		//不是我们关注的消息处理默认的返回函数
		return DefWindowProc(hwnd,uMsg,wParam,lParam);									
	}   										
	
	//入口main函数
	int APIENTRY WinMain(HINSTANCE hInstance,			//app的imagebase
	                     HINSTANCE hPrevInstance,
	                     LPSTR     lpCmdLine,
	                     int       nCmdShow)
	{
	 	// TODO: Place code here.
	
		WNDCLASS wndclass = {0};	//默认要把初下面以外的所有不关注成员赋值为0	
		TCHAR className[] = TEXT("Myfirstwindow");
		//设置回调函数
		wndclass.lpfnWndProc = WindowProc;
		//设置app的imagebase
		wndclass.hInstance = hInstance;
		//设置窗口的背景色
		wndclass.hbrBackground = (HBRUSH)COLOR_MENU;
		//设置窗口的名称编号
		wndclass.lpszClassName = className;
	
		//注册窗口类
		RegisterClass(&wndclass);  		
	
		//注册完就可以create窗口了
		HWND hwnd = CreateWindow(  							
		className,							//类名		
		TEXT("我的第一个窗口"),				//窗口标题		
		WS_OVERLAPPEDWINDOW,				//窗口外观样式 		
		10,									//相对于父窗口的X坐标		
		10,									//相对于父窗口的Y坐标		
		600,								//窗口的宽度  		
		300,								//窗口的高度  		
		NULL,								//父窗口句柄，为NULL  		
		NULL,								//菜单句柄，为NULL  		
		hInstance,							//当前应用程序的句柄  		
		NULL);								//附加数据一般为NULL		
								
		if(hwnd == NULL)					//是否创建成功  		
		return 0;  
		
		//创建完窗口以后要展示窗口
		ShowWindow(hwnd, SW_SHOW);			
	
		
		//显示完窗口以后就可以进行消息的循环取出了
		MSG msg;  		//GetMessage函数返回一个MSG结构体类型消息，则需要定义一个
		while(GetMessage(&msg, NULL, 0, 0))  			
		{  			
			TranslateMessage(&msg);  		
			DispatchMessage(&msg);  		
		}  			
	
		//回调函数 系统自己调用
		//注意：1窗口回调函数处理过的消息，必须传回0
		//2窗口回调不处理的消息，右DefWindowProc函数来处理
	
		return 0;
	}


步骤1：创建Windows应用程序  选择空项目																	
																	
										
					
步骤2：在新建项窗口中选C++代码文件 创建一个新的cpp文件										

	

步骤3：在新的cpp文件中添加:#include <Windows.h>											
							
	#define CALLBACK    __stdcall		
	#define WINAPI      __stdcall						
												
	#define WINAPIV     __cdecl						
												
	#define APIENTRY    WINAPI				默认的winmain类型		
	#define APIPRIVATE  __stdcall
	#define PASCAL      __stdcall													
						
并添加入口函数：																	
																	
	int CALLBACK WinMain(  						CALLBACK 是一个宏 											
		_In_  HINSTANCE hInstance,  			#define CALLBACK    __stdcall											
		_In_  HINSTANCE hPrevInstance,  																
		_In_  LPSTR lpCmdLine,  																
		_In_  int nCmdShow  																
		)  					所有的Win32     API函数都遵循该约定											
	{  																	
																		
		return 0;  																
	}  																	
																	
步骤4：设计窗口类	  下面是代码：


													
																											typedef struct _WNDCLASS { 	
																	    											UINT       style; 			
																													WNDPROC    lpfnWndProc; 		
																   	 												int        cbClsExtra; 			
	//窗口的类名																										int        cbWndExtra; 			
	TCHAR className[] = "My First Window"; 																			HINSTANCE  hInstance; 			
							    																					HICON      hIcon; 			
	// 创建窗口类的对象 																								HCURSOR    hCursor; 
	WNDCLASS wndclass = {0};						//一定要先将所有值赋值								    		HBRUSH     hbrBackground; 		
	wndclass.hbrBackground = (HBRUSH)COLOR_MENU;						//窗口的背景色								LPCTSTR    lpszMenuName; 		
	wndclass.lpfnWndProc = WindowProc;						//窗口过程函数								    		LPCTSTR    lpszClassName; 		
	wndclass.lpszClassName = className;						//窗口类的名字											} WNDCLASS, *PWNDCLASS; 		
	wndclass.hInstance = hInstance;						//定义窗口类的应用程序的实例句柄											
																		
																	
步骤5：注册窗口类																	
																	
	RegisterClass(&wndclass);  																	
																	
																	
步骤6：创建窗口																	
																	
	// 创建窗口  																
	HWND hwnd = CreateWindow(  																	
	className,							//类名												
	TEXT("我的第一个窗口"),				//窗口标题												
	WS_OVERLAPPEDWINDOW,				//窗口外观样式 												
	10,									//相对于父窗口的X坐标												
	10,									//相对于父窗口的Y坐标												
	600,								//窗口的宽度  												
	300,								//窗口的高度  												
	NULL,								//父窗口句柄，为NULL  												
	NULL,								//菜单句柄，为NULL  												
	hInstance,							//当前应用程序的句柄  												
	NULL);								//附加数据一般为NULL												
	
	if(hwnd == NULL)					//是否创建成功  						
	return 0;  																

步骤7：显示窗口	
																
														typedef struct tagMSG {								
										 				 HWND   hwnd; 		//相当于窗口的编号，句柄。						
									 					 UINT   message; 		//什么样的动作，消息的类型。指定消息标识符。应用程序只能使用低位字；高位字由系统保留。						
	// 显示窗口											WPARAM wParam; 		//指定有关消息的其他信息。确切含义取决于消息成员的值。						
	ShowWindow(hwnd, SW_SHOW);  						LPARAM lParam; 		//指定有关消息的其他信息。确切含义取决于消息成员的值。						
										  				DWORD  time; 		//时间						
								 		 				POINT  pt; 		//坐标						
														} MSG, *PMSG; 
步骤8：消息循环	
								
	MSG msg;  																	
	while(GetMessage(&msg, NULL, 0, 0))  									BOOL GetMessage( LPMSG lpMsg, // message informationlpMsg此为out类型参数	
	{  																							HWND hWnd, // handle to window	hWnd经常为NULL		
		TranslateMessage(&msg);  				//翻译消息										UINT wMsgFilterMin, // first message
																								UINT wMsgFilterMax // last message);
		DispatchMessage(&msg);  				//分发派送给系统消息								如果wMsgFilterMin和wMsgFilterMax都为零，	
	}  																							则GetMessage返回所有可用消息（即，不执行范围过滤）。	
																	
																	
步骤9：回调函数																	
																	
/*																	
窗口消息处理程序 窗口回调函数：																	
																	
1、窗口回调函数处理过的消息，必须传回0.																	
																	
2、窗口回调不处理的消息，由DefWindowProc来处理.	
																
	*/												typedef UINT WPARAM;					
													typedef LONG LPARAM;					
	LRESULT CALLBACK WindowProc(  												typedef LONG LRESULT;					
								IN  HWND hwnd,  										
								IN  UINT uMsg,  										
								IN  WPARAM wParam,  										
								IN  LPARAM lParam  										
								)  										
	{  																					Parameters					
	switch(uMsg)																		hwnd					
	{																					[in] Handle to the window.					
		//窗口消息																		uMsg					
	case WM_CREATE: 																	[in] Specifies the message.					
		{																				wParam					
			DbgPrintf("WM_CREATE %d %d\n",wParam,lParam);								[in] Specifies additional message information.				
			CREATESTRUCT* createst = (CREATESTRUCT*)lParam;								lParam					
			DbgPrintf("CREATESTRUCT %s\n",createst->lpszClass);							[in] Specifies additional message information. 
																	
			return 0;																	Return Values					
		}																				The return value is the result of the message processing and depends on the message sent.					
	case WM_MOVE:																
		{										 								WM_CREATE.uMsg:1					
			DbgPrintf("WM_MOVE %d %d\n",wParam,lParam);									 					
			POINTS points = MAKEPOINTS(lParam);									 hwnd:12022a					
			DbgPrintf("X Y %d %d\n",points.x,points.y);									 					
																				 WM_MOVE.uMsg:3					
			return 0;									 					
		}										 								hwnd:12022a					
	case WM_SIZE:											 					
		{																		 WM_SIZE.uMsg:3					
			DbgPrintf("WM_SIZE %d %d\n",wParam,lParam);									 					
			int newWidth  = (int)(short) LOWORD(lParam);    					hwnd:12022a					
			int newHeight  = (int)(short) HIWORD(lParam);   														
			DbgPrintf("WM_SIZE %d %d\n",newWidth,newHeight);					WM_KEYDOWN.uMsg:100					
												 					
			return 0;									 						hwnd:12022a					
		}										 					
	case WM_DESTROY:											 				WM_KEYUP.uMsg:101					
		{										 					
			DbgPrintf("WM_DESTROY %d %d\n",wParam,lParam);						hwnd:12022a					
			PostQuitMessage(0);									 					
												 								WM_LBUTTONDOWN.uMsg:201					
			return 0;									 					
		}										 								hwnd:12022a					
		//键盘消息															
	case WM_KEYUP:																WM_LBUTTONUP.uMsg:202					
		{										 					
			DbgPrintf("WM_KEYUP %d %d\n",wParam,lParam);						hwnd:12022a					
																	
			return 0;														
		}															
	case WM_KEYDOWN:																
		{															
			DbgPrintf("WM_KEYDOWN %d %d\n",wParam,lParam);														
																	
			return 0;														
		}															
		//鼠标消息															
	case WM_LBUTTONDOWN:																
		{															
			DbgPrintf("WM_LBUTTONDOWN %d %d\n",wParam,lParam);														
			POINTS points = MAKEPOINTS(lParam);														
			DbgPrintf("WM_LBUTTONDOWN %d %d\n",points.x,points.y);														
																	
			return 0;														
		}															
	}																
	return DefWindowProc(hwnd,uMsg,wParam,lParam);																
}  																	
						|											
						|																								
						|											
	Parameters																
	hWnd											 					
	[in] Handle to the window procedure that received the message.																
	Msg																
	[in] Specifies the message.																
	wParam																
	[in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.																
	lParam																
	[in] Specifies additional message information. The content of this parameter depends on the value of the Msg parameter.																
																	
	Return Values																
	The return value is the result of the message processing and depends on the message.																
