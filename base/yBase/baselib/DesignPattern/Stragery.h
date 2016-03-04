
#pragma once

/**
策略模式，定义了算法家族，分别封装起来，让他们之间相互替换，此模式让算法变化，不会影响到使用算法的客户
减少算法类与使用者之间的耦合
优点
1、   提供了一种替代继承的方法，而且既保持了继承的优点(代码重用)，还比继承更灵活(算法独立，可以任意扩展)；
2、   避免程序中使用多重条件转移语句，使系统更灵活，并易于扩展；
3、   遵守大部分GRASP原则和常用设计原则，高内聚、低偶合；
4、   易于进行单元测试，各个算法区分开，可以针对每个算法进行单元测试；
5、   ……
缺点
1、   因为每个具体策略类都会产生一个新类，所以会增加系统需要维护的类的数量；
2、   选择何种算法需要客户端来创建对象，增加了耦合，这里可以通过与工厂模式结合解决该问题；
3、   程序复杂化
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