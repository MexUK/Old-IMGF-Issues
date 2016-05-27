#ifndef CWindowControlEntry_List_H
#define CWindowControlEntry_List_H

#include "Types.h"
#include <string>
#include <vector>

class CWindowControl_List;

class CWindowControlEntry_List
{
public:
	CWindowControlEntry_List(void) :
		m_pList(nullptr)
	{};
	
	void									unload(void) {};

	void									setList(CWindowControl_List *pList) { m_pList = pList; }
	CWindowControl_List*					getList(void) { return m_pList; }
	
	std::vector<std::vector<std::string>>&	getText(void) { return m_vecText; }
	
private:
	CWindowControl_List*					m_pList;
	std::vector<std::vector<std::string>>	m_vecText;
};

#endif