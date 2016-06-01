#ifndef CWindowControlEntry_Drop_H
#define CWindowControlEntry_Drop_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include <string>

class CWindowControlEntry_Drop
{
public:
	void									unload(void) {};

	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
private:
	std::string								m_strText;
};

#endif