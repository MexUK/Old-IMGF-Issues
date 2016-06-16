#ifndef CGUIString_H
#define CGUIString_H

#include "Types.h"
#include "CSize2D.h"
#include <string>

class CGUIString
{
public:
	void							setSize(CSize2D& vecSize) { m_vecSize = vecSize; }
	CSize2D&						getSize(void) { return m_vecSize; }

	void							setString(std::string& strString) { m_strString = strString; }
	std::string&					getString(void) { return m_strString; }

private:
	CSize2D							m_vecSize;
	std::string						m_strString;
};

#endif