#ifndef CIDEEntry_MLO_H
#define CIDEEntry_MLO_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_MLO : public CIDEEntry_Data
{
public:
	CIDEEntry_MLO(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_MLO)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif