
#pragma once

/**
explicit ���εĹ��캯��������ʾ����
*/

//����ָ��
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


//�����ü���������ָ��
template <typename T>
class smart_ptr
{
public:
	smart_ptr(T *p = 0): pointee(p), count(new size_t(1)) { }  //��ʼ�ļ���ֵΪ1
	smart_ptr(const smart_ptr &rhs): pointee(rhs.pointee), count(rhs.count) { ++*count; } //�������캯����������1
	~smart_ptr() { decr_count(); }              //������������1������0ʱ�����������գ����ͷſռ�
	smart_ptr& operator= (const smart_ptr& rhs) //���ظ�ֵ������
	{
		//������ֵҲ�ԣ���Ϊ�������ֵ���������ȼ�1���ټ�1����δ�����ı�
		++*count;
		decr_count();
		pointee = rhs.pointee;
		count = rhs.count;
		return *this;
	}  
	//���ؼ�ͷ�������ͽ����ò�������δ�ṩָ��ļ��
	T *operator->() { return pointee; }
	const T *operator->() const { return pointee; }
	T &operator*() { return *pointee; }
	const T &operator*() const { return *pointee; }
	size_t get_refcount() { return *count; } //������ü�����ֵ
private: 
	T *pointee;       //ʵ��ָ�룬������  
	size_t *count;    //���ü�����
	void decr_count() //��������1
	{
		if(--*count == 0) 
		{
			delete pointee;
			delete count;
		}
	}
};

/**
����ģʽ 
	
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