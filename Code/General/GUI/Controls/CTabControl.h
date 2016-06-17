#ifndef CTabControl_H
#define CTabControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Entries/CTabControlEntry.h"
#include <string>
#include <unordered_map>

class CGUILayer;

class CTabControl : public CGUIControl, public CVectorPool<CTabControlEntry*>
{
public:
	CTabControl(void);

	void									bindEvents(void);

	void									unserialize(bool bSkipControlId = false);
	void									serialize(void);

	void									onLeftMouseDown(CPoint2D& vecCursorPosition);

	void									render(void);

	uint32									getActiveIndex(void);

	CTabControlEntry*						addTab(std::string strTabText, bool bSetActiveTab = false);
	void									removeTab(CTabControlEntry *pTab);

	CTabControlEntry*						getTabFromPosition(CPoint2D& vecPosition);

	void									bindTabLayer(CTabControlEntry *pTab, CGUILayer *pLayer);
	void									applyTabLayer(CTabControlEntry *pTab, CTabControlEntry *pPreviousTab);

	void									setActiveTab(CTabControlEntry* pActiveTab) { m_pActiveTab = pActiveTab; }
	CTabControlEntry*						getActiveTab(void) { return m_pActiveTab; }

	void									setActiveTabHeightDifference(uint32 uiActiveTabHeightDifference) { m_uiActiveTabHeightDifference = uiActiveTabHeightDifference; }
	uint32									getActiveTabHeightDifference(void) { return m_uiActiveTabHeightDifference; }

private:
	CTabControlEntry*										m_pActiveTab;
	uint32													m_uiActiveTabHeightDifference;
	std::unordered_map<CTabControlEntry*, CGUILayer*>		m_umapTabLayers;
};

#endif