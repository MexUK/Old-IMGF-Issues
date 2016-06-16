#include "CTabControlEntry.h"
#include "GUI/Controls/CTabControl.h"

CTabControlEntry::CTabControlEntry(void) :
	m_pTabControl(nullptr)
{
}

// active tab
void									CTabControlEntry::setActiveTab(void)
{
	getTabControl()->setActiveTab(this);
}

bool									CTabControlEntry::isActiveTab(void)
{
	return getTabControl()->getActiveTab() == this;
}