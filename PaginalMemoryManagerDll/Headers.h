#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>

typedef char* VirtualAdress;

typedef char* PhysiclAdress;

typedef struct MemoryManagerStruct{

	FILE * pageFile; // ������� � �����, �������� ������.

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
				someBuffer = (char *) ((size_t) someBuffer + sizeof(char));
				vaPointer = (char *) ((size_t) vaPointer + sizeof(char));
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