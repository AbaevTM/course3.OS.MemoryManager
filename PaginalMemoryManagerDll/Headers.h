
#include <stdio.h>

typedef char* VirtualAdress;

typedef char* PhysicalAdress;

typedef  unsigned short  Bool;

#define true 1;

# define false 0;

typedef struct {

	Bool isPresent;
	
	Bool isModefied;
	
	PhysicalAdress physicalAddress;

} Page;

typedef struct MemoryBlockS MemoryBlock;

struct MemoryBlockS{

	MemoryBlock * previous;

	MemoryBlock * next;

	Bool isFree;

	VirtualAdress blockAdress;

	size_t blockSize;
};

typedef struct {

	FILE * pageFile; // ������� � �����, �������� ������.

	Page * pages;

	PhysicalAdress physicalMemoryBegin;

	MemoryBlock firstMemoryBlock;

	size_t numberOfPages;

	size_t sizeOfPage;

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