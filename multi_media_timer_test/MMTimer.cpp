#include "stdafx.h"
#include "MMTimer.h"

#include <assert.h>


CMMTimer::CMMTimer()
{
	RtlZeroMemory(&m_timeCaps, sizeof(TIMECAPS));
	m_retTimeGetDevCaps = ::timeGetDevCaps(&m_timeCaps, sizeof(TIMECAPS));
	if (m_retTimeGetDevCaps == MMSYSERR_NOERROR)
	{
		m_retTimeGetDevCaps = ::timeBeginPeriod(m_timeCaps.wPeriodMin);
	}
	::InitializeCriticalSection(&m_section);
}

CMMTimer::~CMMTimer()
{
	{
		CAutoLock lock(&m_section);
		T_mapTimer::iterator it = m_mapTimeID.begin();
		T_mapTimer::iterator itEnd = m_mapTimeID.end();
		for (; it != itEnd; ++it)
		{
			assert(false);
			::timeKillEvent(it->first);
		}
	}

	if (m_retTimeGetDevCaps == MMSYSERR_NOERROR)
	{
		m_retTimeGetDevCaps = ::timeEndPeriod(m_timeCaps.wPeriodMin);
	}
	::DeleteCriticalSection(&m_section);
}

void CALLBACK CMMTimer::HighTimerCallBack(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	if (dwUser == NULL)
	{
		return;
	}
	CMMTimer* pSelf = (CMMTimer*)dwUser;
	pSelf->SendHighTimerMsg(uTimerID);
}

void CMMTimer::KillTimer(UINT nUserTimerID)
{
	CAutoLock lock(&m_section);
	T_mapTimer::iterator it = m_mapTimeID.begin();
	T_mapTimer::iterator itEnd = m_mapTimeID.end();
	for (; it != itEnd; ++it)
	{
		if (it->second.nUserTimerID == nUserTimerID)
		{
			::timeKillEvent(it->first);
			m_mapTimeID.erase(it);
			return;
		}
	}
	assert(false);
}

int CMMTimer::SetTimer(ITimerCallBack* pCallback, UINT nUserTimerID, DWORD uElapse, bool bOnce /*= false*/)
{
	if (pCallback == NULL || uElapse <= 0)
	{
		return 0;
	}
	unsigned int fuEvent = TIME_PERIODIC | TIME_CALLBACK_FUNCTION;
	if (bOnce)
	{
		fuEvent = TIME_ONESHOT | TIME_CALLBACK_FUNCTION;
	}
	MMRESULT timeID = ::timeSetEvent(uElapse, 0, (LPTIMECALLBACK)HighTimerCallBack, (DWORD_PTR)this, fuEvent);
	if (timeID == NULL)
	{
		return 0;
	}

	CAutoLock lock(&m_section);
	UserTimerInfo stUserTimerInfo;
	stUserTimerInfo.pCallBack = pCallback;
	stUserTimerInfo.nUserTimerID = nUserTimerID;
	m_mapTimeID.insert(std::make_pair(timeID, stUserTimerInfo));
	return timeID;
}

void CMMTimer::SendHighTimerMsg( UINT nTimerID )
{
	ITimerCallBack* pCallBack = NULL;
	DWORD nUserTimerID = 0;
	{
		CAutoLock lock(&m_section);
		T_mapTimer::iterator itFind = m_mapTimeID.find(nTimerID);
		if (itFind == m_mapTimeID.end())
		{
			return;
		}
		pCallBack = itFind->second.pCallBack;
		nUserTimerID = itFind->second.nUserTimerID;
	}
	pCallBack->TimerCallBack();

}

bool CMMTimer::HasTimer( UINT nUserTimerID )
{
	CAutoLock lock(&m_section);
	T_mapTimer::iterator it = m_mapTimeID.begin();
	T_mapTimer::iterator itEnd = m_mapTimeID.end();
	for (; it != itEnd; ++it)
	{
		if (it->second.nUserTimerID == nUserTimerID)
		{
			return true;
		}
	}
	return false;
}


