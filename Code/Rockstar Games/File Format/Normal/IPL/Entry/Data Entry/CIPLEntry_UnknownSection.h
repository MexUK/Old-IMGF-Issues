#ifndef CIPLEntry_UnknownSection_H
#define CIPLEntry_UnknownSection_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"
#include <string>

class CIPLEntry_UnknownSection : public CIPLEntry_Data
{
public:
	CIPLEntry_UnknownSection(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_UNKNOWN)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setLine(std::string& strLine) { m_strLine = strLine; }
	std::string&				getLine(void) { return m_strLine; }

private:
	std::string					m_strLine;
};

#endif