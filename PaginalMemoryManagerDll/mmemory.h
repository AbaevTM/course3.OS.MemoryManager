/************************************************************************
		������������ ������ �1

	�������� ������������ ������� ������ ��������� ������

 ************************************************************************/

typedef char* VirtualAddres;				// ��� ����������� ����� ����� 

#ifdef C_PLUS_PLUS 
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

/**
 	@func	_malloc	
 	@brief	�������� ���� ������ ������������� �������
	
	@param	[out] ptr		������ �����
	@param	[in]  szBlock	������ �����
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	�������� ������
	@retval	1	����������� ������
 **/
EXTERN  int _malloc (VirtualAddres ptr, size_t szBlock);

/**
 	@func	_free
 	@brief	�������� ����� ������
	
	@param	[in] ptr		������ �����
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
EXTERN  int _free (VirtualAddres ptr);

/**
 	@func	_read
 	@brief	������ ���������� �� ����� ������
	
	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
EXTERN  int _read (VirtualAddres ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_write
 	@brief	������ ���������� � ���� ������
	
	@param	[in] ptr		������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
EXTERN  int _write (VirtualAddres ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_init
 	@brief	������������� ������ ��������� ������
	
	@param	[in] n		���������� �������
	@param	[in] szPage	������ ��������

	� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage
	
	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
EXTERN  int _init (int n, int szPage);