
#include <stdio.h>

typedef char* VirtualAddress;

typedef char* PhysicalAddress;

typedef  unsigned short  Bool;

#define true 1;

# define false 0;

typedef struct {

	Bool isPresent;
	
	Bool isModefied;
	
	PhysicalAddress physicalAddress;

} Page;

typedef struct MemoryBlockS MemoryBlock;

struct MemoryBlockS{

	MemoryBlock * previous;

	MemoryBlock * next;

	Bool isFree;

	VirtualAddress blockAdress;

	size_t blockSize;
};

typedef struct {

	FILE * pageFile; // Создаем в ините, забиваем нулями.

	Page * pages;

	PhysicalAddress physicalMemoryBegin;

	MemoryBlock * firstMemoryBlock;

	///// PAGINAL MEMORY PROPERTIES

	size_t numberOfPages;

	size_t sizeOfPage;

	unsigned long virtualAddressSize;

	unsigned short addressOffset;

	unsigned short pageAddressOffset;

	/*

	numberOfPages = 2
	sizeOfPage = 2
	Addresses:
	0|0
	0|1
	1|0
	1|1

	1 sign + addressOffset + addressBytes  = 32
	01000000 00000000 00000000 00000000


	00000000 00000000 00000000 00000010
	^								^
	|-----------------addressOffset|
									^^
									||pageaAddressOffset

	*/


} MemoryManager; 

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
				someBuffer = (char *) (someBuffer + 1);
				vaPointer = (char *) (vaPointer + 1);
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