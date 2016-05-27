#ifndef CIDEEntry_LODM_H
#define CIDEEntry_LODM_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_LODM : public CIDEEntry_Data
{
public:
	CIDEEntry_LODM(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_LODM)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif