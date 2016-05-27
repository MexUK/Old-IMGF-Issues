#include "CEventManager.h"

using namespace std;

void										CEventManager::bindEvent(string strEventName, void(*fEventFunction)(void*))
{
	m_umapBoundFunctions[strEventName].push_back(fEventFunction);
}

uint32										CEventManager::getBoundFunctionCount(string strEventName)
{
	if(m_umapBoundFunctions.count(strEventName) == 0)
	{
		return 0;
	}
	return m_umapBoundFunctions[strEventName].size();
}

bool										CEventManager::triggerEvent(string strEventName, void *pData)
{
	if(m_umapBoundFunctions.count(strEventName) == 0)
	{
		return true;
	}
	for(void(*fEventFunction)(void*) : m_umapBoundFunctions[strEventName])
	{
		fEventFunction(pData);
	}
	bool bDefaultActionPrevented = isDefaultActionPrevented();
	setDefaultActionPrevented(false);
	return !bDefaultActionPrevented;
}