#ifndef CIDEEntry_2DFX_Audio_H
#define CIDEEntry_2DFX_Audio_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_Audio : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_Audio(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_AUDIO)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif