#ifndef CEditor_H
#define CEditor_H

#include "GUI/Layer/CGUILayer.h"
#include "Pool/CVectorPool.h"
#include "GUI/Editor/Tab/CEditorTab.h"

class CKGMWindow;

class CEditor : public CGUILayer
{
public:
	CEditor(void);

	CKGMWindow*					getWindow(void) { return (CKGMWindow*) CGUILayer::getWindow(); }

	void						setActiveTab(CEditorTab *pActiveTab) { m_pActiveTab = pActiveTab; }
	CEditorTab*					getActiveTab(void) { return m_pActiveTab; }

	CVectorPool<CEditorTab*>&	getTabs(void) { return m_vecTabs; }

private:
	CEditorTab*					m_pActiveTab;
	CVectorPool<CEditorTab*>	m_vecTabs;
};

#endif