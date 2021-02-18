定义

	#include <stdlib.h>
	#include <stdio.h>
	#include <malloc.h>
	#include <memory.h>
	#include <string.h>
	#define BYTE char
	#define WORD short
	#define DWORD int
	#define FILE_PATH "E:\\notepad.exe"
	struct IMAGE_NT_HEADERS;
	struct IMAGE_FILE_HEADER;
	struct IMAGE_OPTIONAL_HEADER;
	struct IMAGE_SECTION_HEADER;



//申请内存

	int func2()
	{
	
		FILE* fp2 = fopen(FILE_PATH,"rb"); 
		if(fp2 == NULL)
		{
			printf("this file isnt open!\n");
		}
		//fwrite(arr,sizeof(arr),1,fp);
		fseek(fp2,0,SEEK_END);
		int len = ftell(fp2);
		printf("%d \n",len);//已经获得文件在内存中的大小

		char* pmalloc2;			// 在内存中分配堆，将文件读取到内存中
		if(pmalloc2 == NULL)
		{	
			printf("申请内存错误\n");
			return 0;
		}


---------------------
//初始化数据

		pmalloc2 = (char*)malloc(sizeof(char)*len);
		memset(pmalloc2,0,sizeof(char)*len);//堆已经分配好
		fseek(fp2,0,0);
		fread(pmalloc2,sizeof(char)*len,1,fp2);//把文件流复制到分配的堆中		
		printf("堆的地址 %x \n",pmalloc2);

--------------------
//将malloc地址赋给dos头指针


		IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)pmalloc2;
			
		if(dos_header->e_magic != 0x5A4D)
			{	
				printf("不是有效的MZ标志\n");
				free(pmalloc2);
				return 0;
			}
		int dos_header_address = (int)pmalloc2;
		if(*((DWORD*)(dos_header_address+(int)dos_header->e_lfanew)) != 0x4550)
		{
			printf(" %x ",dos_header->e_lfanew);
			printf("不是有效的PE文件\n");
			return 0;
		}
			printf("************DOS头****************\n");
			printf("MZ标记：%x \n",dos_header->e_magic);
			printf("e_cblp标记：%x \n",dos_header->e_cblp);
			printf("e_cblp标记：%x \n",dos_header->e_cp);
			printf("e_crlc标记：%x \n",dos_header->e_crlc);
			printf("e_cparhdr标记：%x \n",dos_header->e_cparhdr);
			printf("e_minalloc标记：%x \n",dos_header->e_minalloc);
			printf("e_maxalloc标记：%hx \n",dos_header->e_maxalloc);	//从short转换成int时，会自动按符号位扩充。
																	//	即4个F变成8个F，再匹配%x输出。
																	//如果你是%hx，则直接匹配，不会做类型转换。
			printf("e_ss标记：%x \n",dos_header->e_ss);
			printf("e_sp标记：%x \n",dos_header->e_sp);
			printf("e_csum标记：%x \n",dos_header->e_csum);
			printf("e_ip标记：%x \n",dos_header->e_ip);
			printf("e_cs标记：%x \n",dos_header->e_cs);
			printf("e_lfarlc标记：%x \n",dos_header->e_lfarlc);
			printf("e_ovno标记：%x \n",dos_header->e_ovno);
			printf("e_res标记：%x \n",dos_header->e_res);
			printf("e_oemid标记：%x \n",dos_header->e_oemid);	
			printf("e_oeminfo标记：%x \n",dos_header->e_oeminfo);
			printf("e_res2标记：%x \n",dos_header->e_res2);
			printf("e_lfanew标记：%x \n",dos_header->e_lfanew);


//dos头打印完毕
	
	printf("IMAGE_NT_SIGNATURE:%x \n",*((DWORD*)(dos_header_address+(int)dos_header->e_lfanew)));

//printf("************FILE_PE头****************\n");

	IMAGE_FILE_HEADER* file_header = (IMAGE_FILE_HEADER*)(dos_header_address+(int)dos_header->e_lfanew+4);
	printf("MACHINE标记：%x \n",file_header->Machine);
	printf("NumberOfSections标记：%x \n",file_header->NumberOfSections);
	printf("TimeDateStamp标记：%x \n",file_header->TimeDateStamp);
	printf("PointerToSymbolTable标记：%x \n",file_header->PointerToSymbolTable);
	printf("NumberOfSymbols标记：%x \n",file_header->NumberOfSymbols);
	printf("SizeOfOptionalHeader标记：%x \n",file_header->SizeOfOptionalHeader);
	printf("Characteristics标记：%x \n",file_header->Characteristics);

//printf("************OPTION_PE头****************\n");

	IMAGE_OPTIONAL_HEADER* optional_header = (IMAGE_OPTIONAL_HEADER*)(dos_header_address+(int)dos_header->e_lfanew+0x18);
	printf("Magic标记：%x \n",optional_header->Magic);
	printf("MajorLinkerVersion标记：%x \n",optional_header->MajorLinkerVersion);
	printf("MinorLinkerVersion标记：%x \n",optional_header->MinorLinkerVersion);
	printf("SizeOfCode标记：%x \n",optional_header->SizeOfCode);
	printf("SizeOfInitializedData标记：%x \n",optional_header->SizeOfInitializedData);
	printf("SizeOfUninitializedData标记：%x \n",optional_header->SizeOfUninitializedData);
	printf("AddressOfEntryPoint标记：%x \n",optional_header->AddressOfEntryPoint);
	printf("BaseOfCode标记：%x \n",optional_header->BaseOfCode);
	printf("BaseOfData标记：%x \n",optional_header->BaseOfData);
	printf("ImageBase标记：%x \n",optional_header->ImageBase);
	printf("SectionAlignment标记：%x \n",optional_header->SectionAlignment);
	printf("FileAlignment标记：%x \n",optional_header->FileAlignment);
	printf("MajorOperatingSystemVersion标记：%x \n",optional_header->MajorOperatingSystemVersion);
	printf("MinorOperatingSystemVersion标记：%x \n",optional_header->MinorOperatingSystemVersion);
	printf("MajorImageVersion标记：%x \n",optional_header->MajorImageVersion);
	printf("MinorImageVersion标记：%x \n",optional_header->MinorImageVersion);
	printf("MinorImageVersion标记：%x \n",optional_header->MajorSubsystemVersion);
	printf("MinorSubsystemVersion标记：%x \n",optional_header->MinorSubsystemVersion);
	printf("Win32VersionValue标记：%x \n",optional_header->Win32VersionValue);
	printf("SizeOfImage标记：%x \n",optional_header->SizeOfImage);
	printf("SizeOfHeaders标记：%x \n",optional_header->SizeOfHeaders);
	printf("CheckSum标记：%x \n",optional_header->CheckSum);
	printf("Subsystem标记：%x \n",optional_header->Subsystem);
	printf("DllCharacteristics标记：%x \n",optional_header->DllCharacteristics);
	printf("SizeOfStackReserve标记：%x \n",optional_header->SizeOfStackReserve);
	printf("SizeOfStackCommit标记：%x \n",optional_header->SizeOfStackCommit);
	printf("SizeOfHeapReserve标记：%x \n",optional_header->SizeOfHeapReserve);
	printf("SizeOfHeapCommit标记：%x \n",optional_header->SizeOfHeapCommit);
	printf("LoaderFlags标记：%x \n",optional_header->LoaderFlags);
	printf("NumberOfRvaAndSizes标记：%x \n",optional_header->NumberOfRvaAndSizes);

printf("************节表信息****************\n");

	//获取节表头的地址
	IMAGE_SECTION_HEADER* section_header = (IMAGE_SECTION_HEADER*)(dos_header_address+(int)dos_header->e_lfanew+(int)file_header->SizeOfOptionalHeader+0x18);
	
	for(int x = 0;x<file_header->NumberOfSections;x++)
	{
	
		char section_name[9] = {0};
		for(int i = 0;i<8;i++)
		{
			section_name[i] = section_header->Name[i];
		}
		printf("节表名称: %s\n",section_name);
		printf("虚拟内存中节表的大小：%x \n",section_header->Misc);

		printf("VirtualAddress的大小：%x \n",section_header->VirtualAddress);
		printf("SizeOfRawData的大小：%x \n",section_header->SizeOfRawData);
		printf("PointerToRawData的大小：%x \n",section_header->PointerToRawData);
		printf("PointerToRelocations的大小：%x \n",section_header->PointerToRelocations);
		
		printf("PointerToLinenumbers的大小：%x \n",section_header->PointerToLinenumbers);
		printf("NumberOfRelocations的大小：%x \n",section_header->NumberOfRelocations);
		printf("NumberOfLinenumbers的大小：%x \n",section_header->NumberOfLinenumbers);
		printf("Characteristics的大小：%x \n",section_header->Characteristics);

	section_header = (IMAGE_SECTION_HEADER*)((int)section_header + 0x28);
	}
	

-------------

	free(pmalloc2);
		pmalloc2 = NULL;

		fclose(fp2);
		return 1;
	}
