
#pragma once

class BaseOpreator
{
public:
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	virtual int Add(int a, int b) = 0;
	/**
    @brief: �ƶ���굽�ƶ�λ��
	@param[in]: ����ƶ���λ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long MouseMove(POINT point) = 0;
	/**
    @brief: �����
	@param[in]: �����λ��, "L" ��ʾ����� "M" ��ʾ�м䣬 "R" ��ʾ�һ�
	@param[in]: ����Ĵ���
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long MouseClick(LPCTSTR lpType, int iTimes) = 0;
	/**
    @brief: ������λ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
	@param[out]��point�����λ��
    **/
	virtual long GetMousePos(POINT& point) = 0;
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long KeyboardDown(int iType) = 0;
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long KeyboardUp(int iTypet) = 0;
	/**
    @brief: ͨ��classname��ĳ�����ڼ���
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long SetWindowFocus(LPCTSTR lpClassName, LPCTSTR lpTitleName) = 0;
	/**
    @brief: ͨ��classname��ĳ��������ʾ����
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[in]: ��ʾ����(SW_SHOWNORMAL 1������ʾ, 2��С����3���)
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
    **/
	virtual long ShowWindows(LPCTSTR lpClassName, LPCTSTR lpTitleName, int iType) = 0;
	/**
    @brief: ͨ���������ó�������
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]��RECT��ʾ���ڵ�λ��
    **/
	virtual long GetWindowPos(LPCTSTR lpClassName, LPCTSTR lpTitleName, RECT& rect) = 0;
	/**
    @brief: �жϴ����Ƿ����
	@param[in]: classname(Ϊ��ʱ�� L"")
	@param[in]: titlename(Ϊ��ʱ�� L"")
	@param[out]��0��ʾ���ڣ�������ʾ������
    **/
	virtual long IsWindowExit(LPCTSTR lpClassName, LPCTSTR lpTitleName) = 0;
	/**
    @brief: ȡ��Ļĳ�����ɫ
	@param[in]: point ��ʾ����λ��
	@param[out]���ɹ�Ϊ0��ʧ��Ϊ��0
	@param[out]��r(��ɫRGB��ֵ)
	@param[out]��g
	@param[out]��b
    **/
	virtual long GetPiexlColor(POINT pos, int& r, int& g, int& b) = 0;
};

