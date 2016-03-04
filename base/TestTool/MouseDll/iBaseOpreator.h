
#pragma once
#include "baseoperator.h"

class CBaseOpreator : public BaseOpreator
{
public:
	virtual void AddRef();
	virtual void Release();

	virtual int Add(int a, int b);

	/**
    @brief: �ƶ���굽�ƶ�λ��
	@param[in]: ����ƶ���λ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long MouseMove(POINT point);
	/**
    @brief: �����
	@param[in]: �����λ��, "L" ��ʾ����� "M" ��ʾ�м䣬 "R" ��ʾ�һ�
	@param[in]: ����Ĵ���
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long MouseClick(LPCTSTR lpType, int iTimes);
	/**
    @brief: ������λ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
	@param[out]��x
	@param[out]��point�����λ��
	**/
	virtual long GetMousePos(POINT& point);
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long KeyboardDown(int iType);
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long KeyboardUp(int iTypet);
	/**
    @brief: ͨ��classname��ĳ�����ڼ���
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long SetWindowFocus(LPCTSTR lpClassName, LPCTSTR lpTitleName);
	/**
    @brief: ͨ��classname��ĳ��������ʾ����
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[in]: ��ʾ����(SW_SHOWNORMAL 1������ʾ, 2��С����3���)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long ShowWindows(LPCTSTR lpClassName, LPCTSTR lpTitleName, int iType);
	/**
    @brief: ͨ���������ó�������
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]��RECT��ʾ���ڵ�λ��
	**/
	virtual long GetWindowPos(LPCTSTR lpClassName, LPCTSTR lpTitleName, RECT& rect);
	/**
    @brief: �жϴ����Ƿ����
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]��0��ʾ���ڣ�������ʾ������
    **/
	virtual long IsWindowExit(LPCTSTR lpClassName, LPCTSTR lpTitleName);
	/**
    @brief: ȡ��Ļĳ�����ɫ
	@param[in]: point��ʾ��Ļλ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
	@param[out]��r(��ɫRGB��ֵ)
	@param[out]��g
	@param[out]��b
    **/
	virtual long GetPiexlColor(POINT pos, int& r, int& g, int& b);

};
