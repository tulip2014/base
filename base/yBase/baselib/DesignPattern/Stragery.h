
#pragma once

/**
����ģʽ���������㷨���壬�ֱ��װ������������֮���໥�滻����ģʽ���㷨�仯������Ӱ�쵽ʹ���㷨�Ŀͻ�
�����㷨����ʹ����֮������
�ŵ�
1��   �ṩ��һ������̳еķ��������Ҽȱ����˼̳е��ŵ�(��������)�����ȼ̳и����(�㷨����������������չ)��
2��   ���������ʹ�ö�������ת����䣬ʹϵͳ������������չ��
3��   ���ش󲿷�GRASPԭ��ͳ������ԭ�򣬸��ھۡ���ż�ϣ�
4��   ���ڽ��е�Ԫ���ԣ������㷨���ֿ����������ÿ���㷨���е�Ԫ���ԣ�
5��   ����
ȱ��
1��   ��Ϊÿ����������඼�����һ�����࣬���Ի�����ϵͳ��Ҫά�������������
2��   ѡ������㷨��Ҫ�ͻ���������������������ϣ��������ͨ���빤��ģʽ��Ͻ�������⣻
3��   �����ӻ�
*/


class Stragery
{
public:
	Stragery(){};
	virtual ~Stragery(){};

	virtual DWORD Operator() = 0;
};

class StrageryA : public Stragery
{
public:
	StrageryA(){};
	virtual ~StrageryA(){};

	virtual DWORD Operator(){ return 1; }
};

class StrageryB : public Stragery
{
public:
	StrageryB(){};
	virtual ~StrageryB(){};

	virtual DWORD Operator(){ return 1; }
};

class Context
{
private:
	Stragery* m_Stragery;

public:
	Context(Stragery* s){ m_Stragery = s;};
	virtual ~Context(){}

	DWORD Operator(){ return m_Stragery->Operator(); };
};

/**
Context test(new StrageryA);
DWORD dwRet = test.Operator();
*/