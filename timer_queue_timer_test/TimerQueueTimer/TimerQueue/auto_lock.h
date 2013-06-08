/**
How to use TimerQueue.
Thread safe.
cswuyg@gmail.com
2013.6.8
*/


#pragma once
/**
@brief critical secion auto lock
*/
class CAutoLock
{
public:
	CAutoLock(CRITICAL_SECTION* pSection)
		: m_pSection(pSection)
	{
		::EnterCriticalSection(m_pSection);
	}
	~CAutoLock()
	{
		::LeaveCriticalSection(m_pSection);
	}
private:
	CRITICAL_SECTION* m_pSection;
};