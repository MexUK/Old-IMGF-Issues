#ifndef CIntermediateTextureFormat_H
#define CIntermediateTextureFormat_H

#include "Pool/CVectorPool.h"
#include "CIntermediateTexture.h"

class CIntermediateTextureFormat : public CVectorPool<CIntermediateTexture*>
{
public:
	void						unload(void);
};

#endif