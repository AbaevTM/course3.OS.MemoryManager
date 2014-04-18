#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

typedef char* VirtualAdress;

typedef char* PhysiclAdress;

typedef struct MemoryManagerStruct{

	FILE * pageFile; // Создаем в ините, забиваем нулями.

	Page pages [];

} MemoryManager; 

typedef struct PageStruct{

	bool isPresent;
	
	bool isModefied;
	
	PhysiclAdress physiclAddress;

} Page;

typedef struct MemoryBlockStruct{

	MemoryBlock * previous;

	MemoryBlock * next;

	bool isFree;

	VirtualAdress blockAdress;

	size_t blockSize;

} MemoryBlock;

/*

int _read (VA ptr, void* pBuffer, size_t szBuffer);
		|
		| Читает из блока памяти в виртуальном пространстве размером szBuffer с началом в ptr во внешний pBuffer
		|
		|
		| Обнаружить блок, которому принадлежит виртуальный адрес
		| Проверить на выход за пределы выделенной памяти
		|
		| Создать буффер2 на байт. И для всех адресов, которые нужно прочитать c ptr до ptr + szBuffer
		| readByte(...) в буффер2 и копируешь это значение во внешний буффер
		|
		|	char * outsideBuffer = (char *) pBuffer;
			while(...){
				readByteFromVirtualSpace ( vaPointer, insideBuffer);
				*outsideBuffer = *insideBuffer;
				someBuffer = (char *) ((size_t) someBuffer + sizeof(char));
				vaPointer = (char *) ((size_t) vaPointer + sizeof(char));
			}
		|
		|
		|
	readByteFromVirtualSpace ( VirtualAdress pointer, void* buffer)
				|
				|
				| Определяю номер страницы
				| Если она есть в памяти читаю отуда по преобразованному адресу нужный байт
				| А если её нет, то я сбрасываю первую страницу на диск, загружаю нужную и читаю из неё.
				|
				|
*/