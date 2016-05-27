#ifndef CIDEEntry_LINK_H
#define CIDEEntry_LINK_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_LINK : public CIDEEntry_Data
{
public:
	CIDEEntry_LINK(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_LINK)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif