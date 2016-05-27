#ifndef CSearchEntry_H
#define CSearchEntry_H

class CWindowTab;
class CIMGEntry;

class CSearchEntry
{
public:
	CSearchEntry(void) :
		m_pWindowTab(nullptr),
		m_pIMGEntry(nullptr)
	{};

	void						setWindowTab(CEntryListWindowTab* pWindowTab) { m_pWindowTab = pWindowTab; }
	CEntryListWindowTab*		getWindowTab(void) { return m_pWindowTab; }

	void						setIMGEntry(CIMGEntry* pIMGEntry) { m_pIMGEntry = pIMGEntry; }
	CIMGEntry*					getIMGEntry(void) { return m_pIMGEntry; }

private:
	CEntryListWindowTab*		m_pWindowTab;
	CIMGEntry*					m_pIMGEntry;
};

#endif