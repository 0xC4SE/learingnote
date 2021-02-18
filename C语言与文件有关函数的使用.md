
	下面是 fseek() 函数的声明。
	
	int fseek(FILE *stream, long int offset, int whence)
	参数
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
	offset -- 这是相对 whence 的偏移量，以字节为单位。	
	whence -- 这是表示开始添加偏移 offset 的位置。它一般指定为下列常量之一：
	常量     		描述
	>SEEK_SET	文件的开头
	>SEEK_CUR	文件指针的当前位置
	>SEEK_END	文件的末尾

	#include <stdio.h>
	int main ()
	{
	   FILE *fp;
	
	   fp = fopen("file.txt","w+");
	   fputs("This is runoob.com", fp);	  									
	   fseek( fp, 7, SEEK_SET );				//fseek的功能主要是将指针指向文件流的开始或者结尾(当然可以是中间位置)
	   fputs(" C Programming Langauge", fp);	//文件流操作过程中fputs是比较简单的赋值函数
	   fclose(fp);   
	   return(0);
	}					//让我们编译并运行上面的程序，这将创建文件 file.txt，它的内容如下。最初程序创建文件和写入 This is runoob.com，
						但是之后我们在第七个位置重置了写指针，并使用 puts() 语句来重写文件,内容如下:This is C Programming Langauge

-------------------------
	
	下面是 fwrite() 函数的声明。

	size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	参数
	ptr -- 这是指向要被写入的元素数组的指针。
	size -- 这是要被写入的每个元素的大小，以字节为单位。
	nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
	返回值
	如果成功，该函数返回一个 size_t 对象，表示元素的总数，该对象是一个整型数据类型。如果该数字与 nmemb 参数不同，则会显示一个错误
	
	#include<stdio.h>
	int main ()
	{
	   FILE *fp;
	   char str[] = "This is runoob.com";		//可以是一个内存中指针，将内存中的数据写入到本地文件当中
	 
	   fp = fopen( "file.txt" , "w" );
	   fwrite(str, sizeof(str) , 1, fp );
	   fclose(fp);	  
	   return(0);		//让我们编译并运行上面的程序，这将创建一个文件 file.txt，它的内容如下：This is runoob.com
	}	

------------------------

	下面是 fread() 函数的声明。

	size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	参数
	ptr -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
	size -- 这是要读取的每个元素的大小，以字节为单位。
	nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
	返回值
	成功读取的元素总数会以size_t对象返回，size_t对象是一个整型数据类型。如果总数与nmemb参数不同，则可能发生了一个错误或者到达了文件末尾。

	#include <stdio.h>
	#include <string.h> 
	int main()
	{
	   FILE *fp;
	   char c[] = "This is runoob";
	   char buffer[20];
	 
	   /* 打开文件用于读写 */
	   fp = fopen("file.txt", "w+");
	   /* 写入数据到文件 */
	   fwrite(c, strlen(c) + 1, 1, fp);
	   /* 查找文件的开头 */
	   fseek(fp, 0, SEEK_SET);
	   /* 读取并显示数据 */
	   fread(buffer, strlen(c)+1, 1, fp);
	   printf("%s\n", buffer);
	   fclose(fp);			//该函数共能是从给定文件流stream读取数据到内存中，与fwrite相反	   
	   return(0);		
	}		

---------------------------

	下面是 ftell() 函数的声明。
	
	long int ftell(FILE *stream)
	参数
	stream -- 这是指向 FILE 对象的指针，该 FILE 对象标识了流。
	返回值
	该函数返回位置标识符的当前值。如果发生错误，则返回 -1L，全局变量 errno 被设置为一个正值。	

	#include <stdio.h>
	int main ()
	{
	   FILE *fp;
	   int len;
	
	   fp = fopen("file.txt", "r");
	   if( fp == NULL ) 
	   {
	      perror ("打开文件错误");
	      return(-1);
	   }
	   fseek(fp, 0, SEEK_END);
	   len = ftell(fp);				//ftell的功能是告诉该文件流的大小长度，使用前一般用fseek将指针指向文件流结尾，来获取大小
	   fclose(fp);		
	   printf("file.txt 的总大小 = %d 字节\n", len);	   
	   return(0);
	}

---------------------------------

	FILE * fopen(const char * path,const char * mode);
	 -- path: 文件路径，如："F:\Visual Stdio 2012\test.txt"
	 -- mode: 文件打开方式，例如：
     	"r" 以只读方式打开文件，该文件必须存在。
	    "w" 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
	    "w+" 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
	    "a" 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
	    "a+" 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。（原来的EOF符不保留）
	    "wb" 只写打开或新建一个二进制文件，只允许写数据。
	    "wb+" 读写打开或建立一个二进制文件，允许读和写。
	    "ab" 追加打开一个二进制文件，并在文件末尾写数据。
	    "ab+"读写打开一个二进制文件，允许读，或在文件末追加数据。   
	 --返回值: 文件顺利打开后，指向该流的文件指针就会被返回。如果文件打开失败则返回NULL，并把错误代码存在errno中。 


##练习：

1、将记事本的.exe文件读取到内存，并返回读取后在内存中的地址.								
								
2、将内存中的数据存储到一个文件中，(.exe格式)，然后双击打开，看是否能够使用.								

	#include "stdafx.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include <malloc.h>
	#include <memory.h>
	#include <string.h>
	int func()							
	{													
		FILE* fp = fopen("E:\\notepad.exe","rb"); 						
		if(fp == NULL)						
		{						
			printf("this file isnt open!\n");					
		}						
		//fwrite(arr,sizeof(arr),1,fp);						
		fseek(fp,0,SEEK_END);						
		int len = ftell(fp);						
		printf("%d \n",len);						//已经获得文件在内存中的大小													
		char* pmalloc;								// 在内存中分配堆，将文件读取到内存中			
		if(pmalloc == NULL)						
		{						
			return 0;					
		}						
		pmalloc = (char*)malloc(sizeof(char)*len);						
		memset(pmalloc,0,sizeof(char)*len);			//堆已经分配好						
		fseek(fp,0,0);						
		fread(pmalloc,sizeof(char)*len,1,fp);		//把文件流复制到分配的堆中						
		printf("文件流在堆内存中的地址是 %x ",pmalloc);						
		FILE* fp2 = fopen("E:\\notepad2.exe","wb+");						
		if(fp2 == NULL)						
		{						
			printf("文件未创建成功");					
		}						
		fwrite(pmalloc,sizeof(char)*len,1,fp2);														
		fclose(fp2);						
		free(pmalloc);						
		pmalloc = NULL;												
		fclose(fp);						
		return 1;						
	}							
