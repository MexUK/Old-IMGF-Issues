#include "CEventType.h"
#include "CEventManager.h"

CEventType::CEventType(uint32 uiEventTypeId) :
	m_uiEventTypeId(uiEventTypeId)
{
}

CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*), void *pArgument, int32 iZOrder, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pArgument, iZOrder);
}

CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*,void*), void *pArgument, int32 iZOrder, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pArgument, iZOrder);
}

void										CEventType::unbindEvent(CEventBoundFunction *pEventBoundFunction)
{
	CEventManager::getInstance()->unbindEvent(pEventBoundFunction);
}

bool										CEventType::triggerEvent(uint32 uiEventId, void *pArgument, uint32 uiEventTypeIndex)
{
	return CEventManager::getInstance()->triggerEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pArgument);
}