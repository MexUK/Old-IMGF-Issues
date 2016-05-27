#ifndef CIDEEntry_AMAT_H
#define CIDEEntry_AMAT_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_AMAT : public CIDEEntry_Data
{
public:
	CIDEEntry_AMAT(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_AMAT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif