#ifndef CIDEEntry_TREE_H
#define CIDEEntry_TREE_H

#include "Types.h"
#include "IDE/Entry/CIDEEntry_Data.h"

class CIDEEntry_TREE : public CIDEEntry_Data
{
public:
	CIDEEntry_TREE(CIDEFormat *pIDEFormat) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_TREE)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif