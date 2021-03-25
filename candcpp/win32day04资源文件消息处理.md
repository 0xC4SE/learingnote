#资源文件消息处理

###窗口资源文件的创建

	#include "stdafx.h"
	#include "resource.h"
	
										
	BOOL CALLBACK DialogProc(									
							 HWND hwndDlg,  // handle to dialog box			
							 UINT uMsg,     // message			
							 WPARAM wParam, // first message parameter			
							 LPARAM lParam  // second message parameter			
							 )			
	{									
										
		switch(uMsg)								
		{								
		case  WM_INITDIALOG :															
		//	MessageBox(NULL,TEXT("WM_INITDIALOG"),TEXT("INIT"),MB_OK);														
			return TRUE ;							
										
		case  WM_COMMAND :		//这里不再是api调用的WM_MOVE等等消息，而是资源文件统一的WM_COMMAND														
			switch (LOWORD (wParam))							
			{							
			case   IDC_BUTTON_OK:															
			//	MessageBox(NULL,TEXT("IDC_BUTTON_OK"),TEXT("OK"),MB_OK);													
				return TRUE;													
			case   IDC_BUTTON_ERROR:														
			//	MessageBox(NULL,TEXT("IDC_BUTTON_OUT"),TEXT("OUT"),MB_OK);														
				EndDialog(hwndDlg, 0);														
				return TRUE;						
			}							
			break ;							
	    }																	
		return FALSE ;	
		//说明：					
		//1、处理过的消息，返回TRUE					
		//2、不处理的消息，返回FALSE			
							
	}									
										
	
	int APIENTRY WinMain(HINSTANCE hInstance,
	                     HINSTANCE hPrevInstance,
	                     LPSTR     lpCmdLine,
	                     int       nCmdShow)
	{		//1声明窗口类wndclass,已省略
			//2给窗口类赋值,已省略
			//3注册窗口类,已省略
			//4注册完就可以create窗口了，就是DialogBox函数,已省略
		//5创建资源窗口
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),0,DialogProc);					
			//6展示窗口,已省略
			//7回调函数，已在上面实现
		return 0;
	}


1、创建一个空的Win32应用程序									
									
2、在VC6中新增资源									
									
File ->  New  -> Resource Script									
									
创建成功后会新增2个文件：xxx.rc 和 resource.h 									
									
									
3、打开新创建的xxx.rc文件,在资源窗口新增一个对话框，并在对话框上新增2个按钮.									
									
									
4、打开resource.h文件，观察变化：									
									
#define IDD_DIALOG_MAIN                 101									
#define IDC_BUTTON_OK                   1000									
#define IDC_BUTTON_OUT                  1001									
									
									
5、通过DialogBox创建对话框									
									
									
INT_PTR DialogBox(									
  HINSTANCE hInstance,  // handle to module									
  LPCTSTR lpTemplate,   // dialog box template									
  HWND hWndParent,      // handle to owner window									
  DLGPROC lpDialogFunc  // dialog box procedure									
);									
									
参考MSDN									
									
									
6、定义Dialog消息处理函数									
									
									
	BOOL CALLBACK DialogProc(									
							 HWND hwndDlg,  // handle to dialog box			
							 UINT uMsg,     // message			
							 WPARAM wParam, // first message parameter			
							 LPARAM lParam  // second message parameter			
							 )			
	{									
										
		switch(uMsg)								
		{								
		case  WM_INITDIALOG :								
										
			MessageBox(NULL,TEXT("WM_INITDIALOG"),TEXT("INIT"),MB_OK);							
										
			return TRUE ;							
										
		case  WM_COMMAND :								
										
			switch (LOWORD (wParam))							
			{							
			case   IDC_BUTTON_OK :							
										
				MessageBox(NULL,TEXT("IDC_BUTTON_OK"),TEXT("OK"),MB_OK);						
										
				return TRUE;						
										
			case   IDC_BUTTON_OUT:							
										
				MessageBox(NULL,TEXT("IDC_BUTTON_OUT"),TEXT("OUT"),MB_OK);						
										
				EndDialog(hwndDlg, 0);						
										
				return TRUE;						
			}							
			break ;							
	    }									
										
		return FALSE ;								
	}									
									
说明：									
									
1、处理过的消息，返回TRUE									
									
2、不处理的消息，返回FALSE									

							
1、获取文本框句柄：							
							
							
	HWND hEditUser = GetDlgItem(hDlg,IDC_EDIT_USER);							
							
	hDlg							
		对话框句柄						
								
	IDC_EDIT_USER							
								
		编号						
							
							
2、获取文本框内容							
							
	TCHAR szUserBuff[0x50];							
								
	GetWindowText(hEditUser,szUserBuff,0x50);							
								
	hEditUser							
								
		文本框句柄						
								
	szUserBuff							
								
		输出缓冲区						
							
