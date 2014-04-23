#include "mmemory.h"
#include <stdlib.h>
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

// Ёкспортируемые функции
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
	//MemoryBlock * deletedBlock;
	//MemoryBlock *temp;
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