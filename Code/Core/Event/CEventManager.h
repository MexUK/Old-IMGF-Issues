#ifndef CEventManager_H
#define CEventManager_H

#include "Types.h"
#include "CSingleton.h"
#include "eEventType.h"
#include "CVector2ui32.h"
#include <string>
#include <vector>
#include <unordered_map>

class CEventBoundFunction;

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

	// EVENT_TYPE_GENERAL, 0
	CEventBoundFunction*						bindEvent(uint32 uiEventId, void(*fEventFunction)(void*), int32 iZIndex = 0);		// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventId, void(*fEventFunction)(void*,void*), int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	bool										doesEventExist(uint32 uiEventId);
	uint32										getBoundFunctionCount(uint32 uiEventId);
	bool										triggerEvent(uint32 uiEventId, void *pFunctionArgument = nullptr);

	// EVENT_TYPE_*, *
	CEventBoundFunction*						bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*), int32 iZIndex = 0);		// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*,void*), int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	bool										unbindEvent(CEventBoundFunction *pEventBoundFunction);
	bool										doesEventExist(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId);
	uint32										getBoundFunctionCount(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId);
	bool										triggerEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void *pFunctionArgument = nullptr);

	// CEventManager continued
	void										setLastCursorPosition(CVector2ui32& vecLastCursorPosition) { m_vecLastCursorPosition = vecLastCursorPosition; }
	CVector2ui32&								getLastCursorPosition(void) { return m_vecLastCursorPosition; }

	void										setDefaultActionPrevented(bool bDefaultActionPrevented) { m_bDefaultActionPrevented = bDefaultActionPrevented; }
	bool										isDefaultActionPrevented(void) { return m_bDefaultActionPrevented; }

	void										setEventHogged(bool bEventHogged) { m_bEventHogged = bEventHogged; }
	bool										isEventHogged(void) { return m_bEventHogged; }

private:
	uint32										getInsertionIndexFromZIndex(CEventBoundFunction *pEventBoundFunction);
	CEventBoundFunction*						createEventBoundFunctionObject(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, int32 iZIndex);

private:
	uint8															m_bDefaultActionPrevented	: 1;
	uint8															m_bEventHogged				: 1;
	CVector2ui32													m_vecLastCursorPosition;
	std::unordered_map<
		uint32,
		std::unordered_map<
			uint32,
			std::unordered_map<
				uint32,
				std::vector<CEventBoundFunction*>
			>
		>
	>																m_umapEventFunctions;
};

#endif