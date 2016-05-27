#ifndef CIPLEntry_RTFX_H
#define CIPLEntry_RTFX_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_RTFX : public CIPLEntry_Data
{
public:
	CIPLEntry_RTFX(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_RTFX)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif