// test.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
//要注入的dll文件
char* path = "c:/123.exe";
//要注入的dll路径
char* dllPath = "c:/testdll.dll";

//注入后的文件
char* newfile = "c:/999999.exe";

PIMAGE_DOS_HEADER dosHeader = NULL;
PIMAGE_NT_HEADERS ntHeader = NULL;
PIMAGE_FILE_HEADER  fileHeader = NULL;
PIMAGE_OPTIONAL_HEADER optionalHeader = NULL;
PIMAGE_SECTION_HEADER  sectionHeader = NULL;
PIMAGE_IMPORT_DESCRIPTOR improtDescriptor = NULL;

char* fileBuffer = NULL;
char* getFileBuffer();
DWORD RvaToFoa(char* FileBuffer, DWORD rva);
PIMAGE_SECTION_HEADER getSection(char* FileBuffer);
DWORD FoaToRva(char* FileBuffer,DWORD foa);

DWORD fileSize;

//新节的大小

DWORD newSectionSize = 0x1000;

int _tmain(int argc, _TCHAR* argv[])
{
	

	fileBuffer = getFileBuffer();

	
	//DOS头
	dosHeader = (PIMAGE_DOS_HEADER)fileBuffer;
	//NT头
	ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(fileBuffer + (dosHeader->e_lfanew));
	//标准PE头
	fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//可选PE头
	optionalHeader = (PIMAGE_OPTIONAL_HEADER)&(ntHeader->OptionalHeader);
	//节表
	sectionHeader = getSection(fileBuffer);

	PIMAGE_DATA_DIRECTORY basePoint = optionalHeader->DataDirectory;
	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		basePoint = basePoint + 2;
	}

	//导入表     RVA转FOA 的时候会改变节表的初始位置，所以定义一个函数，方便得到节表初始位置 PIMAGE_SECTION_HEADER getSection();
	improtDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(fileBuffer + RvaToFoa(fileBuffer, (basePoint + 1)->VirtualAddress));

	//所有节总大小
	DWORD sectionNumble = (sizeof(*sectionHeader))* fileHeader->NumberOfSections;
	//提升头

	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		char* sectionTemp = (char*)malloc(sizeof(*ntHeader) + sectionNumble + 16);
		//将原来的数据拷贝一份
		memcpy(sectionTemp, (char*)ntHeader, sizeof(*ntHeader) + sectionNumble + 16);
		//将0复制到原来节表的位置
		memset(sectionHeader, 0, sectionNumble);
		//将拷贝的数据放到DOS头下面的位置
		memcpy(fileBuffer + sizeof(*dosHeader), sectionTemp, sizeof(*ntHeader) + sectionNumble + 16);
		free(sectionTemp);

	}
	else
	{
		char* sectionTemp = (char*)malloc(sizeof(*ntHeader) + sectionNumble);
		//将原来的数据拷贝一份
		memcpy(sectionTemp, (char*)ntHeader, sizeof(*ntHeader) + sectionNumble);
		//将0复制到原来节表的位置
		memset(sectionHeader, 0, sectionNumble);
		//将拷贝的数据放到DOS头下面的位置
		memcpy(fileBuffer + sizeof(*dosHeader), sectionTemp, sizeof(*ntHeader) + sectionNumble);
		free(sectionTemp);
	}
	

	//修复头的起始位置
	dosHeader->e_lfanew = sizeof(*dosHeader);


	char* newBuffer = (char*)malloc(fileSize + newSectionSize);

	memset(newBuffer, 0, fileSize + newSectionSize);

	memcpy(newBuffer, fileBuffer, fileSize);

	free(fileBuffer);

	//重新定位PE结构
	//DOS头
	dosHeader = (PIMAGE_DOS_HEADER)newBuffer;
	//NT头
	ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(newBuffer + (dosHeader->e_lfanew));
	//标准PE头
	fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//可选PE头
	optionalHeader = (PIMAGE_OPTIONAL_HEADER)&(ntHeader->OptionalHeader);
	//节表
	sectionHeader = getSection(newBuffer);

	basePoint = optionalHeader->DataDirectory;
	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		basePoint = basePoint + 2;
	}
	//导入表     RVA转FOA 的时候会改变节表的初始位置，所以定义一个函数，方便得到节表初始位置 PIMAGE_SECTION_HEADER getSection();
	improtDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)newBuffer + RvaToFoa(newBuffer, (basePoint + 1)->VirtualAddress));

	sectionNumble = fileHeader->NumberOfSections * sizeof(*sectionHeader);

	char* newSectionPos = ((char*)sectionHeader + sectionNumble);

	PIMAGE_SECTION_HEADER  newSection = (PIMAGE_SECTION_HEADER)newSectionPos;
	//将第一个节表复制给新增的节
	memcpy(newSection, sectionHeader, sizeof(*sectionHeader));
	//将新节表的名字清零
	memset(newSection->Name, 0, 8);
	//修复新节表
	memcpy(newSection->Name, ".vmp", 4);
	newSection->PointerToRawData = fileSize;


	PIMAGE_SECTION_HEADER lastSection = (PIMAGE_SECTION_HEADER)((DWORD)sectionHeader + sectionNumble-40);
	DWORD lastSectionSize = lastSection->VirtualAddress + lastSection->Misc.VirtualSize;

	//算出新节的偏移
	if (lastSectionSize % 1000 != 0)
	{
		newSection->VirtualAddress = (lastSectionSize - (lastSectionSize % 0x1000)) + 0x1000;
		
	}
	else
	{
		newSection->VirtualAddress = lastSectionSize + 0x1000;
	}
	//newSection->VirtualAddress = fileSize;
	newSection->PointerToRawData = lastSection->PointerToRawData + lastSection->SizeOfRawData;
	newSection->SizeOfRawData = 0x1000;
	newSection->Misc.VirtualSize = newSectionSize;
	newSection->Characteristics = 0xE0000040;

	fileBuffer = newBuffer;
	fileSize = fileSize + newSectionSize;
	fileHeader->NumberOfSections = fileHeader->NumberOfSections + 1;
	optionalHeader->SizeOfImage = optionalHeader->SizeOfImage + newSectionSize;
	
	


	//定位导入表新的开始位置
	char* importPos = fileBuffer + newSection->PointerToRawData;
	//将导入表复制到新的节，并得到导入表移动后的位置
	char* curPos = (char*)memcpy(importPos, improtDescriptor, (basePoint + 1)->Size);
	(basePoint + 1)->VirtualAddress = FoaToRva(fileBuffer, newSection->PointerToRawData);
	(basePoint + 1)->Size = (basePoint + 1)->Size + sizeof(*improtDescriptor);

	FILE* dllFile = fopen(dllPath,"rb");
	fseek(dllFile, 0, SEEK_END);
	DWORD dllSize = ftell(dllFile);
	 fseek(dllFile, 0, SEEK_SET);
	 char* dllBuffer = (char*)malloc(dllSize);
	 fread(dllBuffer, 1, dllSize, dllFile);

	 PIMAGE_NT_HEADERS  DLLnt = (PIMAGE_NT_HEADERS)(dllBuffer + ((PIMAGE_DOS_HEADER)dllBuffer)->e_lfanew);
	PIMAGE_OPTIONAL_HEADER DLLoptionHeader = &DLLnt->OptionalHeader;
	PIMAGE_EXPORT_DIRECTORY DLLexport = (PIMAGE_EXPORT_DIRECTORY)(dllBuffer + RvaToFoa(dllBuffer, (DLLoptionHeader->DataDirectory)->VirtualAddress));
	DWORD* dllFunc = (DWORD*)(dllBuffer + (RvaToFoa(dllBuffer, DLLexport->AddressOfNames)));

	//(sectionHeader->PointerToRawData + sectionHeader->Misc.VirtualSize) + (optionalHeader->DataDirectory + 1)->Size + (sizeof(*sectionHeader)*2);

	

	PIMAGE_IMPORT_DESCRIPTOR newImport;
	newImport = (PIMAGE_IMPORT_DESCRIPTOR)(importPos + (basePoint + 1)->Size - sizeof(*improtDescriptor) * 2);
	PIMAGE_IMPORT_BY_NAME newByName;

	sectionHeader = getSection(fileBuffer);
	

	DWORD* newRva = (DWORD*)(fileBuffer + newSection->PointerToRawData+(newSectionSize/3));

	char* newData = fileBuffer + (newSection->PointerToRawData + ((newSectionSize / 3)*2));

	newImport->OriginalFirstThunk = FoaToRva(fileBuffer, newSection->PointerToRawData+(newSectionSize/3));

	*newRva = FoaToRva(fileBuffer, newSection->PointerToRawData + (newSectionSize / 3));

	DWORD funNameSize = (newSection->PointerToRawData + ((newSectionSize / 3) * 2));

	//INT表
	for (int i = 0; i < DLLexport->NumberOfFunctions; i++)
	{
		*newRva =FoaToRva(fileBuffer,funNameSize);
		memcpy(newData + 2, (dllBuffer + RvaToFoa(dllBuffer, *dllFunc)), strlen((dllBuffer + RvaToFoa(dllBuffer, *dllFunc))));
		dllFunc = dllFunc + 1;
		newRva = newRva + 1;
		if (i == DLLexport->NumberOfFunctions - 1)
		{
			break;
		}
		funNameSize = funNameSize + strlen(dllBuffer + *dllFunc) + 2;
		newData = newData + strlen(dllBuffer + *dllFunc) + 2;
		
	}

	dllFunc = (DWORD*)(dllBuffer + (RvaToFoa(dllBuffer, DLLexport->AddressOfNames)));

	funNameSize = (fileBuffer, (newSection->PointerToRawData + (newSectionSize / 3) * 2));

	newImport->FirstThunk = FoaToRva(fileBuffer,(newSection->PointerToRawData + (newSectionSize / 3)) + DLLexport->NumberOfFunctions * 4 + 4);

	newRva = newRva + 1;
	newData = newData + 2;
	

	for (int i = 0; i < DLLexport->NumberOfFunctions; i++)
	{
		*newRva = FoaToRva(fileBuffer,funNameSize);
		dllFunc = dllFunc + 1;
		newRva = newRva + 1;
		if (i == DLLexport->NumberOfFunctions - 1)
		{
			break;
		}
		funNameSize = funNameSize + strlen(dllBuffer + *dllFunc) + 2;
	}
	//存DLL的RVA
	funNameSize = funNameSize + 24;
	newImport->Name = FoaToRva(fileBuffer, funNameSize);
	//存DLL名字
	char* newDllName = dllBuffer + RvaToFoa(dllBuffer, DLLexport->Name);
	memcpy(fileBuffer + funNameSize, newDllName, strlen(newDllName));

	FILE* outFile = fopen(newfile, "wb");


	fwrite(fileBuffer, 1, fileSize, outFile);


	return 0;
}


char* getFileBuffer()
{
	FILE* file = fopen(path, "rb");

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* FileBuffer = (char*)malloc(fileSize);
	fread(FileBuffer, 1, fileSize, file);
	fclose(file);
	return FileBuffer;
}

PIMAGE_SECTION_HEADER getSection(char* FileBuffer)
{
	//dos头
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NT头
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//标准PE头
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//可选PE头
	PIMAGE_OPTIONAL_HEADER optionalHeader = (PIMAGE_OPTIONAL_HEADER)&(ntHeader->OptionalHeader);
	PIMAGE_SECTION_HEADER  sectionHeader;
	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		sectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)optionalHeader + fileHeader->SizeOfOptionalHeader);
	}
	else
	{
		sectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)optionalHeader + fileHeader->SizeOfOptionalHeader);
	}
	
	return sectionHeader;
}

DWORD RvaToFoa(char* FileBuffer,DWORD rva)
{
	//dos头
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NT头
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//标准PE头
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//节表
	PIMAGE_SECTION_HEADER  SectionHeader = getSection(FileBuffer);
	
	for (int i = 0; i < fileHeader->NumberOfSections; i++)
	{
		//如果RVA大于等于节内存偏移  与   RVA小于等于节的偏移 +  节的大小   注：RVA的地址不会 大于 节的偏移 + 节的大小
		
		if (rva >= SectionHeader->VirtualAddress && rva <= SectionHeader->VirtualAddress + SectionHeader->SizeOfRawData)
		{
			//条件成立  说明这个RVA在这个节的范围  用RVA 减去 （节的内存偏移 减 节的文件偏移） 等于 FOA
			rva = rva - (SectionHeader->VirtualAddress - SectionHeader->PointerToRawData);
			return rva;
		}
		else if (rva < SectionHeader->VirtualAddress)//RVA小于节的内存偏移，说明RVA在头部 直接返回就可以了
		{
			return rva;
		}
		SectionHeader = SectionHeader + 1;//条件不成立则指向下一个节
	}

}

DWORD FoaToRva(char* FileBuffer,DWORD foa)
{
	//dos头
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NT头
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//标准PE头
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//节表
	PIMAGE_SECTION_HEADER  SectionHeader = getSection(FileBuffer);
	for (int i = 0; i < fileHeader->NumberOfSections; i++)
	{
		//如果RVA大于等于节内存偏移  与   RVA小于等于节的偏移 +  节的大小   注：RVA的地址不会 大于 节的偏移 + 节的大小

		if (foa >= SectionHeader->PointerToRawData && foa < SectionHeader->PointerToRawData + SectionHeader->SizeOfRawData)
		{
			//条件成立  说明这个RVA在这个节的范围  用RVA 减去 （节的内存偏移 减 节的文件偏移） 等于 FOA
			return foa + (SectionHeader->VirtualAddress - SectionHeader->PointerToRawData);
		}
		else if (foa < SectionHeader->PointerToRawData)//RVA小于节的内存偏移，说明RVA在头部 直接返回就可以了
		{
			return foa;
		}
		SectionHeader = SectionHeader + 1;//条件不成立则指向下一个节
	}

}

