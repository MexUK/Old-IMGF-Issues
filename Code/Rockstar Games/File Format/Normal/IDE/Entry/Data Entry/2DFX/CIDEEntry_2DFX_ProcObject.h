#ifndef CIDEEntry_2DFX_ProcObject_H
#define CIDEEntry_2DFX_ProcObject_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include <string>

class CIDEEntry_2DFX_ProcObject : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_ProcObject(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_PROC_OBJECT)
	{
	};

	void						unserialize(void);
	void						serialize(void);
};

#endif