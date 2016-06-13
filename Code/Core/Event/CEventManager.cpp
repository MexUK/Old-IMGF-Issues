#include "CEventManager.h"
#include "CEventBoundFunction.h"

using namespace std;

// EVENT_TYPE_GENERAL, 0
CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventId, void(*fEventFunction)(void*), void *pBoundArgument, int32 iZIndex)
{
	return bindEvent(EVENT_TYPE_GENERAL, 0, uiEventId, fEventFunction, pBoundArgument, iZIndex);
}

CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventId, void(*fEventFunction)(void*, void*), void *pBoundArgument, int32 iZIndex)
{
	return bindEvent(EVENT_TYPE_GENERAL, 0, uiEventId, fEventFunction, pBoundArgument, iZIndex);
}

CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventId, CInputEventCallbacks *pObject, void *pBoundArgument, int32 iZIndex)
{
	return bindEvent(EVENT_TYPE_GENERAL, 0, uiEventId, pObject, pBoundArgument, iZIndex);
}

bool										CEventManager::doesEventExist(uint32 uiEventId)
{
	return doesEventExist(EVENT_TYPE_GENERAL, 0, uiEventId);
}

uint32										CEventManager::getBoundFunctionCount(uint32 uiEventId)
{
	return getBoundFunctionCount(EVENT_TYPE_GENERAL, 0, uiEventId);
}

bool										CEventManager::triggerEvent(uint32 uiEventId, void *pTriggerArgument)
{
	return triggerEvent(EVENT_TYPE_GENERAL, 0, uiEventId, pTriggerArgument);
}

// EVENT_TYPE_*, *
CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*), void *pBoundArgument, int32 iZIndex)
{
	CEventBoundFunction *pEventBoundFunction = createEventBoundFunctionObject(uiEventTypeId, uiEventTypeIndex, uiEventId, pBoundArgument, iZIndex);
	pEventBoundFunction->setEventFunctionType(EVENT_FUNCTION_TYPE_1_ARG);
	pEventBoundFunction->setFunction(fEventFunction);
	m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].insert(m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].begin() + getInsertionIndexForEventBoundFunction(pEventBoundFunction), pEventBoundFunction);
	return pEventBoundFunction;
}

CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void(*fEventFunction)(void*,void*), void *pBoundArgument, int32 iZIndex)
{
	CEventBoundFunction *pEventBoundFunction = createEventBoundFunctionObject(uiEventTypeId, uiEventTypeIndex, uiEventId, pBoundArgument, iZIndex);
	pEventBoundFunction->setEventFunctionType(EVENT_FUNCTION_TYPE_2_ARGS);
	pEventBoundFunction->setFunction(fEventFunction);
	m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].insert(m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].begin() + getInsertionIndexForEventBoundFunction(pEventBoundFunction), pEventBoundFunction);
	return pEventBoundFunction;
}

CEventBoundFunction*						CEventManager::bindEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, CInputEventCallbacks *pObject, void *pBoundArgument, int32 iZIndex)
{
	CEventBoundFunction *pEventBoundFunction = createEventBoundFunctionObject(uiEventTypeId, uiEventTypeIndex, uiEventId, pBoundArgument, iZIndex);
	pEventBoundFunction->setEventFunctionType(EVENT_FUNCTION_TYPE_OBJECT_CALLBACK);
	pEventBoundFunction->setCallbackObject(pObject);
	m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].insert(m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].begin() + getInsertionIndexForEventBoundFunction(pEventBoundFunction), pEventBoundFunction);
	return pEventBoundFunction;
}

bool										CEventManager::unbindEvent(CEventBoundFunction *pEventBoundFunction)
{
	if (!doesEventExist(pEventBoundFunction->getEventTypeId(), pEventBoundFunction->getEventTypeIndex(), pEventBoundFunction->getEventId()))
	{
		return false;
	}

	vector<CEventBoundFunction*>& vecBoundFunctions = m_umapEventFunctions[pEventBoundFunction->getEventTypeId()][pEventBoundFunction->getEventTypeIndex()][pEventBoundFunction->getEventId()];
	auto it = std::find(vecBoundFunctions.begin(), vecBoundFunctions.end(), pEventBoundFunction);
	if (it == vecBoundFunctions.end())
	{
		return false;
	}

	vecBoundFunctions.erase(it);
}

bool										CEventManager::doesEventExist(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId)
{
	return m_umapEventFunctions.count(uiEventTypeId) != 0
		|| m_umapEventFunctions[uiEventTypeId].count(uiEventId) != 0
		|| m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex].count(uiEventId) != 0;
}

uint32										CEventManager::getBoundFunctionCount(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId)
{
	if (!doesEventExist(uiEventTypeId, uiEventTypeIndex, uiEventId))
	{
		return 0;
	}
	return m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].size();
}

bool										CEventManager::triggerEvent(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void *pTriggerArgument)
{
	if (!doesEventExist(uiEventTypeId, uiEventTypeIndex, uiEventId))
	{
		return true;
	}
	uint32 uiEventFunctionIndex = 0;
	while (uiEventFunctionIndex < m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId].size())
	{
		CEventBoundFunction *pEventBoundFunction = m_umapEventFunctions[uiEventTypeId][uiEventTypeIndex][uiEventId][uiEventFunctionIndex];
		pEventBoundFunction->onEventTriggered(uiEventId, pTriggerArgument);
		uiEventFunctionIndex++;
	}
	bool bDefaultActionPrevented = isDefaultActionPrevented();
	setDefaultActionPrevented(false);
	return !bDefaultActionPrevented;
}

// CEventManager continued
CEventBoundFunction*						CEventManager::createEventBoundFunctionObject(uint32 uiEventTypeId, uint32 uiEventTypeIndex, uint32 uiEventId, void *pBoundArgument, int32 iZIndex)
{
	CEventBoundFunction *pEventBoundFunction = new CEventBoundFunction;
	pEventBoundFunction->setEventTypeId(uiEventTypeId);
	pEventBoundFunction->setEventTypeIndex(uiEventTypeIndex);
	pEventBoundFunction->setEventId(uiEventId);
	pEventBoundFunction->setFunctionBoundArgument(pBoundArgument);
	pEventBoundFunction->setZIndex(iZIndex);
	return pEventBoundFunction;
}

uint32										CEventManager::getInsertionIndexForEventBoundFunction(CEventBoundFunction *pEventBoundFunction)
{
	uint32 uiEventBoundFunctionInsertionIndex = 0;

	if (!doesEventExist(pEventBoundFunction->getEventTypeId(), pEventBoundFunction->getEventTypeIndex(), pEventBoundFunction->getEventId()))
	{
		return uiEventBoundFunctionInsertionIndex;
	}

	int32 iZIndex = pEventBoundFunction->getZIndex();
	for (CEventBoundFunction *pEventBoundFunction2 : m_umapEventFunctions[pEventBoundFunction->getEventTypeId()][pEventBoundFunction->getEventTypeIndex()][pEventBoundFunction->getEventId()])
	{
		if (iZIndex < pEventBoundFunction2->getZIndex())
		{
			return uiEventBoundFunctionInsertionIndex;
		}
		uiEventBoundFunctionInsertionIndex++;
	}
	return uiEventBoundFunctionInsertionIndex;
}