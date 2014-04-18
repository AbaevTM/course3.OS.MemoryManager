// Приведенный ниже блок ifdef - это стандартный метод создания макросов, упрощающий процедуру 
// экспорта из библиотек DLL. Все файлы данной DLL скомпилированы с использованием символа PAGINALMEMORYMANAGERDLL_EXPORTS,
// в командной строке. Этот символ не должен быть определен в каком-либо проекте
// использующем данную DLL. Благодаря этому любой другой проект, чьи исходные файлы включают данный файл, видит 
// функции PAGINALMEMORYMANAGERDLL_API как импортированные из DLL, тогда как данная DLL видит символы,
// определяемые данным макросом, как экспортированные.
#ifdef PAGINALMEMORYMANAGERDLL_EXPORTS
#define PAGINALMEMORYMANAGERDLL_API __declspec(dllexport)
#else
#define PAGINALMEMORYMANAGERDLL_API __declspec(dllimport)
#endif

// Этот класс экспортирован из PaginalMemoryManagerDll.dll
class PAGINALMEMORYMANAGERDLL_API CPaginalMemoryManagerDll {
public:
	CPaginalMemoryManagerDll(void);
	// TODO: Добавьте здесь свои методы.
};

extern PAGINALMEMORYMANAGERDLL_API int nPaginalMemoryManagerDll;

PAGINALMEMORYMANAGERDLL_API int fnPaginalMemoryManagerDll(void);
