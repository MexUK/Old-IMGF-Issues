#ifndef CIPLEntry_VNOD_H
#define CIPLEntry_VNOD_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_VNOD : public CIPLEntry_Data
{
public:
	CIPLEntry_VNOD(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_VNOD)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif