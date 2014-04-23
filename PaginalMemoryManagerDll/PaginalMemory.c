#include "mmemory.h"
#include <stdlib.h>

#define NUMBER_OF_ACTIVE_PAGES 0.5;
  // Cтатические функции

 /////
 ///// PAGINAL MEMORY
 /////

static int saveVirtualPageFromPhysicalMemory( size_t virtualPageIndex, PhysicalAddress physicalAddress ){
	size_t sizeOfPage = memoryManager.sizeOfPage;
	size_t fileShift = sizeOfPage * virtualPageIndex;
	PhysicalAddress pagePhysicalAddress = physicalAddress;
	fseek(memoryManager.pageFile,fileShift,SEEK_CUR);
	if ( fwrite(physicalAddress,
		sizeof(char),
		sizeOfPage,
		memoryManager.pageFile
		) != sizeOfPage){
		return 1;
	}
	memoryManager.pages[virtualPageIndex].isModefied = false;
	return 0;
}

 static void moveReplacedPageNumber(){
	 size_t replacedPageNumber = memoryManager.replacedPageNumber;
	 if ( replacedPageNumber == memoryManager.numberOfPages - 1){
		 replacedPageNumber = 0;
	 } 
	 else {
		 ++replacedPageNumber;
	 }
	 memoryManager.replacedPageNumber = replacedPageNumber;
	 return;
 }

static size_t findNextPageToBeReplaced(){
	size_t nextPageToBeReplaced = memoryManager.replacedPageNumber;
	while(1){
		if( memoryManager.pages[memoryManager.replacedPageNumber].isPresent){
			return memoryManager.replacedPageNumber;
		}
		moveReplacedPageNumber();
	}
	return -1;
}


static int loadVirtualPageToPhysicalMemory( size_t virtualPageIndex, PhysicalAddress physicalAddress ){
	size_t index;
	size_t readBytes =0;
	size_t sizeOfPage = memoryManager.sizeOfPage;
	size_t fileShift = sizeOfPage * virtualPageIndex;
	PhysicalAddress pagePhysicalAddress = physicalAddress;
	PhysicalAddress buffer = (PhysicalAddress) malloc( sizeOfPage );
	PhysicalAddress leftPointer = buffer;
	if( !buffer ){
		return 1;
	}
	fseek(memoryManager.pageFile,fileShift,SEEK_CUR);
	readBytes = fread_s(buffer,
		sizeOfPage,
		sizeof(char),
		sizeOfPage,
		memoryManager.pageFile
		); 
	if (  readBytes != sizeOfPage){
		free(leftPointer);
		return 1;
	}
	for(index = 0; index < sizeOfPage; ++index, buffer += 1, physicalAddress +=1){
		*physicalAddress = *buffer;
	}
	memoryManager.pages[virtualPageIndex].isModefied = false;
	memoryManager.pages[virtualPageIndex].isPresent = true;
	memoryManager.pages[virtualPageIndex].physicalAddress = pagePhysicalAddress;
	free(leftPointer);
	return 0;
}

 void readByteFromVirtualSpace( VirtualAddress virtualAddress, PhysicalAddress buffer){
	 PhysicalAddress physicalAddress;
	 size_t virtualPageNumber = (size_t)(virtualAddress) >> ( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset);
	 size_t addressShift = ((size_t)(virtualAddress) << ( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset)) >>
			( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset );
	 if( ! memoryManager.pages[virtualPageNumber].isPresent){
		 PhysicalAddress physicalAddressToBeReplaced;
		 findNextPageToBeReplaced();
		 physicalAddressToBeReplaced = memoryManager.pages[memoryManager.replacedPageNumber].physicalAddress;
		 if( memoryManager.pages[memoryManager.replacedPageNumber].isModefied){
			 saveVirtualPageFromPhysicalMemory(memoryManager.replacedPageNumber, physicalAddressToBeReplaced);
		 }
		 memoryManager.pages[memoryManager.replacedPageNumber].isModefied = false;
		 memoryManager.pages[memoryManager.replacedPageNumber].isPresent = false;
		 loadVirtualPageToPhysicalMemory(virtualPageNumber,physicalAddressToBeReplaced);
		 moveReplacedPageNumber();
	 }
	  physicalAddress = memoryManager.pages[virtualPageNumber].physicalAddress + addressShift;
		 *buffer = *physicalAddress;
 }

 void writeByteToVirtualSpace( VirtualAddress virtualAddress, PhysicalAddress buffer){
	 PhysicalAddress physicalAddress;
	 size_t virtualPageNumber = (size_t)(virtualAddress) >> ( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset);
	 size_t addressShift = ((size_t)(virtualAddress) << ( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset)) >>
			( 32 - memoryManager.addressOffset - memoryManager.pageAddressOffset);
	 if( ! memoryManager.pages[virtualPageNumber].isPresent ){
		 PhysicalAddress physicalAddressToBeReplaced;
		 findNextPageToBeReplaced();
		 physicalAddressToBeReplaced = memoryManager.pages[memoryManager.replacedPageNumber].physicalAddress;
		 if( memoryManager.pages[memoryManager.replacedPageNumber].isModefied){
			 saveVirtualPageFromPhysicalMemory(memoryManager.replacedPageNumber, physicalAddressToBeReplaced);
		 }
		 memoryManager.pages[memoryManager.replacedPageNumber].isModefied = false;
		 memoryManager.pages[memoryManager.replacedPageNumber].isPresent = false;
		 loadVirtualPageToPhysicalMemory(virtualPageNumber,physicalAddressToBeReplaced);
		 moveReplacedPageNumber();
	 }
	 physicalAddress = memoryManager.pages[virtualPageNumber].physicalAddress + addressShift;
	 *physicalAddress = *buffer;
	 memoryManager.pages[virtualPageNumber].isModefied = true;
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
	 size_t numberOfActivePages = getNumberOfActivePages( numberOfPages);
	 size_t index;
	 memoryManager.physicalMemoryBegin = (PhysicalAddress) calloc( numberOfActivePages*pageSize , sizeof(char));
	 memoryManager.pages = (Page*) calloc ( numberOfPages , sizeof(Page));
	  if(memoryManager.physicalMemoryBegin == NULL ||
		  memoryManager.pages == NULL ||
		  setOffsets() != 0
		  ){
		 return 1;
	 }
	 eachPageOffset = memoryManager.physicalMemoryBegin;
	 for(index = 0; index < numberOfPages ;	++index){
		 memoryManager.pages[index].isModefied = false;
		 memoryManager.pages[index].isPresent = false;
		 memoryManager.pages[index].physicalAddress = eachPageOffset;
		 eachPageOffset += pageSize;
	 }	
	 for(index = 0 ; index < numberOfActivePages; ++index){
		 if ( loadVirtualPageToPhysicalMemory(index, memoryManager.pages[index].physicalAddress) == 1){
			return 1;
		 }
	 }
	 return 0;
 }

static int _fileInit( size_t n, size_t szPage ){
	 VirtualAddress buffer = (VirtualAddress) calloc( n*szPage, sizeof(char));
	 if( memoryManager.pageFile){
		 fclose(memoryManager.pageFile);
	 }
	 if( fopen_s( &(memoryManager.pageFile), "pageFile", "w+b") == 0 &&
		 buffer != NULL){
			if ( fwrite(buffer, sizeof(char), n*szPage, memoryManager.pageFile) == n*szPage){
			fflush(memoryManager.pageFile);
			fseek( memoryManager.pageFile , 0, SEEK_SET);
			memoryManager.numberOfPages = n;
			memoryManager.sizeOfPage = szPage;
			memoryManager.virtualAddressSize = n*szPage;
			memoryManager.replacedPageNumber = 0;
			free(buffer);
			printf("FinishFileINIT\n");
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