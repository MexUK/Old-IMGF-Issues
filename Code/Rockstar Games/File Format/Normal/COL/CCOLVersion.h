#ifndef CCOLVersion_H
#define CCOLVersion_H

#include "eCOLVersion.h"
#include <string>

class CCOLVersion
{
public:
	void				unload(void) {}

	void				setVersionId(eCOLVersion eVersionId) { m_eVersionId = eVersionId; }
	eCOLVersion			getVersionId(void) { return m_eVersionId; }

	void				setText(std::string strText) { m_strText = strText; }
	std::string			getText(void) { return m_strText; }

	void				setLocalizationKey(std::string strLocalizationKey) { m_strLocalizationKey = strLocalizationKey; }
	std::string			getLocalizationKey(void) { return m_strLocalizationKey; }

private:
	eCOLVersion			m_eVersionId;
	std::string			m_strText;
	std::string			m_strLocalizationKey;
};

#endif