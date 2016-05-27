#ifndef CIDEEntry_TANM_H
#define CIDEEntry_TANM_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_TANM : public CIDEEntry_Data
{
public:
	CIDEEntry_TANM(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_TANM)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif