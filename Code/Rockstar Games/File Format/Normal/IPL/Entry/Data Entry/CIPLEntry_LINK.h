#ifndef CIPLEntry_LINK_H
#define CIPLEntry_LINK_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_LINK : public CIPLEntry_Data
{
public:
	CIPLEntry_LINK(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_LINK)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif