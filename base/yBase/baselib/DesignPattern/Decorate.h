
#pragma once 

/**
 װ����ģʽ��̬�ؽ����θ��ӵ������ϣ���Ҫ��չ���ܣ�װ�����ṩ�˱ȼ̳и��е��Ե����������
 ��̬�ظ�һ���������һЩ�����ְ�𡣾����ӹ�����˵��װ��ģʽ����������� ��Ϊ��
 ��ʱ����ϣ����ĳ��������������������һЩ���ܡ�������һ���ֻ���������Ϊ�ֻ�������ԣ��������ӹҼ�����Ļ��Ĥ�ȡ�
 һ��������Ʒ�ʽ�ǣ� ���ֻ�Ƕ�뵽��һ�����У����������������Ե���ӣ����ǳ����Ƕ��Ķ���Ϊװ�Ρ�
 ���װ��������װ�ε�����ӿ�һ�£��������ʹ�ø�����Ŀͻ�͸����

 �����Ҫ����¹��ܣ����ֲ����޸Ķ���õĽӿڻ��߳����࣬��ô��ʱ��ͱȽ��ʺ�װ��ģʽ��
 ����ܶ�ͼ��ϵͳ�е�������Ϳ���ʹ��װ��ģʽ�������µ�����ڼ̳����ڹ��ܵĻ���������µĹ��ܡ�
 װ��ģʽһ������Խӿڻ��߳�����ı仯������Ǿ���ʵ����ı仯����Ҫ������������ģʽ��
*/

class Person
{
public:
	Person(){};
	virtual ~Person(){};
	virtual DWORD Show() = 0;
};

class OldDress : public Person
{
public:
	OldDress(){};
	virtual ~OldDress(){};

	virtual DWORD Show() { MessageBox(NULL, _T("OldDress"), _T("title"), MB_OK); return 0; };
};


class Decorate : public Person
{
private:
	Person* m_person;

public:
	Decorate(){};
	Decorate(Person* person) { m_person = person; };
	virtual ~Decorate(){ delete m_person;};

	virtual DWORD Show(){ m_person ->Show(); return 0; };
	void SetDecorate(Person* person) { m_person = person; };
};

class DecorateA : public Decorate
{
public:
	virtual DWORD Show()
	{
		MessageBox(NULL, _T("DecorateA"), _T("title"), MB_OK);
		Decorate::Show();
		return 0;
	};
};

class DecorateB : public Decorate
{
public:
	virtual DWORD Show()
	{
		MessageBox(NULL, _T("DecorateB"), _T("title"), MB_OK);
		Decorate::Show();
		return 0;
	};
};


/**
OldDress* test = new OldDress;
DecorateA* a = new DecorateA;
DecorateB* b = new DecorateB;

a->SetDecorate(test);
b->SetDecorate(a);

b->Show();
*/