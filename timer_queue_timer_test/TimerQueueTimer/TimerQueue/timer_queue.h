/**
How to use TimerQueue.
Thread safe.
cswuyg@gmail.com
2013.6.8
*/

#pragma once

#include <windows.h>
#include <string>
#include <map>


/**
@brief User Timer callback Interface
*/
class ITQTimerCallBack
{
public:
	virtual void TimerCallBack() = 0;
};

/**
@brief Timer Queue Timer
*/
class CTQTimer
{
public:
	CTQTimer();
	~CTQTimer();

	typedef  std::map<UINT, HANDLE> T_mapTimer;

	BOOL SetTimer(ITQTimerCallBack* pCallback, UINT nUserTimerID, DWORD uElapse, BOOL bOnce = FALSE);
	BOOL KillTimer(UINT nUserTimerID);
	BOOL HasTimer(UINT nUserTimerID);

private:
	void static CALLBACK TQTimerCallBack(LPVOID pVoid, BOOLEAN bTimeOrWait);
	void SendHighTimerMsg(UINT nTimerID);

private:
	HANDLE m_hTimerQueue;           //< Timer Queue HANDLE
	T_mapTimer m_mapTimeID;         //< TimerID & Timer HANDLE 
	CRITICAL_SECTION m_section;
};