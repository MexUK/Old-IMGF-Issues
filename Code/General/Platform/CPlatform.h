#ifndef CPlatform_H
#define CPlatform_H

#include "ePlatform.h"
#include <string>

class CPlatform
{
public:
	void					unload(void) {}

	void					setPlatformId(ePlatform ePlatformValue) { m_ePlatformId = ePlatformValue; }
	ePlatform				getPlatformId(void) { return m_ePlatformId; }

	void					setName(std::string strName) { m_strName = strName; }
	std::string				getName(void) { return m_strName; }

private:
	ePlatform				m_ePlatformId;
	std::string				m_strName;
};

#endif