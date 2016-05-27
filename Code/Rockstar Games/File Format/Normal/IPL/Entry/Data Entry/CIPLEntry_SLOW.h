#ifndef CIPLEntry_SLOW_H
#define CIPLEntry_SLOW_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_SLOW : public CIPLEntry_Data
{
public:
	CIPLEntry_SLOW(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_SLOW)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif