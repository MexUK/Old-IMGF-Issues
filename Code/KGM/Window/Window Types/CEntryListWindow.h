#ifndef CEntryListWindow_H
#define CEntryListWindow_H

#include "Types.h"
#include "Window/CTabbedWindow.h"
#include "Window/Window Types/Tabs/CEntryListWindowTab.h"
#include "IMG/eIMGVersion.h"
#include <string>

class CSearchEntry;
class CWindowControl_List;

class CEntryListWindow : public CTabbedWindow
{
public:
	CEntryListWindow(void) :
		CTabbedWindow(),
		m_uiSelectedEntryCount(0),
		m_uiSearchHitCount(0), // todo - rename to SearchHitEntryCount
		m_uiSearchFileCount(0), // todo - rename to SearchHitFileCount
		m_pEntryListControl(nullptr)
	{};

	void						initMenu(void);
	void						loadRightClickMenu(int xPos, int yPos);

	CEntryListWindowTab*		addTab(std::string strIMGPath, eIMGVersion eIMGVersionValue);
	CEntryListWindowTab*		addBlankTab(std::string strIMGPath, eIMGVersion eIMGVersionValue);
	void						removeTab(CWindowTab *pWindowTab);

	void						refreshActiveTab(void);
	CEntryListWindowTab*		getActiveTab(void) { return (CEntryListWindowTab*)CTabbedWindow::getActiveTab(); }
	void						setActiveTab(CEntryListWindowTab *pActiveTab);

	void						readdColumnsToMainListView(eIMGVersion eIMGVersionValue);
	void						addColumnsToMainListView(eIMGVersion eIMGVersionValue);
	void						removeColumnsFromMainListView(void);
	int							getMainListControlItemByEntry(CIMGEntry *pIMGEntry); // todo - naming -- list control vs list view

	void						onSelectIMGEntry(bool bEntryIsSelected);

	void						logAllTabs(std::string strText, bool bExtendedModeOnly = false);
	void						logWithNoTabsOpen(std::string strText, bool bExtendedModeOnly = false);

	uint32						getEntryCountForAllTabs(void);
	void						updateSelectedEntryCountText(void);

	std::vector<CIMGFormat*>	getAllMainWindowTabsIMGFiles(void);

	void						setSelectedEntryCount(uint32 uiSelectedEntryCount) { m_uiSelectedEntryCount = uiSelectedEntryCount; }
	uint32						getSelectedEntryCount(void) { return m_uiSelectedEntryCount; }

	void						setSearchHitCount(uint32 uiSearchHitCount) { m_uiSearchHitCount = uiSearchHitCount; }
	uint32						getSearchHitCount(void) { return m_uiSearchHitCount; }

	void						setSearchFileCount(uint32 uiSearchFileCount) { m_uiSearchFileCount = uiSearchFileCount; }
	uint32						getSearchFileCount(void) { return m_uiSearchFileCount; }

	void						setEntryListControl(CWindowControl_List *pEntryListControl) { m_pEntryListControl = pEntryListControl; }
	CWindowControl_List*		getEntryListControl(void) { return m_pEntryListControl; }

	std::vector<CSearchEntry*>&	getSearchEntries(void) { return m_vecSearchEntries; }

private:
	CEntryListWindowTab*		_addTab(CIMGFormat *pIMGFormat);

private:
	uint32						m_uiSelectedEntryCount;
	uint32						m_uiSearchHitCount;
	uint32						m_uiSearchFileCount;
	CWindowControl_List*		m_pEntryListControl;
	std::vector<CSearchEntry*>	m_vecSearchEntries;
};

#endif