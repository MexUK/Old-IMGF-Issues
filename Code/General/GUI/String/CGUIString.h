#ifndef CGUIString_H
#define CGUIString_H

#include "Types.h"
#include "CVector2ui32.h"
#include <string>

class CGUIString
{
public:
	void							setSize(CVector2ui32& vecSize) { m_vecSize = vecSize; }
	CVector2ui32&					getSize(void) { return m_vecSize; }

	void							setString(std::string& strString) { m_strString = strString; }
	std::string&					getString(void) { return m_strString; }

private:
	CVector2ui32					m_vecSize;
	std::string						m_strString;
};

#endif