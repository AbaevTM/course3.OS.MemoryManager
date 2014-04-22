
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

	FILE * pageFile; // ������� � �����, �������� ������.

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
		| ������ �� ����� ������ � ����������� ������������ �������� szBuffer � ������� � ptr �� ������� pBuffer
		|
		|
		| ���������� ����, �������� ����������� ����������� �����
		| ��������� �� ����� �� ������� ���������� ������
		|
		| ������� ������2 �� ����. � ��� ���� �������, ������� ����� ��������� c ptr �� ptr + szBuffer
		| readByte(...) � ������2 � ��������� ��� �������� �� ������� ������
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
				| ��������� ����� ��������
				| ���� ��� ���� � ������ ����� ����� �� ���������������� ������ ������ ����
				| � ���� � ���, �� � ��������� ������ �������� �� ����, �������� ������ � ����� �� ��.
				|
				|
*/