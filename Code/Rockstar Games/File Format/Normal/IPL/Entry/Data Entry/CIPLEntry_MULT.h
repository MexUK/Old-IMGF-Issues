#ifndef CIPLEntry_MULT_H
#define CIPLEntry_MULT_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_MULT : public CIPLEntry_Data
{
public:
	CIPLEntry_MULT(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_MULT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif