#ifndef CIDEEntry_2DFX_WorldPoint_H
#define CIDEEntry_2DFX_WorldPoint_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_WorldPoint : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_WorldPoint(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_WORLD_POINT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif