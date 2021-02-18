	
	#define BYTE char
	#define WORD short
	#define DWORD int
	#define FILE_PATH "E:\\notepad.exe"
	struct IMAGE_NT_HEADERS;
	struct IMAGE_FILE_HEADER;
	struct IMAGE_OPTIONAL_HEADER;
	struct IMAGE_SECTION_HEADER;	
	





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
	
	
