#ifndef CEventType_H
#define CEventType_H

#include "Types.h"

class CEventBoundFunction;

class CEventType
{
public:
	CEventType(uint32 uiEventTypeId);

	CEventBoundFunction*					bindEvent(uint32 uiEventId, void(*pFunction)(void*),		void *pArgument = nullptr, int32 iZOrder = 0, uint32 uiEventTypeIndex = 0);
	CEventBoundFunction*					bindEvent(uint32 uiEventId, void(*pFunction)(void*,void*),	void *pArgument = nullptr, int32 iZOrder = 0, uint32 uiEventTypeIndex = 0);
	void									unbindEvent(CEventBoundFunction *pEventBoundFunction);
	bool									triggerEvent(uint32 uiEventId, void *pArgument = nullptr, uint32 uiEventTypeIndex = 0);

private:
	uint32									m_uiEventTypeId;
};

#endif