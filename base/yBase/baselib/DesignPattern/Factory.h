
#pragma once

/**
简单工厂模式（Simple Factory Pattern）属于类的创新型模式，又叫静态工厂方法模式
*/

class Operator
{
private:
	double dOpeA;
	double dOpeB;

public:
	Operator() {}
	virtual ~Operator() {}
	
	void SetOpeA(double dTmp) { dOpeA = dTmp; }
	double GetOpeA() { return dOpeA; }

	void SetOpeB(double dTmp) { dOpeB = dTmp; }
	double GetOpeB() { return dOpeB; }

	virtual double GetResult() { return 0; }
};

class OperatorAdd : public Operator
{
public:
	virtual double GetResult() { return GetOpeA() + GetOpeB(); }
};

class OperatorSub : public Operator
{
public:
	virtual double GetResult() { return GetOpeA() - GetOpeB(); }
};

class OperatorMul : public Operator
{
public:
	virtual double GetResult() { return GetOpeA() * GetOpeB(); }
};

class OperatorDiv : public Operator
{
public:
	virtual double GetResult()
	{
		double dTmp = GetOpeB();
		if (0 == dTmp)
		{
			return 0;
		}

		return GetOpeA() / GetOpeB();
	}
};

class FactoryOperator
{
public:
	static Operator* CreateOperator(std::wstring strOperatorName)
	{
		Operator* oper = NULL;
		if (_T("+") == strOperatorName)
		{
			oper = new OperatorAdd();
		}
		else if (_T("-") == strOperatorName)
		{
			oper = new OperatorSub();
		}
		else if (_T("*") == strOperatorName)
		{
			oper = new OperatorMul();
		}
		else if (_T("/") == strOperatorName)
		{
			oper = new OperatorDiv();
		}
		return oper;
	}

};

/***********************************************************************
	Operator* test = FactoryOperator::CreateOperator(_T("+"));
	test->SetOpeA(1);
	test->SetOpeB(2);	
	double a = test->GetResult();
***********************************************************************/



class FactoryMall
{
private:
	double dTotal;

public:
	double GetTotal() { return dTotal; }
	void SetTotal( double dSet) { dTotal = dSet; }
	double AddTotal( double dAdd ) { dTotal += dAdd; }
};

class Discount : public FactoryMall
{
public:
	Discount( double dTmpDisCount = 10.0, double dTmpNumber = 0, double dTmpPrice = 0 ) { dDisConut = dTmpDisCount; dNumber = dTmpNumber, dPrice = dTmpPrice; }
	double AddDiscount()
	{
		AddTotal( 0.1 * dDisConut * dNumber * dPrice );
		return GetTotal();
	}
private:
	double dDisConut;
	double dNumber;
	double dPrice;
};

class FullCut : public FactoryMall
{
public:
	FullCut ( double dTmpBasePrice = 0, double dTmpCutPrice = 0 ){ dBasePrice = dTmpBasePrice, dCutPrice = dTmpCutPrice; }
	double AddFullCut()
	{
		AddTotal( dBasePrice - dCutPrice );
		return GetTotal();
	}
private:
	double dBasePrice;
	double dCutPrice;
};

class CashFactory
{
private:
//	FactoryMall* mall;
public:
	static FactoryMall* CreateCashFactory(std::wstring strType)
	{
		FactoryMall* mall = NULL;
		if (_T("discount") == strType)
		{
			mall = new Discount();
		}
	}
};