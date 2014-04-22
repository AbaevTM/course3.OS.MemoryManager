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
	*ptr = adress;
	return 0;
}


 int _free (VirtualAddress ptr){
	MemoryBlock * deletedBlock;
	MemoryBlock *temp;
	printf("free\n");
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
	/*if(!deletedBlock->next && !deletedBlock->previous){
		deletedBlock->isFree = true;
		return 0;
	}
	//if deleted block is first and next one is not free
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
	// -//- is last and the previous one is not free 
	if(deletedBlock->next == NULL && !deletedBlock->previous->isFree){
		deletedBlock->isFree = true;
		return 0;
	}
	// -//- is last and the previous one is free 
	if(deletedBlock->next == NULL && deletedBlock->previous->isFree){
		temp = deletedBlock->previous;
		temp->blockSize = temp->blockSize + deletedBlock->blockSize;
		temp->next = NULL;
		free(deletedBlock);
		return 0;
	}
	// -//- is not first and next one is free and previous one is not
	if(deletedBlock->next->isFree && !deletedBlock->previous->isFree){
		freeBlockUnion(deletedBlock, deletedBlock->next);
	}
	// -//- is not first and next one is not free and previous one is;
	if(!deletedBlock->next->isFree && deletedBlock->previous->isFree){
		freeBlockUnion(deletedBlock->previous, deletedBlock);
		return 0;
	}
	// -//- is not first and next and previous blocks are free;
	if(deletedBlock->next->isFree && deletedBlock->previous->isFree){
		freeBlockUnion(deletedBlock, deletedBlock->next);
		freeBlockUnion(deletedBlock->previous, deletedBlock);
		return 0;
	}
	if(!deletedBlock->next->isFree && !deletedBlock->previous->isFree){
		deletedBlock->isFree = true;
		return 0;
	}*/
	return 0;
}

 int _read (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
	printf("read\n");
	return 0;
}

 int _write (VirtualAddress ptr, void* pBuffer, size_t szBuffer){
	MemoryBlock * writableBlock = getBlock(ptr);
	printf("write\n");
	if(pBuffer == NULL || szBuffer == 0){
		return -1;
	}
	if(!writableBlock || writableBlock->blockSize < szBuffer){
		return -2;
	} 
	else{
		
		return 0;
	}
	return 1;
}