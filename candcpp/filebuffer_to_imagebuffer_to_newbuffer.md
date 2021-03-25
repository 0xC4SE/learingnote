	
#.h
	
	#include <stdlib.h>
	#include <stdio.h>
	#include <malloc.h>
	#include <memory.h>
	#include <string.h>
	//定义数据宽度
	#define BYTE char
	#define WORD short
	#define DWORD int
	#define size_t int
	//定义参数返回
	#define IN 
	#define OUT 
	//文件路径
	#define FILE_PATH "E:\\notepad.exe"
	#define BACK_PATH "E:\\notepad2.exe"
	extern char* file_buffer;
	extern char* image_buffer;
	extern char* new_buffer;
	extern int sizeof_newbuffer;
	//创建结构体
	struct IMAGE_NT_HEADERS;
	struct IMAGE_FILE_HEADER;
	struct IMAGE_OPTIONAL_HEADER;
	struct IMAGE_SECTION_HEADER;
	
	void pefile_free();
	void peimage_free();
	void pnew_free();
	
	
	//char* IN_FILE_PATH;
	//void* FILE_BUFFER;
	//void* IMAGE_BUFFER;
	//void* NEW_BUFFER;
	//char* BACK_FILE_PATH;
	
	
	
	//函数声明																
							
	//**************************************************************************								
	DWORD readpefile(IN char* IN_FILE_PATH,OUT char* FILE_BUFFER);						
	//**************************************************************************								
							
	//**************************************************************************								
	DWORD filebuffertoimagebuffer(IN char* FILE_BUFFER,OUT char* IMAGE_BUFFER);								
	//**************************************************************************								
							
	//**************************************************************************								
	DWORD imagebuffertonewbuffer(IN char* IMAGE_BUFFER,OUT char* NEW_BUFFER);								
	//**************************************************************************								
							
	//**************************************************************************								
	DWORD buffertofile(IN char* NEW_BUFFER,IN size_t size,OUT char* BACK_FILE_PATH);								
	//**************************************************************************								
						
	//**************************************************************************								
	DWORD rvatofoa(IN void* FILE_BUFFER,IN DWORD RVA);								









#.cpp

	#include "StdAfx.h"
	#include "global.h"
	
	char IN_FILE_PATH[];
	char BACK_FILE_PATH[];
	char* pmalloc;
	char* pimage_buffer;
	char* pnew_buffer;
	char* file_buffer;
	char* image_buffer;
	char* new_buffer;
	int sizeof_newbuffer;

/******************************************************************************/
//PE结构体定义

	#define IMAGE_SIZEOF_SHORT_NAME 8;
	struct IMAGE_DOS_HEADER
	{ 
	 WORD e_magic; // DOS可执行文件标记 MZ(4Dh 5Ah) 
	 WORD e_cblp;
	 WORD e_cp; 
	 WORD e_crlc; 
	 WORD e_cparhdr; 
	 WORD e_minalloc; 
	 WORD e_maxalloc; 
	 WORD e_ss; // DOS代码的初始化堆栈SS 
	 WORD e_sp; // DOS代码的初始化堆栈指针SP 
	 WORD e_csum;
	 WORD e_ip; // DOS代码的初始化指令入口[指针IP] 
	 WORD e_cs; // DOS代码的初始堆栈入口 
	 WORD e_lfarlc;
	 WORD e_ovno;
	 WORD e_res[4]; 
	 WORD e_oemid; 
	 WORD e_oeminfo; 
	 WORD e_res2[10]; 
	 DWORD e_lfanew; // 指向PE文件头 
	};
	struct IMAGE_FILE_HEADER
	{ 
	WORD    Machine;             
	WORD    NumberOfSections;     
	DWORD   TimeDateStamp; 
	DWORD   PointerToSymbolTable; 
	DWORD   NumberOfSymbols;       
	WORD    SizeOfOptionalHeader; 
	WORD    Characteristics;
	};
	struct IMAGE_OPTIONAL_HEADER
	{ 
	WORD    Magic;                   
	BYTE    MajorLinkerVersion;         // 链接器的主版本号 -> 05
	BYTE    MinorLinkerVersion;         // 链接器的次版本号 -> 0C
	DWORD   SizeOfCode;                 // 代码节大小，一般放在“.text”节里，必须是FileAlignment的整数倍 -> 40 00 04 00
	DWORD   SizeOfInitializedData;      // 已初始化数大小，一般放在“.data”节里，必须是FileAlignment的整数倍 -> 40 00 0A 00
	DWORD   SizeOfUninitializedData;   
	DWORD   AddressOfEntryPoint;      
	DWORD   BaseOfCode;              
	DWORD   BaseOfData;                
	DWORD   ImageBase;                     
	DWORD   SectionAlignment;      
	DWORD   FileAlignment;  
	WORD    MajorOperatingSystemVersion;// 主系统的主版本号 -> 00 04
	WORD    MinorOperatingSystemVersion;// 主系统的次版本号 -> 00 00
	WORD    MajorImageVersion;          // 镜像的主版本号 -> 00 00
	WORD    MinorImageVersion;          // 镜像的次版本号 -> 00 00
	WORD    MajorSubsystemVersion;      // 子系统的主版本号 -> 00 04
	WORD    MinorSubsystemVersion;      // 子系统的次版本号 -> 00 00
	DWORD   Win32VersionValue;          // 保留，必须为0 -> 00 00 00 00
	DWORD   SizeOfImage;              
	DWORD   SizeOfHeaders; 
	DWORD   CheckSum;
	WORD    Subsystem;           
	WORD    DllCharacteristics;
	DWORD   SizeOfStackReserve;
	DWORD   SizeOfStackCommit;
	DWORD   SizeOfHeapReserve;
	DWORD   SizeOfHeapCommit;    
	DWORD   LoaderFlags;            
	DWORD   NumberOfRvaAndSizes;       
	//    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};														
	struct IMAGE_SECTION_HEADER
	{						
	    BYTE Name[8];						
	    union {						
	            DWORD   PhysicalAddress;						
	            DWORD   VirtualSize;						
	    }Misc;						
	    DWORD   VirtualAddress;						
	    DWORD   SizeOfRawData;						
	    DWORD   PointerToRawData;						
	    DWORD   PointerToRelocations;						
	    DWORD   PointerToLinenumbers;						
	    WORD    NumberOfRelocations;						
	    WORD    NumberOfLinenumbers;						
	    DWORD   Characteristics;						
	};						
/***************************************************************************/

//--------------------------------------------------------------------------//
//file_path to file_buffer


	DWORD readpefile(IN char* IN_FILE_PATH, OUT char* FILE_BUFFER)
	{
		//打开文件，结尾记得关闭
			FILE* pbuffer = fopen(IN_FILE_PATH,"rb"); 							
			if(pbuffer == NULL)							
			{							
				printf("This file isn't open!\n");						
			}
		//将pbuffer指针指向文件结尾，来获得文件的大小			
			fseek(pbuffer,0,SEEK_END);							
			int len = ftell(pbuffer);							
			printf("%d \n",len);	
		//获得文件大小后就应该将pbuffer指针指向文件开头			
			fseek(pbuffer,0,0);	
		//分配堆的大小，按照上面获得的大小来申请内存
			pmalloc = (char*)malloc(sizeof(char)*len);				
			if(pmalloc == NULL)							
			{							
				return 0;						
			}
		//将申请的内存清零
			memset(pmalloc,0,sizeof(char)*len);
		//堆已经分配好							
		//将pbuffer的指针指向的内存读取到pmalloc中(按照获得的大小来复制)
			fread(pmalloc,sizeof(char)*len,1,pbuffer);
		//把文件流复制到分配的堆中			
			printf("File_buffer的地址是：%x \n",pmalloc);				
		//获得file_buffer的地址
			file_buffer = (char*)pmalloc;		
		//关闭文件流和释放堆	
		printf("FILE_BUFFER: %x\n",file_buffer);

			fclose(pbuffer);							
			return 1;							
	
		}

//------------------------------------------------------------//
//filebuffer to imagebuffer函数

	DWORD filebuffertoimagebuffer(IN char* FILE_BUFFER,OUT char* IMAGE_BUFFER)
	{
	//	char* pimage_buffer;
		printf("FILE_BUFFER: %x\n",FILE_BUFFER);
		IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)FILE_BUFFER;
		IMAGE_OPTIONAL_HEADER* optional_header = (IMAGE_OPTIONAL_HEADER*)((int)dos_header+(int)dos_header->e_lfanew+0x18);
	//已经申请了IMAGE_BUFFER的大小
		pimage_buffer= (char*)malloc(sizeof(char)*optional_header->SizeOfImage);
	
		if(pimage_buffer == NULL)
		{
			printf("IMAGE_BUFFER申请失败! \n");
			return 0;
		}
	//将申请的IMAGE_BUFFER内存清零
		int i = optional_header->SizeOfImage;
		printf("%x ",i);
		memset(pimage_buffer,0,sizeof(char)*i);
		
		printf("pimage_buffer:%x \n",pimage_buffer);
		memcpy(pimage_buffer,file_buffer,optional_header->SizeOfHeaders);
	
		IMAGE_FILE_HEADER* file_header = (IMAGE_FILE_HEADER*)((int)FILE_BUFFER+(int)dos_header->e_lfanew+0x4);
		
		IMAGE_SECTION_HEADER* section_header = (IMAGE_SECTION_HEADER*)((int)FILE_BUFFER+(int)dos_header->e_lfanew+(int)file_header->SizeOfOptionalHeader+0x18);
	
		for(int j = 0;j<file_header->NumberOfSections;j++)
		{
		int x = (int)section_header->VirtualAddress+(int)pimage_buffer;
		int y = (int)section_header->PointerToRawData+(int)FILE_BUFFER;
		
		memcpy((char*)x,(char*)y,section_header->SizeOfRawData);	
		section_header = (IMAGE_SECTION_HEADER*)((int)section_header + 0x28);
		}
		
		image_buffer = pimage_buffer;
		return 1;
	}

//-----------------------------------------------------------------//
//image_buffer to new_buffer

	DWORD imagebuffertonewbuffer(IN char* IMAGE_BUFFER,OUT char* NEW_BUFFER)
	{
		IMAGE_DOS_HEADER* dos_header = NULL;
		IMAGE_OPTIONAL_HEADER* optional_header = NULL;
		IMAGE_FILE_HEADER* file_header = NULL;
		IMAGE_SECTION_HEADER* section_header = NULL;
		int sizeofnewbuffer = 0;
	//	char* pnew_buffer;
		printf("IMAGE_BUFFER: %x\n",IMAGE_BUFFER);
		dos_header = (IMAGE_DOS_HEADER*)IMAGE_BUFFER;
		optional_header = (IMAGE_OPTIONAL_HEADER*)((int)IMAGE_BUFFER+(int)dos_header->e_lfanew+0x18);
		file_header = (IMAGE_FILE_HEADER*)((int)IMAGE_BUFFER+(int)dos_header->e_lfanew+0x4);
		section_header = (IMAGE_SECTION_HEADER*)((int)IMAGE_BUFFER+(int)dos_header->e_lfanew+(int)file_header->SizeOfOptionalHeader+0x18);
		sizeofnewbuffer = (int)optional_header->SizeOfHeaders;
		for(int j = 0;j<file_header->NumberOfSections;j++)
		{
			
			sizeofnewbuffer = sizeofnewbuffer + (int)section_header->SizeOfRawData;
			section_header = (IMAGE_SECTION_HEADER*)((int)section_header + 0x28);
		}
		pnew_buffer = (char*)malloc(sizeof(char)*sizeofnewbuffer);
		if(pnew_buffer == NULL)
		{
			printf("内存读写错误!\n");
		}
		memset(pnew_buffer,0,sizeofnewbuffer);
		printf("new_buffer地址:%x\n",pnew_buffer);
		
		memcpy(pnew_buffer,IMAGE_BUFFER,optional_header->SizeOfHeaders);
	
		section_header = (IMAGE_SECTION_HEADER*)((int)IMAGE_BUFFER+(int)dos_header->e_lfanew+(int)file_header->SizeOfOptionalHeader+0x18);
		for(int k = 0;k<file_header->NumberOfSections;k++)
		{
		int x = (int)section_header->VirtualAddress+(int)IMAGE_BUFFER;
		int y = (int)section_header->PointerToRawData+(int)pnew_buffer;
		
		memcpy((char*)y,(char*)x,section_header->SizeOfRawData);	
		section_header = (IMAGE_SECTION_HEADER*)((int)section_header + 0x28);
		}
		
		sizeof_newbuffer = sizeofnewbuffer;
		new_buffer = pnew_buffer;
		printf("last_back_newbuffer:%x",new_buffer);
		return 1;
	}
	


//---------------------------------------------------------------------------------//
//image_buffer to file_path

	DWORD buffertofile(IN char* NEW_BUFFER,int size,OUT char* BACK_FILE_PATH)
	{	
		printf("回到文件NEW_BUFFER：%x\n",NEW_BUFFER);
		FILE* fp2 = fopen(BACK_FILE_PATH,"wb+");						
		if(fp2 == NULL)						
		{						
				printf("文件未创建成功");					
		}
		printf("文件返回new_buffer:%x\n",NEW_BUFFER);
		size = sizeof_newbuffer;
		fwrite(NEW_BUFFER,sizeof(char)*size,1,fp2);														
		fclose(fp2);		
		return 1;
	}

	void pnew_free()
	{
		free(pnew_buffer);
		pnew_buffer = NULL;
	}
	
	void peimage_free()
	{
		free(pimage_buffer);
		pimage_buffer = NULL;
	}
	
	void pefile_free()
	{
		free(pmalloc);							
		pmalloc = NULL;							
	}



//----------------------------------------------------------------------------------//



	DWORD rvatofoa(IN char* rva_IMAGE_BUFFER,IN DWORD RVA)
	{
		IMAGE_DOS_HEADER* dos_header = NULL;
		IMAGE_OPTIONAL_HEADER* optional_header = NULL;
		IMAGE_FILE_HEADER* file_header = NULL;
		IMAGE_SECTION_HEADER* section_header = NULL;
	//	int sizeofnewbuffer = 0;
	//	char* pnew_buffer;
		printf("IMAGE_BUFFER: %x\n",rva_IMAGE_BUFFER);
		dos_header = (IMAGE_DOS_HEADER*)image_buffer;
		optional_header = (IMAGE_OPTIONAL_HEADER*)((int)image_buffer+(int)dos_header->e_lfanew+0x18);
		file_header = (IMAGE_FILE_HEADER*)((int)image_buffer+(int)dos_header->e_lfanew+0x4);
		section_header = (IMAGE_SECTION_HEADER*)((int)image_buffer+(int)dos_header->e_lfanew+(int)file_header->SizeOfOptionalHeader+0x18);
		int addr = (int)image_buffer + (int)optional_header->SizeOfImage;
	
		if((int)rva_IMAGE_BUFFER <(int)image_buffer || (int)rva_IMAGE_BUFFER>addr)
		{
			printf("该地址不在image_buffer中!\n");
			return 0;
		}
		int rva = (int)rva_IMAGE_BUFFER - (int)image_buffer;
		if(rva > 0 && rva < (int)optional_header->SizeOfHeaders)
		{
			printf("该rva对应的foa为:%x \n",rva);
			RVA = rva;
		}
		
		for(int j = 0;j<file_header->NumberOfSections;j++)
		{
			if(rva > (int)section_header->VirtualAddress && rva < ((int)section_header->VirtualAddress+(int)section_header->Misc.VirtualSize))
			{
			
			int k = (int)section_header->PointerToRawData + (int)rva - (int)section_header->VirtualAddress;;
			printf("该rva对应第%d个节的foa为:%x \n",(j+1),k);
			RVA = rva;
			}
		section_header = (IMAGE_SECTION_HEADER*)((int)section_header + 0x28);
		}
		return 1;
	}
