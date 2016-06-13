#ifndef CListControlEntry_H
#define CListControlEntry_H

#include "Types.h"
#include <string>
#include <vector>

class CListControl;

class CListControlEntry
{
public:
	CListControlEntry(void);
	
	void									unload(void) {}

	void									setList(CListControl *pList) { m_pList = pList; }
	CListControl*							getList(void) { return m_pList; }
	
	std::vector<std::vector<std::string>>&	getText(void) { return m_vecText; }
	
private:
	CListControl*							m_pList;
	std::vector<std::vector<std::string>>	m_vecText;
};

#endif