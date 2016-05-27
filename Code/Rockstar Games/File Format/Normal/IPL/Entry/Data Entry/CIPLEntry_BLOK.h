#ifndef CIPLEntry_BLOK_H
#define CIPLEntry_BLOK_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_BLOK : public CIPLEntry_Data
{
public:
	CIPLEntry_BLOK(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_BLOK)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif