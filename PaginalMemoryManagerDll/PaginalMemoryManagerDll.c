// PaginalMemoryManagerDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "mmemory.h"
#include <stdlib.h>

 static  MemoryManager memoryManager;

  // Cтатические функции

 /////
 ///// PAGINAL MEMORY
 /////
 static int _fileInit( size_t n, size_t szPage ){
	 VirtualAddress buffer = (VirtualAddress) calloc( n*szPage, sizeof(char));
	 if( fopen_s( &(memoryManager.pageFile), "pageFile", "wb") == 0 &&
		 buffer != NULL){
			 if ( fwrite(buffer, sizeof(char), n*szPage, memoryManager.pageFile) == n*szPage){
			fflush(memoryManager.pageFile);
			fseek( memoryManager.pageFile , 0, SEEK_SET);
			memoryManager.numberOfPages = n;
			memoryManager.sizeOfPage = szPage;
			memoryManager.virtualAddressSize = n*szPage;
			return 0;
		}
	 }
	 return 1;
 }

 static int setOffsets(){
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
	 PhysicalAddress eachPageOffset;
	 size_t index;
	 memoryManager.physicalMemoryBegin = (PhysicalAddress) calloc(numberOfPages * pageSize , sizeof(char));
	 memoryManager.pages = (Page*) calloc ( numberOfPages , sizeof(Page));
	  if(memoryManager.physicalMemoryBegin == NULL ||
		  memoryManager.pages == NULL ||
		  setOffsets() != 0
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

 /////
 ///// BLOCK MEMORY
 /////

 static MemoryBlock * findFreeBlock(size_t size){
	 MemoryBlock *iterator = memoryManager.firstMemoryBlock; 	
	 while(iterator != NULL){
		 if(iterator->isFree && iterator->blockSize >= size){
			return iterator;
		 }
	 }
	 return NULL;
 }

 static void switchBlocks(MemoryBlock *freeBlock, MemoryBlock * newBlock){
	freeBlock->previous->next = newBlock;
	newBlock->previous = freeBlock->previous;
	newBlock->next = freeBlock;
	freeBlock->previous = newBlock;
 }

 static MemoryBlock * getBlock(VirtualAddress adress){
	 MemoryBlock *iterator = memoryManager.firstMemoryBlock;
	 while(iterator != NULL){
		 if(iterator->blockAdress == adress){
			 return iterator;
		 }
	 }
	 return NULL;
 }


 // Экспортируемые функции
int _malloc (VirtualAddress ptr, size_t szBlock){
	MemoryBlock * freeBlock;
	MemoryBlock * newBlock;
	VirtualAddress adress;
	printf("malloc\n");
	if(!szBlock){
		return -1;
	}
	freeBlock = findFreeBlock(szBlock);
	if(!freeBlock){
		return -2;
	}
	newBlock = (MemoryBlock *)malloc(sizeof(MemoryBlock));
	adress = freeBlock->blockAdress;
	freeBlock->blockAdress = adress + szBlock;
	freeBlock->blockSize = freeBlock->blockSize - szBlock;
	newBlock->blockAdress = adress;
	newBlock->blockSize = szBlock;
	newBlock->isFree = false;
	if(!(freeBlock->previous)){
		newBlock->next = freeBlock;
		freeBlock->previous = newBlock;
		newBlock->previous = NULL;
		memoryManager.firstMemoryBlock = newBlock;
	}
	else{
		switchBlocks(freeBlock, newBlock);
	}
	ptr = adress;
	return 0;
}


 int _free (VirtualAddress ptr){
	MemoryBlock * deletedBlock;
	MemoryBlock *temp;
	printf("free\n");
	/*deletedBlock = getBlock(ptr);
	if(!deletedBlock){
		return -1;	 
	}
	//if dissmised block is first and next one is not free
	if(deletedBlock->previous == NULL && !deletedBlock->next->isFree){
		deletedBlock->isFree = true;
		return 0;
	}
	// -//- is first and the next one is free
	if(deletedBlock->previous == NULL && deletedBlock->next){
		temp = deletedBlock->next;
		temp->blockSize = temp->blockSize + deletedBlock->blockSize;
		temp->blockAdress = deletedBlock->blockAdress;
		temp->previous = NULL;
		memoryManager.firstMemoryBlock = temp;
		free(deletedBlock);
		return 0;
	}
	// -//- is not first and next one is free and previous one is not
	if(deletedBlock->next->isFree && !deletedBlock->previous->isFree){
		temp = deletedBlock->next;
		temp->blockSize = temp->blockSize + deletedBlock->blockSize;
		temp->blockAdress = deletedBlock->blockAdress;
		temp->previous = deletedBlock->previous;
	}*/
	return 0;
}

 int _read (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
	printf("read\n");
	return 0;
}

 int _write (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
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

