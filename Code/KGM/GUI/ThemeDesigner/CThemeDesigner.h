#ifndef CThemeDesigner_H
#define CThemeDesigner_H

#include <string>
#include <unordered_map>

class CWindow;
class CTabControl;
class CTabControlEntry;

class CThemeDesigner
{
public:
	CThemeDesigner(void);

	void					init(void);

	void					initWindow(void);
	void					initWindowDesign(void);
	
	void					setWindow(CWindow* pWindow) { m_pWindow = pWindow; }
	CWindow*				getWindow(void) { return m_pWindow; }

	void					setTabControl(CTabControl *pTabControl) { m_pTabControl = pTabControl; }
	CTabControl*			getTabControl(void) { return m_pTabControl; }

private:
	void					initTab_AddItem(void);
	void					initTab_Items(void);

private:
	CWindow*												m_pWindow;
	CTabControl*											m_pTabControl;
	std::unordered_map<std::string, CTabControlEntry*>		m_umapTabControlEntries;
};

#endif