
#pragma once 

/**
 装饰者模式动态地将责任附加到对象上，若要扩展功能，装饰者提供了比继承更有弹性的替代方案。
 动态地给一个对象添加一些额外的职责。就增加功能来说，装饰模式相比生成子类 更为灵活。
 有时我们希望给某个对象而不是整个类添加一些功能。比如有一个手机，允许你为手机添加特性，比如增加挂件、屏幕贴膜等。
 一种灵活的设计方式是， 将手机嵌入到另一对象中，由这个对象完成特性的添加，我们称这个嵌入的对象为装饰。
 这个装饰与它所装饰的组件接口一致，因此它对使用该组件的客户透明。

 如果需要添加新功能，但又不想修改定义好的接口或者抽象类，那么这时候就比较适合装饰模式，
 例如很多图形系统中的组件，就可以使用装饰模式，来让新的组件在继承现在功能的基础上添加新的功能。
 装饰模式一般是针对接口或者抽象类的变化，如果是具体实现类的变化，则要考虑适用哪种模式。
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