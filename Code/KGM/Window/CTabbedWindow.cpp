#include "CTabbedWindow.h"

CTabbedWindow::CTabbedWindow(void) :
	CWindow(),
	m_pActiveTab(nullptr)
{
}

void									CTabbedWindow::unload(void)
{
	CWindow::unload();
	removeAllEntries();
}

uint32									CTabbedWindow::getNextTabIndex(void)
{
	uint32 uiTabIndex = 0;
	while (getEntryByIndex(uiTabIndex) != nullptr)
	{
		uiTabIndex++;
	}
	return uiTabIndex;
}