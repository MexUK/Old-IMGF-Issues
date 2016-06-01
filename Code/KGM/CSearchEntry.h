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

	void						setWindowTab(CIMGScreenTab* pWindowTab) { m_pWindowTab = pWindowTab; }
	CIMGScreenTab*		getWindowTab(void) { return m_pWindowTab; }

	void						setIMGEntry(CIMGEntry* pIMGEntry) { m_pIMGEntry = pIMGEntry; }
	CIMGEntry*					getIMGEntry(void) { return m_pIMGEntry; }

private:
	CIMGScreenTab*		m_pWindowTab;
	CIMGEntry*					m_pIMGEntry;
};

#endif