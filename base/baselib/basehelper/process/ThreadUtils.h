/********************************************************************
	created:	2016/03/16
	created:	16:3:2016   14:37
	filename: 	base\baselib\basehelper\process\ThreadUtils.h
	file path:	base\baselib\basehelper\process
	file base:	ThreadUtils
	file ext:	h
	author:		murisly
	
	purpose:	thread single utils
*********************************************************************/

#pragma once


template <class T>
class CThreadUtils
{
public:
	static inline T& Instance()
	{
		static T s_Instance;
		return s_Instance;
	}
public:
	CThreadUtils(void)
	{
		m_hRequestThread = NULL;
		m_bStarted = FALSE;
		m_hStopEvent = NULL;
	}
	virtual ~CThreadUtils(void)
	{
		if (m_hStopEvent != NULL)
		{
			CloseHandle(m_hStopEvent);
			m_hStopEvent = NULL;
		}
	}

	HRESULT Start()
	{
		m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, FALSE);

		UINT nThreadID = 0;
		m_hRequestThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &nThreadID);

		if (NULL != m_hRequestThread)
		{
			m_bStarted = TRUE;

			return S_OK;
		}

		return E_FAIL;
	}

	HRESULT Stop()
	{
		if (m_bStarted == FALSE)
		{
			return E_UNEXPECTED;
		}

		SetEvent(m_hStopEvent);

		if (WaitForSingleObject(m_hRequestThread, 1000) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hRequestThread, 0);
			CloseHandle(m_hRequestThread);
		}

		if (m_hStopEvent != NULL)
		{
			CloseHandle(m_hStopEvent);
			m_hStopEvent = NULL;
		}

		return S_OK;
	}

	virtual UINT _ThreadProc()
	{
		while ( WaitForSingleObject(m_hStopEvent, 60000) == WAIT_TIMEOUT )
		{
			Sleep(10000);
		}
		return 0;
	}

private:

	static unsigned __stdcall ThreadProc(void* param)
	{
		CThreadUtils* _this = (CThreadUtils*)param;

		_this->_ThreadProc();
		_endthreadex(0);

		return 0;
	}

protected:
	HANDLE m_hStopEvent;
	HANDLE m_hRequestThread;
	BOOL m_bStarted;

};

