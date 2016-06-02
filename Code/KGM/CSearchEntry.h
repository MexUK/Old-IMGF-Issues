#ifndef CSearchEntry_H
#define CSearchEntry_H

class CIMGEditorTab;
class CIMGEntry;

class CSearchEntry
{
public:
	CSearchEntry(void) :
		m_pEditorTab(nullptr),
		m_pIMGEntry(nullptr)
	{};

	void						setWindowTab(CIMGEditorTab* pEditorTab) { m_pEditorTab = pEditorTab; }
	CIMGEditorTab*				getWindowTab(void) { return m_pEditorTab; }

	void						setIMGEntry(CIMGEntry* pIMGEntry) { m_pIMGEntry = pIMGEntry; }
	CIMGEntry*					getIMGEntry(void) { return m_pIMGEntry; }

private:
	CIMGEditorTab*				m_pEditorTab;
	CIMGEntry*					m_pIMGEntry;
};

#endif