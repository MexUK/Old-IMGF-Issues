#include "CControlGroup.h"

CControlGroup::CControlGroup(void) :
	m_pWindow(nullptr)
{
}

// event binding
void									CControlGroup::bindAllEvents(void)
{
	for (CWindowControl *pWindowControl : getEntries())
	{
		pWindowControl->bindEvents();
	}
}

void									CControlGroup::unbindAllEvents(void)
{
	for (CWindowControl *pWindowControl : getEntries())
	{
		pWindowControl->unbindEvents();
	}
}