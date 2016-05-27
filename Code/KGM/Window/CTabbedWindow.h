#ifndef CTabbedWindow_H
#define CTabbedWindow_H

#include "Types.h"
#include "CWindow.h"
#include "CWindowTab.h"
#include "Pool/CVectorPool.h"

class CEntryListWindowTab;

class CTabbedWindow : public CWindow, public CVectorPool<CWindowTab*>
{
public:
	CTabbedWindow(void);

	void									unload(void);

	void									setActiveTab(CWindowTab *pActiveTab) { m_pActiveTab = pActiveTab; }
	CWindowTab*								getActiveTab(void) { return m_pActiveTab; }
	CEntryListWindowTab*					getEntryListTab(void) { return (CEntryListWindowTab*)m_pActiveTab; }

	uint32									getNextTabIndex(void);

private:
	CWindowTab*								m_pActiveTab;
};

#endif