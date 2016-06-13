#ifndef CEventManager_H
#define CEventManager_H

#include "Types.h"
#include "CSingleton.h"
#include "eEventType.h"
#include "CVector2i32.h"
#include "CInputEventCallbacks.h"
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
	}

	// EVENT_TYPE_GENERAL, 0
	CEventBoundFunction*						bindEvent(uint32 uiEventId, void(*fEventFunction)(void*),		void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventId, void(*fEventFunction)(void*,void*), void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventId, CInputEventCallbacks *pObject,		void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	bool										doesEventExist(uint32 uiEventId);
	uint32										getBoundFunctionCount(uint32 uiEventId);
	bool										triggerEvent(uint32 uiEventId, void *pTriggerArgument = nullptr);

	// EVENT_TYPE_*, *
	CEventBoundFunction*						bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*),		void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*,void*),	void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	CEventBoundFunction*						bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, CInputEventCallbacks *pObject,		void *pBoundArgument = nullptr, int32 iZIndex = 0);	// negative z-index's trigger before positive ones
	bool										unbindEvent(CEventBoundFunction *pEventBoundFunction);
	bool										doesEventExist(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId);
	uint32										getBoundFunctionCount(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId);
	bool										triggerEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void *pTriggerArgument = nullptr);

	// CEventManager continued
	void										setLatestCursorPosition(CVector2i32& vecLatestCursorPosition) { m_vecLatestCursorPosition = vecLatestCursorPosition; }
	CVector2i32&								getLatestCursorPosition(void) { return m_vecLatestCursorPosition; }

	void										setLatestScreenCursorPosition(CVector2i32& vecLatestScreenCursorPosition) { m_vecLatestScreenCursorPosition = vecLatestScreenCursorPosition; }
	CVector2i32&								getLatestScreenCursorPosition(void) { return m_vecLatestScreenCursorPosition; }

	void										setPreviousCursorPosition(CVector2i32& vecPreviousCursorPosition) { m_vecPreviousCursorPosition = vecPreviousCursorPosition; }
	CVector2i32&								getPreviousCursorPosition(void) { return m_vecPreviousCursorPosition; }

	void										setPreviousScreenCursorPosition(CVector2i32& vecPreviousScreenCursorPosition) { m_vecPreviousScreenCursorPosition = vecPreviousScreenCursorPosition; }
	CVector2i32&								getPreviousScreenCursorPosition(void) { return m_vecPreviousScreenCursorPosition; }

	void										setScreenCursorMoveDifference(CVector2i32& vecScreenCursorMoveDifference) { m_vecScreenCursorMoveDifference = vecScreenCursorMoveDifference; }
	CVector2i32&								getScreenCursorMoveDifference(void) { return m_vecScreenCursorMoveDifference; }

	void										setDefaultActionPrevented(bool bDefaultActionPrevented) { m_bDefaultActionPrevented = bDefaultActionPrevented; }
	bool										isDefaultActionPrevented(void) { return m_bDefaultActionPrevented; }

	void										setEventHogged(bool bEventHogged) { m_bEventHogged = bEventHogged; }
	bool										isEventHogged(void) { return m_bEventHogged; }

private:
	CEventBoundFunction*						createEventBoundFunctionObject(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void *pBoundArgument, int32 iZIndex);

	uint32										getInsertionIndexForEventBoundFunction(CEventBoundFunction *pEventBoundFunction);

private:
	uint8															m_bDefaultActionPrevented	: 1;
	uint8															m_bEventHogged				: 1;
	CVector2i32														m_vecLatestCursorPosition;
	CVector2i32														m_vecLatestScreenCursorPosition;
	CVector2i32														m_vecPreviousCursorPosition;
	CVector2i32														m_vecPreviousScreenCursorPosition;
	CVector2i32														m_vecScreenCursorMoveDifference;
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