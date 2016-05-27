#ifndef CIDEEntry_2DFX_ScrollBar_H
#define CIDEEntry_2DFX_ScrollBar_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_ScrollBar : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_ScrollBar(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_SCROLL_BAR)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif