#ifndef CRWSection_UnknownSection_H
#define CRWSection_UnknownSection_H

#include "../CRWSection.h"
#include <string>

class CRWSection_UnknownSection : public CRWSection
{
public:
	CRWSection_UnknownSection(void);

	void							unserialize(void);
	void							serialize(void);

	void							setActualSectionId(uint32 uiActualSectionId) { m_uiActualSectionId = uiActualSectionId; }
	uint32							getActualSectionId(void) { return m_uiActualSectionId; }

	void							setData(std::string& strData) { m_strData = strData; }
	std::string&					getData(void) { return m_strData; }

private:
	uint32							m_uiActualSectionId;
	std::string						m_strData;
};

#endif