#ifndef CTabControl_H
#define CTabControl_H

#include "Types.h"
#include "GUI/Control/CGUIControl.h"
#include "GUI/Controls/Entries/CTabControlEntry.h"
#include <string>
#include <unordered_map>

class CTabControlEntry;
class CControlGroup;

class CTabControl : public CGUIControl, public CVectorPool<CTabControlEntry*>
{
public:
	CTabControl(void);

	void									bindEvents(void);

	void									onLeftMouseDown(CVector2i32& vecCursorPosition);

	void									render(void);

	CTabControlEntry*						addTab(std::string strTabText, bool bSetActiveTab = false);
	void									removeTab(CTabControlEntry *pTab);

	CTabControlEntry*						getTabFromPosition(CVector2i32& vecPosition);

	void									bindTabLayer(CTabControlEntry *pTab, CControlGroup *pControlGroup);
	void									applyTabLayer(CTabControlEntry *pTab, CTabControlEntry *pPreviousTab);

	void									setActiveTab(CTabControlEntry* pActiveTab) { m_pActiveTab = pActiveTab; }
	CTabControlEntry*						getActiveTab(void) { return m_pActiveTab; }

private:
	CTabControlEntry*						m_pActiveTab;
	std::unordered_map<CTabControlEntry*, CControlGroup*>		m_umapTabLayers;
};

#endif