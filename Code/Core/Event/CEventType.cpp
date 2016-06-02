#include "CEventType.h"
#include "CEventManager.h"

CEventType::CEventType(uint32 uiEventTypeId) :
	m_uiEventTypeId(uiEventTypeId)
{
}

// event binding
CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*), void *pBoundArgument, int32 iZOrder, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pBoundArgument, iZOrder);
}

CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*,void*), void *pBoundArgument, int32 iZOrder, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pBoundArgument, iZOrder);
}

void										CEventType::unbindEvent(CEventBoundFunction *pEventBoundFunction)
{
	CEventManager::getInstance()->unbindEvent(pEventBoundFunction);
}

// event triggering
bool										CEventType::triggerEvent(uint32 uiEventId, void *pTriggerArgument, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->triggerEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pTriggerArgument);
}