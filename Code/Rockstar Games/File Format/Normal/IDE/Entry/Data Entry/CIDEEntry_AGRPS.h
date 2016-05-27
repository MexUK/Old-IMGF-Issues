#ifndef CIDEEntry_AGRPS_H
#define CIDEEntry_AGRPS_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_AGRPS: public CIDEEntry_Data
{
public:
	CIDEEntry_AGRPS(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_AGRPS)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif