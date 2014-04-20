/************************************************************************
		Лабораторная работа №1

	Описание интерфейсных функций модели менеджера памяти

 ************************************************************************/

typedef char* VirtualAddres;				// Тип описывающий адрес блока 

#ifdef C_PLUS_PLUS 
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

/**
 	@func	_malloc	
 	@brief	Выделяет блок памяти определенного размера
	
	@param	[out] ptr		адресс блока
	@param	[in]  szBlock	размер блока
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	нехватка памяти
	@retval	1	неизвестная ошибка
 **/
EXTERN  int _malloc (VirtualAddres ptr, size_t szBlock);

/**
 	@func	_free
 	@brief	Удаление блока памяти
	
	@param	[in] ptr		адресс блока
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
EXTERN  int _free (VirtualAddres ptr);

/**
 	@func	_read
 	@brief	Чтение информации из блока памяти
	
	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
EXTERN  int _read (VirtualAddres ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_write
 	@brief	Запись информации в блок памяти
	
	@param	[in] ptr		адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
EXTERN  int _write (VirtualAddres ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_init
 	@brief	Инициализация модели менеджера памяти
	
	@param	[in] n		количество страниц
	@param	[in] szPage	размер страницы

	В варианте 1 и 2 общий объем памяти расчитывается как n*szPage
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
EXTERN  int _init (int n, int szPage);