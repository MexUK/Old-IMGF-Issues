#include "CEventType.h"
#include "CEventManager.h"

CEventType::CEventType(uint32 uiEventTypeId) :
	m_uiEventTypeId(uiEventTypeId)
{
}

CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*), void *pArgument, uint32 uiEventTypeIndex, int32 iZOrder)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pArgument, iZOrder);
}

CEventBoundFunction*						CEventType::bindEvent(uint32 uiEventId, void(*pFunction)(void*,void*), void *pArgument, uint32 uiEventTypeIndex, int32 iZOrder)
{
	return CEventManager::getInstance()->bindEvent(m_uiEventTypeId, uiEventTypeIndex, uiEventId, pFunction, pArgument, iZOrder);
}

void										CEventType::unbindEvent(CEventBoundFunction *pEventBoundFunction)
{
	CEventManager::getInstance()->unbindEvent(pEventBoundFunction);
}