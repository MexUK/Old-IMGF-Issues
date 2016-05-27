#ifndef CEventManager_H
#define CEventManager_H

#include "Types.h"
#include "CSingleton.h"
#include "CVector2ui32.h"
#include <string>
#include <unordered_map>

class CEventManager : public CSingleton<CEventManager>
{
public:
	CEventManager(void) :
		m_bDefaultActionPrevented(false),
		m_bEventHogged(false)
	{
		m_vecLastCursorPosition.m_x = 0;
		m_vecLastCursorPosition.m_y = 0;
	}

	void										bindEvent(std::string strEventName, void(*fEventFunction)(void*));
	uint32										getBoundFunctionCount(std::string strEventName);
	bool										triggerEvent(std::string strEventName, void *pData = nullptr);
	
	void										setLastCursorPosition(CVector2ui32& vecLastCursorPosition) { m_vecLastCursorPosition = vecLastCursorPosition; }
	CVector2ui32&								getLastCursorPosition(void) { return m_vecLastCursorPosition; }

	void										setDefaultActionPrevented(bool bDefaultActionPrevented) { m_bDefaultActionPrevented = bDefaultActionPrevented; }
	bool										isDefaultActionPrevented(void) { return m_bDefaultActionPrevented; }

	void										setEventHogged(bool bEventHogged) { m_bEventHogged = bEventHogged; }
	bool										isEventHogged(void) { return m_bEventHogged; }

private:
	CVector2ui32													m_vecLastCursorPosition;
	std::unordered_map<std::string, std::vector<void(*)(void*)>>	m_umapBoundFunctions;
	uint8															m_bDefaultActionPrevented		: 1;
	uint8															m_bEventHogged					: 1;
};

#endif