#ifndef CIDEEntry_VNOD_H
#define CIDEEntry_VNOD_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_VNOD : public CIDEEntry_Data
{
public:
	CIDEEntry_VNOD(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_VNOD)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif