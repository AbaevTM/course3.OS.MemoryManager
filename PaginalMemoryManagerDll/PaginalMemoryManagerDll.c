// PaginalMemoryManagerDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "PaginalMemoryManagerDll.h"
#include <stdlib.h>
#include <math.h>

 static  MemoryManager memoryManager;

  // Cтатические функции

 static int _fileInit( size_t n, size_t szPage ){
	 VirtualAddres buffer = (VirtualAddres) calloc( n*szPage, sizeof(char));
	 if( fopen_s( &(memoryManager.pageFile), "pageFile", "wb") == 0 &&
		 buffer != NULL){
			 if ( fwrite(buffer, sizeof(char), n*szPage, memoryManager.pageFile) == n*szPage){
			fflush(memoryManager.pageFile);
			fseek( memoryManager.pageFile , 0, SEEK_SET);
			memoryManager.numberOfPages = n;
			memoryManager.sizeOfPage = szPage;
			return 0;
		}
	 }
	 return 1;
 }

 static int getAddressOffset( size_t numberOfPages ){
	/*size_t mask = 1073741824;
	unsigned short addressOffset = 0;
	printf("Mask %d\n", mask);
	printf("NumberOfpages %d\n", numberOfPages);
	for ( addressOffset = 0; addressOffset < 32; addressOffset += 1){
		if ( numberOfPages & mask == mask ){
			memoryManager.addressOffset = 32 - addressOffset;
			printf("%d\n", 32 - addressOffset);
			return 0;
		}
		numberOfPages << 1;
	}*/
	return 1;
 }

 static int _pagesInit(size_t numberOfPages, size_t pageSize){
	 PhysicalAdress eachPageOffset;
	 size_t index;
	 memoryManager.physicalMemoryBegin = (PhysicalAdress) calloc(numberOfPages * pageSize , sizeof(char));
	 memoryManager.pages = (Page*) calloc ( numberOfPages , sizeof(Page));
	  if(memoryManager.physicalMemoryBegin == NULL ||
		  memoryManager.pages == NULL ||
		  getAddressOffset( numberOfPages) != 0
		  ){
		 return 1;
	 }
	  eachPageOffset = memoryManager.physicalMemoryBegin;
	 for(index = 0; index < numberOfPages ; 	++index){
		 memoryManager.pages[index].isModefied = false;
		 memoryManager.pages[index].isPresent = false;
		 memoryManager.pages[index].physicalAddress = eachPageOffset;
		 eachPageOffset += pageSize;
	 }	
	 return 0;
 }

 static int _memoryBlocksInit(size_t numberOfPages, size_t pageSize){
	 memoryManager.firstMemoryBlock = (MemoryBlock *) malloc(sizeof(MemoryBlock));
	 if(memoryManager.firstMemoryBlock == NULL){
		return 1;
	 }
	memoryManager.firstMemoryBlock->blockAdress = NULL;
	memoryManager.firstMemoryBlock->blockSize = numberOfPages*pageSize;
	memoryManager.firstMemoryBlock->isFree = true;
	memoryManager.firstMemoryBlock->next = NULL;
	memoryManager.firstMemoryBlock->previous = NULL;
	return 0;
 }


 // Экспортируемые функции
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

 int _init (size_t n, size_t szPage){
	 if( n == 0 || szPage == 0){
		return -1;
	 }
	 if ( _fileInit( n, szPage) == 0){
		 if( _pagesInit( n, szPage) == 0){
			 if( _memoryBlocksInit ( n, szPage) == 0){
				return 0;
			 }
		 }
	 }	
	return 1;
}

