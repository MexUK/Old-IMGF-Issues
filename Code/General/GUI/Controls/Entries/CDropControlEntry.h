#ifndef CDropControlEntry_H
#define CDropControlEntry_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include <string>

class CDropControlEntry
{
public:
	void									unload(void) {};

	void									setText(std::string& strText) { m_strText = strText; }
	std::string&							getText(void) { return m_strText; }
	
private:
	std::string								m_strText;
};

#endif