#图标

	#include "stdafx.h"
	#include "resource.h"
	
	HINSTANCE hAppInstance;
	void GetDlgWord(HWND hDlg)
	{
					//获得文本框句柄
					HWND hEditUser = GetDlgItem(hDlg,IDC_EDIT_USERNAME);
					HWND hEditPass = GetDlgItem(hDlg,IDC_EDIT_PASSWORD);
					//获得文本框内容
					TCHAR szUserBuff[0x10];
					TCHAR szPassBuff[0x10];
					GetWindowText(hEditUser,szUserBuff,0x10);
					GetWindowText(hEditPass,szPassBuff,0x10);
	}									
	BOOL CALLBACK DialogProc(									
							 HWND hwndDlg,  // handle to dialog box			
							 UINT uMsg,     // message			
							 WPARAM wParam, // first message parameter			
							 LPARAM lParam  // second message parameter			
							 )			
	{									
		HICON hIconBig;
		HICON hIconSmall;
										
		switch(uMsg)								
		{								
		case  WM_INITDIALOG :															
			//MessageBox(NULL,TEXT("WM_INITDIALOG"),TEXT("INIT"),MB_OK);
		hIconBig = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_BIG));		
		hIconSmall = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON_SMALL));	
			//设置图标
			SendMessage(hwndDlg,WM_SETICON,ICON_BIG,(DWORD)hIconBig);
			SendMessage(hwndDlg,WM_SETICON,ICON_SMALL,(DWORD)hIconSmall);
			return TRUE ;							
										
		case  WM_COMMAND :																
			switch (LOWORD (wParam))							
			{							
			case   IDC_BUTTON_OK :															
				{//	MessageBox(NULL,TEXT("IDC_BUTTON_OK"),TEXT("ok"),MB_OK);
					GetDlgWord(hwndDlg);
					return TRUE;	}												
			case   IDC_BUTTON_ERROR:														
			//	MessageBox(NULL,TEXT("IDC_BUTTON_ERROR"),TEXT("error"),MB_OK);														
				EndDialog(hwndDlg, 0);														
				return TRUE;						
			}
	
			break ;							
	    }																	
		return FALSE ;								
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
		hAppInstance = hInstance;
		DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN),0,DialogProc);	
			//6展示窗口,已省略
			//7回调函数，已在上面实现
		return 0;
	}

1、加载图标		
		
	HICON hIcon;		
			
	hIcon = LoadIcon (hAppInstance, MAKEINTRESOURCE (IDI_ICON));		
			
	hAppInstance		
			
		应用程序句柄	
			
	IDI_ICON		
			
		图标编号	
			
	MAKEINTRESOURCE		
			
		用这个宏的主要原因是有的资源是用序号定义的,而不是字符串.所以要把数字转换成字符串指针	
		
2、设置图标：		
		
	case WM_INITDIALOG :		
			
			hIcon = LoadIcon (hAppInstance, MAKEINTRESOURCE (IDI_ICON));
			//设置图标
			SendMessage(hDlg,WM_SETICON,ICON_BIG,(DWORD)hIcon);
			SendMessage(hDlg,WM_SETICON,ICON_SMALL,(DWORD)hIcon);
			
			return TRUE;
