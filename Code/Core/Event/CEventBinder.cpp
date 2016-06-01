#include "CEventBinder.h"
#include "CEventManager.h"

void									CEventBinder::unbindEvents(void)
{
	CEventManager *pEventManager = CEventManager::getInstance();
	for (CEventBoundFunction *pEventBoundFunction : m_vecEventBoundFunctions.getEntries())
	{
		pEventManager->unbindEvent(pEventBoundFunction);
	}
	m_vecEventBoundFunctions.removeAllEntries();
}

void									CEventBinder::storeEventBoundFunction(CEventBoundFunction *pEventBoundFunction)
{
	m_vecEventBoundFunctions.addEntry(pEventBoundFunction);
}