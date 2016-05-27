#ifndef CRWSection_Frame_H
#define CRWSection_Frame_H

#include "../CRWSection.h"
#include <string>

class CRWSection_Frame : public CRWSection
{
public:
	CRWSection_Frame(void);

	void							unserialize(void);
	void							serialize(void);

	void							setData(std::string& strData) { m_strData = strData; }
	std::string&					getData(void) { return m_strData; }

private:
	std::string						m_strData;
};

#endif