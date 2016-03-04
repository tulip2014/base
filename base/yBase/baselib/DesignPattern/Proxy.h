
#pragma once

/**
explicit 修饰的构造函数必须显示调用
*/

//智能指针
template<class T>
class smart_ptr_c
{
public:
	explicit smart_ptr_c(T* p = 0) : pointee(p) {}
	smart_ptr_c(smart_ptr_c<T>& rhs) : pointee(rhs.release()) {}
	~smart_ptr_c() { delete pointee; };
	smart_ptr_c<T>& operator=(smart_ptr_c<T>& rhs)
	{
		if (this != &rhs) reset(rhs.release());
		return *this;
	}
	T& operator*() const { return *pointee; }
	T* operator->() const { return pointee;};
	T* get() const { return pointee; };
	T* release()
	{
		T* oldPointee = pointee;
		pointee = 0;
		return oldPointee;
	}
	void reset(T* p = 0)
	{
		if (pointee != p)
		{
			delete pointee;
			pointee = p;
		}
	}
private:
	T* pointee;
};

/**
smart_ptr<int> a(new int(5));
smart_ptr<int> b(a);

int i = *a;
int j = *b;
*/


//带引用计数的智能指针
template <typename T>
class smart_ptr
{
public:
	smart_ptr(T *p = 0): pointee(p), count(new size_t(1)) { }  //初始的计数值为1
	smart_ptr(const smart_ptr &rhs): pointee(rhs.pointee), count(rhs.count) { ++*count; } //拷贝构造函数，计数加1
	~smart_ptr() { decr_count(); }              //析构，计数减1，减到0时进行垃圾回收，即释放空间
	smart_ptr& operator= (const smart_ptr& rhs) //重载赋值操作符
	{
		//给自身赋值也对，因为如果自身赋值，计数器先减1，再加1，并未发生改变
		++*count;
		decr_count();
		pointee = rhs.pointee;
		count = rhs.count;
		return *this;
	}  
	//重载箭头操作符和解引用操作符，未提供指针的检查
	T *operator->() { return pointee; }
	const T *operator->() const { return pointee; }
	T &operator*() { return *pointee; }
	const T &operator*() const { return *pointee; }
	size_t get_refcount() { return *count; } //获得引用计数器值
private: 
	T *pointee;       //实际指针，被代理  
	size_t *count;    //引用计数器
	void decr_count() //计数器减1
	{
		if(--*count == 0) 
		{
			delete pointee;
			delete count;
		}
	}
};

/**
代理模式 
	
*/

class Subject
{
public:
	Subject(){};
	virtual ~Subject(){};

	virtual void Requset() = 0;
};

class SubRequset : public Subject
{
public:
	SubRequset(){};
	virtual ~SubRequset(){};

	virtual void Requset(){ MessageBox(NULL, _T("SubRequset"), _T("title"), MB_OK); };
};

class Proxy : public Subject
{
private:
	SubRequset* m_subject;
public:
	Proxy(){};
	virtual ~Proxy(){};

	virtual void Requset(){ m_subject->Requset(); }
};