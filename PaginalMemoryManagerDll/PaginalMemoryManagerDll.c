// PaginalMemoryManagerDll.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//


#include "PaginalMemoryManagerDll.h"
#include <stdlib.h>

 static  MemoryManager memoryManager;

 // �������������� �������
 int _malloc (VirtualAddres ptr, size_t szBlock){
	printf("malloc\n");
	return 0;
}


 int _free (VirtualAddres ptr){
	printf("free\n");
	return 0;
}

 int _read (VirtualAddres ptr, void* pBuffer, size_t szBuffer){
	printf("read\n");
	return 0;
}

 int _write (VirtualAddres ptr, void* pBuffer, size_t szBuffer){
	printf("write\n");
	return 0;
}

 int _init (int n, int szPage){
	printf("init\n");
	return 0;
}

 // C���������� �������

 static int _fileInit(){
	 
 }