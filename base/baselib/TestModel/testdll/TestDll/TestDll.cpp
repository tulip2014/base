// TestDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "TestDll.h"



// ���ǵ���������һ��ʾ��
TESTDLL_API int nTestDll = 2;

// ���ǵ���������һ��ʾ����
TESTDLL_API int fnTestDll(int a , int b)
{
	return a + b;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� TestDll.h
CTestDll::CTestDll()
{
	return;
}

