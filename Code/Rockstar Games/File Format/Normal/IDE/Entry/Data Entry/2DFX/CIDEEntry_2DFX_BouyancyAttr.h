#ifndef CIDEEntry_2DFX_BouyancyAttr_H
#define CIDEEntry_2DFX_BouyancyAttr_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_BouyancyAttr : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_BouyancyAttr(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_BOUYANCY_ATTR)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif