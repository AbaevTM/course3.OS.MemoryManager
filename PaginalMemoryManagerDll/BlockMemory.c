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
		iterator = iterator->next;
	}
	return NULL;
}

static void switchBlocks(MemoryBlock *freeBlock, MemoryBlock * newBlock){
	if(freeBlock->previous != NULL){
		freeBlock->previous->next = newBlock;
	}
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
		iterator = iterator->next;
	}
	return NULL;
}

static void freeBlockUnion(MemoryBlock * block1, MemoryBlock * block2){
	block1->blockSize = block1->blockSize + block2->blockSize;
	block1->isFree = true;
	block1->next = block2->next;
	if(block2->next != NULL){
		block2->next->previous = block1;
	}
	free(block2);
}

// Ёкспортируемые функции
int _malloc (VirtualAddress * ptr, size_t szBlock){
	MemoryBlock * freeBlock;
	MemoryBlock * newBlock;
	VirtualAddress adress;
	//printf("malloc\n");
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
	*ptr = adress;
	return 0;
}


int _free (VirtualAddress ptr){
	MemoryBlock * deletedBlock;
	MemoryBlock *temp;
	//printf("free\n");
	deletedBlock = getBlock(ptr);		
	if(!deletedBlock || deletedBlock->isFree){
		return -1;	 
	}
	if(deletedBlock->next != NULL && deletedBlock->next->isFree){
		freeBlockUnion(deletedBlock, deletedBlock->next);
	}
	if(deletedBlock->previous != NULL && deletedBlock->previous->isFree){
		freeBlockUnion(deletedBlock->previous, deletedBlock);
	}
	else{
		deletedBlock->isFree = true;
	}
	return 0;
}

int _read (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
	MemoryBlock * blockToRead = getBlock(ptr);
	int i = 0;
	//printf("read\n");
	if(pBuffer == NULL || szBuffer == 0){
		return -1;
	}
	if(!blockToRead || blockToRead->blockSize < szBuffer || blockToRead->isFree){
		return -2;
	}
	else {
		char * resultBuffer = (char*) pBuffer;
		char * tempBuffer = (char*)calloc(1, sizeof(char));
		for(i; i < szBuffer; ++i){
			readByteFromVirtualSpace(ptr, tempBuffer);
			*resultBuffer = *tempBuffer;
			++ptr;
			++resultBuffer;
		}
		return 0;
	}
	return 1;
}

int _write (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
	MemoryBlock * writableBlock = getBlock(ptr);
	char* tempBuffer = (char*) pBuffer;
	int i = 0;
	//printf("write\n");
	if(pBuffer == NULL || szBuffer == 0){
		return -1;
	}
	if(!writableBlock || writableBlock->blockSize < szBuffer || writableBlock->isFree){
		return -2;
	} 
	else{
		for(i; i < szBuffer; ++i){
			writeByteToVirtualSpace(ptr, tempBuffer);
			ptr = (char*) (ptr + 1);
			tempBuffer = (char*) (tempBuffer+1);
		}
		return 0;
	}
	return 1;
}