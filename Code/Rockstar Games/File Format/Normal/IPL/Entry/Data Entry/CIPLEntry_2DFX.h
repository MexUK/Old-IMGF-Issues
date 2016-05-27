#ifndef CIPLEntry_2DFX_H
#define CIPLEntry_2DFX_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_2DFX : public CIPLEntry_Data
{
public:
	CIPLEntry_2DFX(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_2DFX)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif