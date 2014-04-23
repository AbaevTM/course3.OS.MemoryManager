#include "mmemory.h"
#include <stdlib.h>

#define NUMBER_OF_ACTIVE_PAGES 0.5;
  // Cтатические функции

 /////
 ///// PAGINAL MEMORY
 /////

 void readByteFromVirtualSpace( VirtualAddress virtualAddress, PhysicalAddress buffer){
 
 }

 void writeByteToVirtualSpace( VirtualAddress virtualAddress, PhysicalAddress buffer){
 
 }

 static void deleteMemoryBlocks(){
	 MemoryBlock * currentBlock = memoryManager.firstMemoryBlock; 
	 MemoryBlock * temp ;
	 if ( memoryManager.firstMemoryBlock != NULL){
		do{
			temp = currentBlock;
			currentBlock = currentBlock->next;
			free(currentBlock);
		} while( currentBlock);
		memoryManager.firstMemoryBlock = NULL;
	 }
	 return;
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

 static int setOffsets(){
	///// SET ADDRESSOFFSET
	 size_t numberOfPages = memoryManager.numberOfPages;
	// 01000000 00000000 00000000 00000000 mask
	size_t mask = 1<<30;
	unsigned short addressOffset = 0;
	unsigned short pageAddressOffset = 0;
	size_t maximalVirtualAddress = memoryManager.virtualAddressSize - 1;
	size_t maximalPageAddress = memoryManager.numberOfPages -1;
	//printf("Mask %d\n", mask);
	//printf("MaximalVirtualAddress %d\n", maximalVirtualAddress);
	for ( addressOffset = 0; addressOffset < 32; addressOffset += 1){
		if ( ( maximalVirtualAddress & mask ) != 0){
			memoryManager.addressOffset = addressOffset;
			//printf("Check %d\n", maximalVirtualAddress & mask);
			//printf("AddressOffset %d\n", addressOffset);
			break;
		}
		maximalVirtualAddress <<= 1;
		//printf("Number of pages offset %d i :%d\n", maximalVirtualAddress,addressOffset);
	}
	if ( addressOffset == 32){
		return 1;
	}
	///// SET PAGEADDRESSOFFSET
	if ( memoryManager.numberOfPages == 1){
		memoryManager.pageAddressOffset = 0;
		return 0;
		//printf("pageAddressOffset %d\n", memoryManager.pageAddressOffset);
	}
	else {
		for ( pageAddressOffset = 0; pageAddressOffset < 32; pageAddressOffset += 1){
		if ( ( maximalPageAddress & mask ) != 0){
			memoryManager.pageAddressOffset = 31 - pageAddressOffset;
			//printf("pageAddressOffset %d\n", memoryManager.pageAddressOffset);
			return 0;
		}
		maximalPageAddress <<= 1;
		//printf("Number of pages offset %d i :%d\n", maximalVirtualAddress,addressOffset);
		}	
	}
	return 1;
 }

 static size_t getNumberOfActivePages( size_t numberOfPages){
	 size_t numberOfAP;
	 if ( numberOfPages == 1){
		 return 1;
	 }
	 else {
		 numberOfAP = numberOfPages * NUMBER_OF_ACTIVE_PAGES;
		 //printf("%d\n\n", numberOfAP);
		 return numberOfAP;
	 }
 }

 static void deletePhysicalMemory(){
	 if(memoryManager.physicalMemoryBegin != NULL){
		 free(memoryManager.physicalMemoryBegin);
	 }
 }

 static void deletePages(){
	 if( memoryManager.pages){
		 free (memoryManager.pages);
	 }
 }

 static int _pagesInit(size_t numberOfPages, size_t pageSize){
	 PhysicalAddress eachPageOffset;
	 size_t index;
	 memoryManager.physicalMemoryBegin = (PhysicalAddress) calloc( getNumberOfActivePages( numberOfPages)*pageSize , sizeof(char));
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

static int _fileInit( size_t n, size_t szPage ){
	 VirtualAddress buffer = (VirtualAddress) calloc( n*szPage, sizeof(char));
	 if( memoryManager.pageFile){
		 fclose(memoryManager.pageFile);
	 }
	 if( fopen_s( &(memoryManager.pageFile), "pageFile", "wb") == 0 &&
		 buffer != NULL){
			if ( fwrite(buffer, sizeof(char), n*szPage, memoryManager.pageFile) == n*szPage){
			fflush(memoryManager.pageFile);
			fseek( memoryManager.pageFile , 0, SEEK_SET);
			memoryManager.numberOfPages = n;
			memoryManager.sizeOfPage = szPage;
			memoryManager.virtualAddressSize = n*szPage;
			free(buffer);
			//printf("FinishFileINIT\n");
			return 0;
		}
	 }
	 free(buffer);
	 return 1;
 }

int _init (size_t n, size_t szPage){
	 deletePhysicalMemory();
	 deletePages();
	 deleteMemoryBlocks();
	 if( n == 0 ||
		 szPage == 0 ||
		 (szPage & (szPage - 1)) != 0 ||
		 szPage == 1 ||
		 n*szPage > (2<<30) - 1 ||
		 n*szPage <= 0
		 ){
		 //printf("n*szPage : %d\n", n*szPage);
		 //printf("2<<30 : %d\n",(2<<30) - 1);
		return -1;
	 }
	 //printf("n*szPage : %d\n", n*szPage);
	 //printf("2<<30 : %d\n",(2<<30) - 1);
	 if ( _fileInit( n, szPage) == 0){
		 if( _pagesInit( n, szPage) == 0){
			 if( _memoryBlocksInit ( n, szPage) == 0){
				return 0;
			 }
		 }
	 }	
	deletePhysicalMemory();
	deletePages();
	deleteMemoryBlocks();
	return 1;
}