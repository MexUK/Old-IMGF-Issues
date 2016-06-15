#ifndef CDropControlEntry_H
#define CDropControlEntry_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "GUI/String/CGUIString.h"
#include <string>

class CDropControlEntry
{
public:
	void									unload(void) {}

	void									setText(std::string& strText) { m_gstrText.setString(strText); }
	std::string&							getText(void) { return m_gstrText.getString(); }

	CGUIString&								getGUIString(void) { return m_gstrText; }
	
private:
	CGUIString								m_gstrText;
};

#endif