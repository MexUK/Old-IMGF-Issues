#ifndef CIDEEntry_2DFX_LightShaft_H
#define CIDEEntry_2DFX_LightShaft_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_LightShaft : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_LightShaft(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_LIGHT_SHAFT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif