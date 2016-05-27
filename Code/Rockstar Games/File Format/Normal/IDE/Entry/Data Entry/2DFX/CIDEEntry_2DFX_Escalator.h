#ifndef CIDEEntry_2DFX_Escalator_H
#define CIDEEntry_2DFX_Escalator_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_Escalator : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_Escalator(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_ESCALATOR)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif