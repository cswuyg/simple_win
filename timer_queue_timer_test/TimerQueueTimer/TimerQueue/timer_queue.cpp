#include "timer_queue.h"
#include "auto_lock.h"

#include <assert.h>

	CTQTimer::CTQTimer()
	{
		m_hTimerQueue = ::CreateTimerQueue();
		::InitializeCriticalSection(&m_section);
	}

	CTQTimer::~CTQTimer()
	{
		{
			CAutoLock lock(&m_section);
			if (m_mapTimeID.size() != 0)
			{
				//you forget ::DeleteTimerQueueTimer ?
				m_mapTimeID.clear();
				assert(false);
			}
			if (m_hTimerQueue != NULL)
			{
				BOOL bDel = ::DeleteTimerQueueEx(m_hTimerQueue, INVALID_HANDLE_VALUE);
				m_hTimerQueue = NULL;
			}
		}
		::DeleteCriticalSection(&m_section);
	}

	BOOL CTQTimer::SetTimer( ITQTimerCallBack* pCallback, UINT nUserTimerID, DWORD uElapse, BOOL bOnce /*= FALSE*/ )
	{
		CAutoLock lock(&m_section);
		if (m_hTimerQueue != NULL)
		{
			HANDLE hTimer;
			DWORD dwPeriod = uElapse;
			if (bOnce)
			{
				dwPeriod = 0;
			}
			BOOL bCreate = ::CreateTimerQueueTimer(&hTimer, m_hTimerQueue, TQTimerCallBack, pCallback, uElapse, dwPeriod, WT_EXECUTEDEFAULT);
			{
				m_mapTimeID.insert(std::make_pair(nUserTimerID, hTimer));
			}
			return bCreate;
		}
		return FALSE;
	}

	BOOL CTQTimer::KillTimer( UINT nUserTimerID )
	{
		CAutoLock lock(&m_section);
		if (m_hTimerQueue != NULL)
		{
			T_mapTimer::iterator it = m_mapTimeID.find(nUserTimerID);
			if (it != m_mapTimeID.end())
			{
				::DeleteTimerQueueTimer(m_hTimerQueue, it->second, INVALID_HANDLE_VALUE);
				m_mapTimeID.erase(it);
				return TRUE;
			}
		}
		assert(false);
		return FALSE;
	}

	BOOL CTQTimer::HasTimer( UINT nUserTimerID )
	{
		CAutoLock lock(&m_section);
		if (m_hTimerQueue != NULL)
		{
			if (m_mapTimeID.find(nUserTimerID) != m_mapTimeID.end())
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	void  CALLBACK CTQTimer::TQTimerCallBack( LPVOID pVoid, BOOLEAN bTimeOrWait )
	{
		if (pVoid != NULL)
		{
			ITQTimerCallBack* pCallback = static_cast<ITQTimerCallBack*>(pVoid);
			pCallback->TimerCallBack();
		}
		else
		{
			assert(false);
		}
	}

