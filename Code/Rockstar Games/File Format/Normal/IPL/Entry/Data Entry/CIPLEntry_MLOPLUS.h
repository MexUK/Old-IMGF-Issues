#ifndef CIPLEntry_MLOPLUS_H
#define CIPLEntry_MLOPLUS_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_MLOPLUS : public CIPLEntry_Data
{
public:
	CIPLEntry_MLOPLUS(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_MLOPLUS)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif