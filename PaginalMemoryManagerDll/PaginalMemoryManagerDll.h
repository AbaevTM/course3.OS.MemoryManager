// ����������� ���� ���� ifdef - ��� ����������� ����� �������� ��������, ���������� ��������� 
// �������� �� ��������� DLL. ��� ����� ������ DLL �������������� � �������������� ������� PAGINALMEMORYMANAGERDLL_EXPORTS,
// � ��������� ������. ���� ������ �� ������ ���� ��������� � �����-���� �������
// ������������ ������ DLL. ��������� ����� ����� ������ ������, ��� �������� ����� �������� ������ ����, ����� 
// ������� PAGINALMEMORYMANAGERDLL_API ��� ��������������� �� DLL, ����� ��� ������ DLL ����� �������,
// ������������ ������ ��������, ��� ����������������.
#ifdef PAGINALMEMORYMANAGERDLL_EXPORTS
#define PAGINALMEMORYMANAGERDLL_API __declspec(dllexport)
#else
#define PAGINALMEMORYMANAGERDLL_API __declspec(dllimport)
#endif

// ���� ����� ������������� �� PaginalMemoryManagerDll.dll
class PAGINALMEMORYMANAGERDLL_API CPaginalMemoryManagerDll {
public:
	CPaginalMemoryManagerDll(void);
	// TODO: �������� ����� ���� ������.
};

extern PAGINALMEMORYMANAGERDLL_API int nPaginalMemoryManagerDll;

PAGINALMEMORYMANAGERDLL_API int fnPaginalMemoryManagerDll(void);
