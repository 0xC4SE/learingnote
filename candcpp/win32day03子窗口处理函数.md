#子窗口的消息处理函数

按钮的本质：									
									
在项目中添加如下代码：									
									
	void CreateButton(HWND hwnd)									
	{									
		HWND hwndPushButton;								
		HWND hwndCheckBox;								
		HWND hwndRadio;								
										
		hwndPushButton = CreateWindow ( 								
			TEXT("button"), 							
			TEXT("普通按钮"),							
			//WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,							
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_DEFPUSHBUTTON,							
			10, 10,							
			80, 20,							
			hwnd, 							
			(HMENU)1001,		//子窗口ID					
			hAppInstance, 							
			NULL);							
										
		hwndCheckBox = CreateWindow ( 								
			TEXT("button"), 							
			TEXT("复选框"),							
			//WS_CHILD | WS_VISIBLE | BS_CHECKBOX | BS_AUTOCHECKBOX,							
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX |BS_AUTOCHECKBOX ,							
			10, 40,							
			80, 20,							
			hwnd, 							
			(HMENU)1002,		//子窗口ID					
			hAppInstance, 							
			NULL);							
										
		hwndRadio = CreateWindow ( 								
			TEXT("button"), 							
			TEXT("单选按钮"),							
			//WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_AUTORADIOBUTTON,							
			WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON  ,							
			10, 70,							
			80, 20,							
			hwnd, 							
			(HMENU)1003,		//子窗口ID					
			hAppInstance, 							
			NULL);							
	} 									
									
									
总结：									
									
按钮就是窗口.									


1、在父窗口的消息处理函数中添加单击左键的事件，观察结果.								
								
								
2、按钮事件的处理：								
								
按钮的WNDCLASS不是我们定义的，是系统预定义好的。如果我们想知道，系统预定义的WNDCLASS都包含什么样的信息								
								
怎么做？								
								
	TCHAR szBuffer[0x20];								
	GetClassName(hwndPushButton,szBuffer,0x20);								
									
	WNDCLASS wc;								
	GetClassInfo(hAppInstance,szBuffer,&wc);								
	OutputDebugStringF("-->%s\n",wc.lpszClassName);								
	OutputDebugStringF("-->%x\n",wc.lpfnWndProc);								
								
总结：								
								
1、按钮是一种特殊的窗体，并不需要提供单独的窗口回调函数.								
								
2、当按钮有事件产生时，会给父窗口消息处理程序发送一个WM_COMMAND消息								
								
								
								
	按钮------------------->系统提供WinProc------------------->父窗口的WinProc	
	    		单击按钮				 	   转换WM_COMMAND			


代码：								
								
	case WM_COMMAND:								
	{								
		switch(LOWORD(wParam))							
		{							
			case 1001:						
				MessageBox(hwnd,"Hello Button 1","Demo",MB_OK);					
				return 0;					
			case 1002:						
				MessageBox(hwnd,"Hello Button 2","Demo",MB_OK);					
				return 0;					
			case 1003:						
				MessageBox(hwnd,"Hello Button 3","Demo",MB_OK);					
				return 0;					
		}							
		return DefWindowProc(hwnd,uMsg,wParam,lParam);							
	}								
								
###button的winproc//callback函数
	
					
	77D3B036    8BFF            MOV EDI,EDI						
	77D3B038    55              PUSH EBP						
	77D3B039    8BEC            MOV EBP,ESP						
	77D3B03B    8B4D 08         MOV ECX,DWORD PTR SS:[EBP+8]						
	77D3B03E    56              PUSH ESI						
	77D3B03F    E8 9CD4FDFF     CALL USER32.77D184E0						
	77D3B044    8BF0            MOV ESI,EAX						
	77D3B046    85F6            TEST ESI,ESI						
	77D3B048    74 38           JE SHORT USER32.77D3B082						
	77D3B04A    8B55 0C         MOV EDX,DWORD PTR SS:[EBP+C]						
	77D3B04D    3B15 E810D777   CMP EDX,DWORD PTR DS:[77D710E8]						
	77D3B053    77 45           JA SHORT USER32.77D3B09A						
	77D3B055    33C0            XOR EAX,EAX						
	77D3B057    8BCA            MOV ECX,EDX						
	77D3B059    83E1 07         AND ECX,7						
	77D3B05C    40              INC EAX						
	77D3B05D    D3E0            SHL EAX,CL						
	77D3B05F    57              PUSH EDI						
	77D3B060    8B3D EC10D777   MOV EDI,DWORD PTR DS:[77D710EC]						
	77D3B066    8BCA            MOV ECX,EDX						
	77D3B068    C1E9 03         SHR ECX,3						
	77D3B06B    8A0C39          MOV CL,BYTE PTR DS:[ECX+EDI]						
	77D3B06E    84C1            TEST CL,AL						
	77D3B070    5F              POP EDI						
	77D3B071    74 27           JE SHORT USER32.77D3B09A						
	77D3B073    6A 01           PUSH 1						
	77D3B075    FF75 14         PUSH DWORD PTR SS:[EBP+14]						
	77D3B078    FF75 10         PUSH DWORD PTR SS:[EBP+10]						
	77D3B07B    52              PUSH EDX						
	77D3B07C    56              PUSH ESI						
	77D3B07D    E8 B6A1FEFF     CALL USER32.77D25238						
	77D3B082    5E              POP ESI						
	77D3B083    5D              POP EBP						
	77D3B084    C2 1000         RETN 10						
	77D3B087    83E8 07         SUB EAX,7						
	77D3B08A    0F85 13040000   JNZ USER32.77D3B4A3						
	77D3B090    B8 00010000     MOV EAX,100						
	77D3B095  ^ E9 68A2FEFF     JMP USER32.77D25302						
	77D3B09A    6A 01           PUSH 1						
	77D3B09C    FF75 14         PUSH DWORD PTR SS:[EBP+14]						
	77D3B09F    FF75 10         PUSH DWORD PTR SS:[EBP+10]						
	77D3B0A2    52              PUSH EDX						
	77D3B0A3    56              PUSH ESI						
	77D3B0A4    E8 31ECFEFF     CALL USER32.77D29CDA						
	77D3B0A9  ^ EB D7           JMP SHORT USER32.77D3B082						
	77D3B0AB    E8 03E1FEFF     CALL USER32.77D291B3						
	77D3B0B0  ^ E9 0E71FFFF     JMP USER32.77D321C3						
	77D3B0B5    33C0            XOR EAX,EAX						
	77D3B0B7  ^ E9 0B71FFFF     JMP USER32.77D321C7						
	77D3B0BC    33C0            XOR EAX,EAX						
	77D3B0BE  ^ E9 BA4FFFFF     JMP USER32.77D3007D						
	77D3B0C3    6A 01           PUSH 1						
	77D3B0C5    6A 01           PUSH 1						
	77D3B0C7    68 F4000000     PUSH 0F4						
	77D3B0CC    FF75 10         PUSH DWORD PTR SS:[EBP+10]						
	77D3B0CF  ^ E9 D39CFEFF     JMP USER32.77D24DA7						
	77D3B0D4    897D FC         MOV DWORD PTR SS:[EBP-4],EDI						
	77D3B0D7  ^ E9 D89AFEFF     JMP USER32.77D24BB4						
	77D3B0DC    33C0            XOR EAX,EAX						
	77D3B0DE    40              INC EAX						
	77D3B0DF  ^ E9 839CFEFF     JMP USER32.77D24D67						
	77D3B0E4    83E9 69         SUB ECX,69						
	77D3B0E7    0F84 F3040000   JE USER32.77D3B5E0						
	77D3B0ED    49              DEC ECX						
	77D3B0EE  ^ 0F85 5DA2FEFF   JNZ USER32.77D25351						
	77D3B0F4    3955 B0         CMP DWORD PTR SS:[EBP-50],EDX						
	77D3B0F7  ^ 0F82 43A2FEFF   JB USER32.77D25340						
	77D3B0FD    395D B0         CMP DWORD PTR SS:[EBP-50],EBX						
	77D3B100    0F87 A2110000   JA USER32.77D3C2A8						
	77D3B106    837D B8 00      CMP DWORD PTR SS:[EBP-48],0						
	77D3B10A    74 07           JE SHORT USER32.77D3B113						
	77D3B10C    C745 B8 0100000>MOV DWORD PTR SS:[EBP-48],1						
	77D3B113    8B46 04         MOV EAX,DWORD PTR DS:[ESI+4]						
	77D3B116    8BC8            MOV ECX,EAX						
	77D3B118    23CB            AND ECX,EBX						
	77D3B11A    3B4D B8         CMP ECX,DWORD PTR SS:[EBP-48]						
	77D3B11D  ^ 0F84 1DA2FEFF   JE USER32.77D25340						
	77D3B123    83E0 FC         AND EAX,FFFFFFFC						
	77D3B126    0B45 B8         OR EAX,DWORD PTR SS:[EBP-48]						
	77D3B129    57              PUSH EDI						
	77D3B12A    8946 04         MOV DWORD PTR DS:[ESI+4],EAX						
	77D3B12D    E8 0C9EFEFF     CALL USER32.77D24F3E						
	77D3B132    85C0            TEST EAX,EAX						
	77D3B134  ^ 0F84 06A2FEFF   JE USER32.77D25340						
	77D3B13A  ^ E9 AAABFEFF     JMP USER32.77D25CE9						
	77D3B13F    90              NOP						
	77D3B140    90              NOP						
	77D3B141    90              NOP						
	77D3B142    90              NOP						
	77D3B143    90              NOP						
	77D3B144 >  8BFF            MOV EDI,EDI						
	77D3B146    55              PUSH EBP						
	77D3B147    8BEC            MOV EBP,ESP						
	77D3B149    53              PUSH EBX						
	77D3B14A    56              PUSH ESI						
	77D3B14B    8B75 08         MOV ESI,DWORD PTR SS:[EBP+8]						
	77D3B14E    6A 00           PUSH 0						
	77D3B150    FF75 0C         PUSH DWORD PTR SS:[EBP+C]						
	77D3B153    83CB FF         OR EBX,FFFFFFFF						
	77D3B156    6A 05           PUSH 5						
	77D3B158    56              PUSH ESI						
	77D3B159    FF15 C414D777   CALL NEAR DWORD PTR DS:[77D714C4]                ; kernel32.FindResourceExA						
	77D3B15F    85C0            TEST EAX,EAX						
	77D3B161    74 40           JE SHORT USER32.77D3B1A3						
	77D3B163    57              PUSH EDI						
	77D3B164    50              PUSH EAX						
	77D3B165    56              PUSH ESI						
	77D3B166    FF15 E412D777   CALL NEAR DWORD PTR DS:[77D712E4]                ; kernel32.LoadResource						
	77D3B16C    8BF8            MOV EDI,EAX						
	77D3B16E    85FF            TEST EDI,EDI						
	77D3B170    74 30           JE SHORT USER32.77D3B1A2						
	77D3B172    56              PUSH ESI						
	77D3B173    57              PUSH EDI						
	77D3B174    FF15 EC12D777   CALL NEAR DWORD PTR DS:[77D712EC]                ; USER32.77D18D06						
	77D3B17A    85C0            TEST EAX,EAX						
	77D3B17C    74 1C           JE SHORT USER32.77D3B19A						
	77D3B17E    6A 02           PUSH 2						
	77D3B180    FF75 18         PUSH DWORD PTR SS:[EBP+18]						
	77D3B183    FF75 14         PUSH DWORD PTR SS:[EBP+14]						
	77D3B186    FF75 10         PUSH DWORD PTR SS:[EBP+10]						
	77D3B189    50              PUSH EAX						
	77D3B18A    56              PUSH ESI						
	77D3B18B    E8 4098FEFF     CALL USER32.DialogBoxIndirectParamAorW						
	77D3B190    56              PUSH ESI						
	77D3B191    57              PUSH EDI						
	77D3B192    8BD8            MOV EBX,EAX						
	77D3B194    FF15 E812D777   CALL NEAR DWORD PTR DS:[77D712E8]                ; USER32.77D18CFB						
	77D3B19A    56              PUSH ESI						
	77D3B19B    57              PUSH EDI						
	77D3B19C    FF15 5C12D777   CALL NEAR DWORD PTR DS:[77D7125C]                ; USER32.77D2AF4C						
	77D3B1A2    5F              POP EDI						
	77D3B1A3    5E              POP ESI						
	77D3B1A4    8BC3            MOV EAX,EBX						
	77D3B1A6    5B              POP EBX						
	77D3B1A7    5D              POP EBP						
	77D3B1A8    C2 1400         RETN 14						



1、回调函数的结构					
					
	LRESULT CALLBACK WindowProc(  					
		IN  HWND hwnd,  				
		IN  UINT uMsg,  				
		IN  WPARAM wParam,  				
		IN  LPARAM lParam  				
		);  				
					
					
					
2、回调函数的堆栈					
					
		|		|	ESP + 8 == uMsg		
		|		|		
		|		|	ESP + C == wParam		
	ESP	|返回地址|				
		|hwnd	|			
		|uMsg	|			
		|wParam|			
		|lParam|				
