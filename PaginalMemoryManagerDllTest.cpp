// PaginalMemoryManagerDllTest.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "mmemory.h"
#include "stdlib.h"
#include <conio.h>




VirtualAddress d = (VirtualAddress) malloc(sizeof(char));
VirtualAddress e = (VirtualAddress) malloc(sizeof(char));

/*void alocateMemory(int *sizes){
	int * temp = sizes;
	_malloc(&a, *temp++);
	_malloc(&b, *temp++);
	_malloc(&c, *temp++);
	_malloc(&d, *temp++);
	_malloc(&e, *temp);
}*/

/*void writeToMemory(){
	char* buffer = "hello";
	size_t size = sizeof(buffer) + 1;
	_write(c, buffer, size);
}*/

/*void readFromMemory(){
	size_t size = 6;
	char *buffer = (char*) calloc(size, sizeof(char));
	_read(c, buffer, size);
	printf("%s", buffer);
}*/

void case1(){
	VirtualAddress a = (VirtualAddress) malloc(sizeof(char));
	char* buffer = "hello";
	size_t size = 6;
	char *readBuffer = (char*) calloc(size, sizeof(char));
	_init(3, 1024);
	_malloc(&a, 256);
	printf("%s\n", buffer);
	_write(a, buffer, size);
	_read(a, readBuffer, size);
	printf("%s\n", readBuffer);
}

void case2(){
	if(_init(3, 3) == -1){
		printf("%s", "Wrong parametrs\n");
	}
	if(!_init(3, 1024)) {
		printf("%s", "Second init. OK\n");
	}
}

void case3(){
	VirtualAddress b = (VirtualAddress) malloc(sizeof(char)); 
	_init(3, 128);
	if(_malloc(&b, 1024) == -2){
		printf("%s", "Not enough memory\n");
	}
}

void case4(){
	_init(3, 1024);
	char* buffer = "hello";
	size_t size = sizeof(buffer) + 1;
	VirtualAddress c = (VirtualAddress) malloc(sizeof(char));
	char *readBuffer = (char*) calloc(size, sizeof(char));
	_malloc(&c, 512);
	_write(c, buffer, size);
	_free(c);
	if(_read(c, readBuffer, size) == -2){
		printf("%s", "Ok\n");
	}
}

void case5(){
	int pageSize = 1024;
	int count = 3;
	int memorySize = pageSize * count;
	_init(count, pageSize);
	
	VirtualAddress d = (VirtualAddress) malloc(sizeof(char));
	_malloc(&d, memorySize);
	char *writeBuffer = (char*) calloc(memorySize, sizeof(char));
	_write(d, writeBuffer, memorySize);
	
	if(!_read(d, writeBuffer, memorySize)){
		printf("%s", "OK");
	}


}


int _tmain(int argc, _TCHAR* argv[])
{
	int command = 1;
	while(command){
		scanf_s("%d", &command);
		switch (command)
		{
		case 0: {
					command = 0;

				}
		case 1: {
					printf("%s", "Case 1\n");
					case1();
					break;
				}
		case 2: {
					printf("%s", "Case 2\n");
					case2();
					break;
				}
		case 3: {
					printf("%s", "Case 3\n");
					case3();
					break;
				}
		case 4: {
					printf("%s", "Case 4\n");
					case4();
					break;
				}
		case 5:{
					printf("%s", "Case 5\n");
					case5();
					break;
			   }
		default:
			break;
		}
	}
	/*VirtualAddress a = (VirtualAddress) malloc(sizeof(char));
	VirtualAddress b = (VirtualAddress) malloc(sizeof(char));
	VirtualAddress c = (VirtualAddress) malloc(sizeof(char));
	VirtualAddress d = (VirtualAddress) malloc(sizeof(char));
	_init(2, 1024);

	_malloc(&a, 512);
	_malloc(&b, 256);
	_malloc(&c, 128);
	_malloc(&d, 64);
	_free(c);
	_free(b);
	printf("%d\n", 11);*/
	_getch();
	return 0;
}

