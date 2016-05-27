#ifndef CIPLEntry_LODM_H
#define CIPLEntry_LODM_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_LODM : public CIPLEntry_Data
{
public:
	CIPLEntry_LODM(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_LODM)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif