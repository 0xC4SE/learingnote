// test.cpp : �������̨Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include <windows.h>
//Ҫע���dll�ļ�
char* path = "c:/123.exe";
//Ҫע���dll·��
char* dllPath = "c:/testdll.dll";

//ע�����ļ�
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

//�½ڵĴ�С

DWORD newSectionSize = 0x1000;

int _tmain(int argc, _TCHAR* argv[])
{
	

	fileBuffer = getFileBuffer();

	
	//DOSͷ
	dosHeader = (PIMAGE_DOS_HEADER)fileBuffer;
	//NTͷ
	ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(fileBuffer + (dosHeader->e_lfanew));
	//��׼PEͷ
	fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//��ѡPEͷ
	optionalHeader = (PIMAGE_OPTIONAL_HEADER)&(ntHeader->OptionalHeader);
	//�ڱ�
	sectionHeader = getSection(fileBuffer);

	PIMAGE_DATA_DIRECTORY basePoint = optionalHeader->DataDirectory;
	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		basePoint = basePoint + 2;
	}

	//�����     RVAתFOA ��ʱ���ı�ڱ�ĳ�ʼλ�ã����Զ���һ������������õ��ڱ��ʼλ�� PIMAGE_SECTION_HEADER getSection();
	improtDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(fileBuffer + RvaToFoa(fileBuffer, (basePoint + 1)->VirtualAddress));

	//���н��ܴ�С
	DWORD sectionNumble = (sizeof(*sectionHeader))* fileHeader->NumberOfSections;
	//����ͷ

	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		char* sectionTemp = (char*)malloc(sizeof(*ntHeader) + sectionNumble + 16);
		//��ԭ�������ݿ���һ��
		memcpy(sectionTemp, (char*)ntHeader, sizeof(*ntHeader) + sectionNumble + 16);
		//��0���Ƶ�ԭ���ڱ��λ��
		memset(sectionHeader, 0, sectionNumble);
		//�����������ݷŵ�DOSͷ�����λ��
		memcpy(fileBuffer + sizeof(*dosHeader), sectionTemp, sizeof(*ntHeader) + sectionNumble + 16);
		free(sectionTemp);

	}
	else
	{
		char* sectionTemp = (char*)malloc(sizeof(*ntHeader) + sectionNumble);
		//��ԭ�������ݿ���һ��
		memcpy(sectionTemp, (char*)ntHeader, sizeof(*ntHeader) + sectionNumble);
		//��0���Ƶ�ԭ���ڱ��λ��
		memset(sectionHeader, 0, sectionNumble);
		//�����������ݷŵ�DOSͷ�����λ��
		memcpy(fileBuffer + sizeof(*dosHeader), sectionTemp, sizeof(*ntHeader) + sectionNumble);
		free(sectionTemp);
	}
	

	//�޸�ͷ����ʼλ��
	dosHeader->e_lfanew = sizeof(*dosHeader);


	char* newBuffer = (char*)malloc(fileSize + newSectionSize);

	memset(newBuffer, 0, fileSize + newSectionSize);

	memcpy(newBuffer, fileBuffer, fileSize);

	free(fileBuffer);

	//���¶�λPE�ṹ
	//DOSͷ
	dosHeader = (PIMAGE_DOS_HEADER)newBuffer;
	//NTͷ
	ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(newBuffer + (dosHeader->e_lfanew));
	//��׼PEͷ
	fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//��ѡPEͷ
	optionalHeader = (PIMAGE_OPTIONAL_HEADER)&(ntHeader->OptionalHeader);
	//�ڱ�
	sectionHeader = getSection(newBuffer);

	basePoint = optionalHeader->DataDirectory;
	if (fileHeader->SizeOfOptionalHeader == 0xF0)
	{
		basePoint = basePoint + 2;
	}
	//�����     RVAתFOA ��ʱ���ı�ڱ�ĳ�ʼλ�ã����Զ���һ������������õ��ڱ��ʼλ�� PIMAGE_SECTION_HEADER getSection();
	improtDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)newBuffer + RvaToFoa(newBuffer, (basePoint + 1)->VirtualAddress));

	sectionNumble = fileHeader->NumberOfSections * sizeof(*sectionHeader);

	char* newSectionPos = ((char*)sectionHeader + sectionNumble);

	PIMAGE_SECTION_HEADER  newSection = (PIMAGE_SECTION_HEADER)newSectionPos;
	//����һ���ڱ��Ƹ������Ľ�
	memcpy(newSection, sectionHeader, sizeof(*sectionHeader));
	//���½ڱ����������
	memset(newSection->Name, 0, 8);
	//�޸��½ڱ�
	memcpy(newSection->Name, ".vmp", 4);
	newSection->PointerToRawData = fileSize;


	PIMAGE_SECTION_HEADER lastSection = (PIMAGE_SECTION_HEADER)((DWORD)sectionHeader + sectionNumble-40);
	DWORD lastSectionSize = lastSection->VirtualAddress + lastSection->Misc.VirtualSize;

	//����½ڵ�ƫ��
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
	
	


	//��λ������µĿ�ʼλ��
	char* importPos = fileBuffer + newSection->PointerToRawData;
	//��������Ƶ��µĽڣ����õ�������ƶ����λ��
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

	//INT��
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
	//��DLL��RVA
	funNameSize = funNameSize + 24;
	newImport->Name = FoaToRva(fileBuffer, funNameSize);
	//��DLL����
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
	//dosͷ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NTͷ
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//��׼PEͷ
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//��ѡPEͷ
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
	//dosͷ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NTͷ
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//��׼PEͷ
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//�ڱ�
	PIMAGE_SECTION_HEADER  SectionHeader = getSection(FileBuffer);
	
	for (int i = 0; i < fileHeader->NumberOfSections; i++)
	{
		//���RVA���ڵ��ڽ��ڴ�ƫ��  ��   RVAС�ڵ��ڽڵ�ƫ�� +  �ڵĴ�С   ע��RVA�ĵ�ַ���� ���� �ڵ�ƫ�� + �ڵĴ�С
		
		if (rva >= SectionHeader->VirtualAddress && rva <= SectionHeader->VirtualAddress + SectionHeader->SizeOfRawData)
		{
			//��������  ˵�����RVA������ڵķ�Χ  ��RVA ��ȥ ���ڵ��ڴ�ƫ�� �� �ڵ��ļ�ƫ�ƣ� ���� FOA
			rva = rva - (SectionHeader->VirtualAddress - SectionHeader->PointerToRawData);
			return rva;
		}
		else if (rva < SectionHeader->VirtualAddress)//RVAС�ڽڵ��ڴ�ƫ�ƣ�˵��RVA��ͷ�� ֱ�ӷ��ؾͿ�����
		{
			return rva;
		}
		SectionHeader = SectionHeader + 1;//������������ָ����һ����
	}

}

DWORD FoaToRva(char* FileBuffer,DWORD foa)
{
	//dosͷ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	//NTͷ
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)(DWORD)(FileBuffer + (dosHeader->e_lfanew));
	//��׼PEͷ
	PIMAGE_FILE_HEADER fileHeader = (PIMAGE_FILE_HEADER)&(ntHeader->FileHeader);
	//�ڱ�
	PIMAGE_SECTION_HEADER  SectionHeader = getSection(FileBuffer);
	for (int i = 0; i < fileHeader->NumberOfSections; i++)
	{
		//���RVA���ڵ��ڽ��ڴ�ƫ��  ��   RVAС�ڵ��ڽڵ�ƫ�� +  �ڵĴ�С   ע��RVA�ĵ�ַ���� ���� �ڵ�ƫ�� + �ڵĴ�С

		if (foa >= SectionHeader->PointerToRawData && foa < SectionHeader->PointerToRawData + SectionHeader->SizeOfRawData)
		{
			//��������  ˵�����RVA������ڵķ�Χ  ��RVA ��ȥ ���ڵ��ڴ�ƫ�� �� �ڵ��ļ�ƫ�ƣ� ���� FOA
			return foa + (SectionHeader->VirtualAddress - SectionHeader->PointerToRawData);
		}
		else if (foa < SectionHeader->PointerToRawData)//RVAС�ڽڵ��ڴ�ƫ�ƣ�˵��RVA��ͷ�� ֱ�ӷ��ؾͿ�����
		{
			return foa;
		}
		SectionHeader = SectionHeader + 1;//������������ָ����һ����
	}

}

