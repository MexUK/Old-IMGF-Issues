#ifndef CEditor_H
#define CEditor_H

#include "GUI/Group/CControlGroup.h"
#include "Pool/CVectorPool.h"
#include "GUI/Editor/Tab/CEditorTab.h"

class CKGMWindow;

class CEditor : public CControlGroup
{
public:
	CEditor(void);

	CKGMWindow*					getWindow(void) { return (CKGMWindow*) CControlGroup::getWindow(); }

	void						setActiveTab(CEditorTab *pActiveTab) { m_pActiveTab = pActiveTab; }
	CEditorTab*					getActiveTab(void) { return m_pActiveTab; }

	CVectorPool<CEditorTab*>&	getTabs(void) { return m_vecTabs; }

private:
	CEditorTab*					m_pActiveTab;
	CVectorPool<CEditorTab*>	m_vecTabs;
};

#endif