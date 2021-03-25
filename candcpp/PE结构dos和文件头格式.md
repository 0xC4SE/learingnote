	
DOS头（分Header和DOS存根）
===============

	typedef struct _IMAGE_DOS_HEADER { // DOS的.EXE头部
	  USHORT e_magic;  '// DOS签名“MZ-->Mark Zbikowski（设计了DOS的工程师）” -> 4D 5A '
	  USHORT e_cblp;    // 文件最后页的字节数 -> 00 90 -> 144
	  USHORT e_cp;      // 文件页数 -> 00 30 -> 48
	  USHORT e_crlc;    // 重定义元素个数 -> 00 00
	  USHORT e_cparhdr; // 头部尺寸，以段落为单位 -> 00 04
	  USHORT e_minalloc; // 所需的最小附加段 -> 00 00
	  USHORT e_maxalloc; // 所需的最大附加段 -> FF FF
	  USHORT e_ss;       // 初始的SS值（相对偏移量） -> 00 00
	  USHORT e_sp;       // 初始的SP值 -> 00 B8 -> 184
	  USHORT e_csum;     // 校验和 -> 00 00
	  USHORT e_ip;       // 初始的IP值 -> 00 00
	  USHORT e_cs;       // 初始的CS值（相对偏移量） -> 00 00
	  USHORT e_lfarlc;   // 重分配表文件地址 -> 00 40 -> 64
	  USHORT e_ovno;     // 覆盖号 -> 00 00
	  USHORT e_res[4];   // 保留字 -> 00 00 00 00 00 00 00 00
	  USHORT e_oemid;    // OEM标识符（相对e_oeminfo） -> 00 00
	  USHORT e_oeminfo;  // OEM信息 -> 00 00
	  USHORT e_res2[10]; // 保留字 -> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	  LONG e_lfanew;    '// 指示NT头的偏移（根据不同文件拥有可变值） -> 00 00 00 C0 -> 192'
	} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

NT头（PE最重要的头，长度由DOS头 e_lfanew 决定）
=========
	
##IMAGE_ NT _HEADERS32

	typedef struct _IMAGE_NT_HEADERS { 
	        DWORD Signature;                         // 类似于DOS头中的e_magic -> 00 00 45 50 -> PE
	        IMAGE_FILE_HEADER FileHeader;             // IMAGE_FILE_HEADER是PE文件头，定义如下
	        IMAGE_OPTIONAL_HEADER32 OptionalHeader;   // 
	} IMAGE_NT_HEADERS32, *PIMAGE_NT_HEADERS32;  

###2.IMAGE_ FILE _HEADER:

	typedef struct _IMAGE_FILE_HEADER { 
	        WORD    Machine;              '// 每个CPU拥有唯一的Machine码 -> 4C 01 -> PE -> 兼容32位Intel X86芯片'
	
	        WORD    NumberOfSections;     '// 指文件中存在的节段（又称节区）数量，也就是节表中的项数 -> 00 04 -> 4
	                                       // 该值一定要大于0，且当定义的节段数与实际不符时，将发生运行错误。'
	
	        DWORD   TimeDateStamp;         // PE文件的创建时间，一般有连接器填写 -> 38 D1 29 1E
	        DWORD   PointerToSymbolTable;  // COFF文件符号表在文件中的偏移 -> 00 00 00 00
	        DWORD   NumberOfSymbols;       // 符号表的数量 -> 00 00 00 00
	
	        WORD    SizeOfOptionalHeader; '// 指出IMAGE_OPTIONAL_HEADER32结构体的长度。->  00 E0 -> 224字节
	                                       // PE32+格式文件中使用的是IMAGE_OPTIONAL_HEADER64结构体，
	                                       // 这两个结构体尺寸是不相同的，所以需要在SizeOfOptionalHeader中指明大小。'
	
	        WORD    Characteristics;      '// 标识文件的属性，二进制中每一位代表不同属性 -> 0F 01
	                                       // 属性参见https://blog.csdn.net/qiming_zhang/article/details/7309909#3.2.2'
	} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

###3.IMAGE_ OPTIONAL _HEADER:

	typedef struct _IMAGE_OPTIONAL_HEADER { 
        WORD    Magic;                     '// 魔数 32位为0x10B，64位为0x20B，ROM镜像为0x107'
        BYTE    MajorLinkerVersion;         // 链接器的主版本号 -> 05
        BYTE    MinorLinkerVersion;         // 链接器的次版本号 -> 0C
        DWORD   SizeOfCode;                 // 代码节大小，一般放在“.text”节里，必须是FileAlignment的整数倍 -> 40 00 04 00
        DWORD   SizeOfInitializedData;      // 已初始化数大小，一般放在“.data”节里，必须是FileAlignment的整数倍 -> 40 00 0A 00
        DWORD   SizeOfUninitializedData;    // 未初始化数大小，一般放在“.bss”节里，必须是FileAlignment的整数倍 -> 00 00 00 00
        DWORD   AddressOfEntryPoint;       '// 指出程序最先执行的代码起始地址(RVA) -> 00 00 10 00'
        DWORD   BaseOfCode;                 // 代码基址，当镜像被加载进内存时代码节的开头RVA。必须是SectionAlignment的整数倍 -> 40 00 10 00

        DWORD   BaseOfData;                 // 数据基址，当镜像被加载进内存时数据节的开头RVA。必须是SectionAlignment的整数倍 -> 40 00 20 00
                                            // 在64位文件中此处被并入紧随其后的ImageBase中。

        DWORD   ImageBase;                 '// 当加载进内存时，镜像的第1个字节的首选地址。
                                            // WindowEXE默认ImageBase值为00400000，DLL文件的ImageBase值为10000000，也可以指定其他值。
                                            // 执行PE文件时，PE装载器先创建进程，再将文件载入内存，
                                            // 然后把EIP寄存器的值设置为ImageBase+AddressOfEntryPoint'

                                           '// PE文件的Body部分被划分成若干节段，这些节段储存着不同类别的数据。'
        DWORD   SectionAlignment;          '// SectionAlignment指定了节段在内存中的最小单位， -> 00 00 10 00'
        DWORD   FileAlignment;             '// FileAlignment指定了节段在磁盘文件中的最小单位，-> 00 00 02 00
                                            // SectionAlignment必须大于或者等于FileAlignment'

        WORD    MajorOperatingSystemVersion;// 主系统的主版本号 -> 00 04
        WORD    MinorOperatingSystemVersion;// 主系统的次版本号 -> 00 00
        WORD    MajorImageVersion;          // 镜像的主版本号 -> 00 00
        WORD    MinorImageVersion;          // 镜像的次版本号 -> 00 00
        WORD    MajorSubsystemVersion;      // 子系统的主版本号 -> 00 04
        WORD    MinorSubsystemVersion;      // 子系统的次版本号 -> 00 00
        DWORD   Win32VersionValue;          // 保留，必须为0 -> 00 00 00 00

        DWORD   SizeOfImage;               '// 当镜像被加载进内存时的大小，包括所有的文件头。向上舍入为SectionAlignment的倍数。
                                            // 一般文件大小与加载到内存中的大小是不同的。 -> 00 00 50 00'

        DWORD   SizeOfHeaders;             '// 所有头的总大小，向上舍入为FileAlignment的倍数。
                                            // 可以以此值作为PE文件第一节的文件偏移量。-> 00 00 04 00'

        DWORD   CheckSum;                   // 镜像文件的校验和 -> 00 00 B4 99

        WORD    Subsystem;                 '// 运行此镜像所需的子系统 -> 00 02 -> 窗口应用程序
                                            // 用来区分系统驱动文件（*.sys)与普通可执行文件（*.exe，*.dll），
                                            // 参考：https://blog.csdn.net/qiming_zhang/article/details/7309909#3.2.3'

        WORD    DllCharacteristics;         // DLL标识 -> 00 00
        DWORD   SizeOfStackReserve;         // 最大栈大小。CPU的堆栈。默认是1MB。-> 00 10 00 00
        DWORD   SizeOfStackCommit;          // 初始提交的堆栈大小。默认是4KB -> 00 00 10 00
        DWORD   SizeOfHeapReserve;          // 最大堆大小。编译器分配的。默认是1MB ->00 10 00 00
        DWORD   SizeOfHeapCommit;           // 初始提交的局部堆空间大小。默认是4K ->00 00 10 00
        DWORD   LoaderFlags;                // 保留，必须为0 -> 00 00 00 00

        DWORD   NumberOfRvaAndSizes;       '// 指定DataDirectory的数组个数，由于以前发行的Windows NT的原因，它只能为16。 -> 00 00 00 10'
        IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES]; '// 数据目录数组。详见下文。' 
    } IMAGE_OPTIONAL_HEADER32, *PIMAGE_OPTIONAL_HEADER32;

###DataDirectory[ ] 数据目录数组:

	typedef struct _IMAGE_DATA_DIRECTORY {  
	    DWORD   VirtualAddress;  
	    DWORD   Size;  
	} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

	#define IMAGE_DIRECTORY_ENTRY_EXPORT          0   // Export Directory '
	#define IMAGE_DIRECTORY_ENTRY_IMPORT          1   // Import Directory '
	#define IMAGE_DIRECTORY_ENTRY_RESOURCE        2   // Resource Directory 
	#define IMAGE_DIRECTORY_ENTRY_EXCEPTION       3   // Exception Directory 
	#define IMAGE_DIRECTORY_ENTRY_SECURITY        4   // Security Directory 
	#define IMAGE_DIRECTORY_ENTRY_BASERELOC       5   // Base Relocation Table 
	#define IMAGE_DIRECTORY_ENTRY_DEBUG           6   // Debug Directory 
	//      IMAGE_DIRECTORY_ENTRY_COPYRIGHT       7   // (X86 usage) 
	#define IMAGE_DIRECTORY_ENTRY_ARCHITECTURE    7   // Architecture Specific Data 
	#define IMAGE_DIRECTORY_ENTRY_GLOBALPTR       8   // RVA of GP 
	#define IMAGE_DIRECTORY_ENTRY_TLS             9   // TLS Directory 
	#define IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG    10   // Load Configuration Directory 
	#define IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT   11   // Bound Import Directory in headers 
	#define IMAGE_DIRECTORY_ENTRY_IAT            12   // Import Address Table 
	#define IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT   13   // Delay Load Import Descriptors 
	#define IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR 14   // COM Runtime descriptor
		