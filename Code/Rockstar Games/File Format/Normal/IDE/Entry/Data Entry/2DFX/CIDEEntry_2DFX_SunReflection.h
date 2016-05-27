#ifndef CIDEEntry_2DFX_SunReflection_H
#define CIDEEntry_2DFX_SunReflection_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"

class CIDEEntry_2DFX_SunReflection : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_SunReflection(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_SUN_REFLECTION)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif