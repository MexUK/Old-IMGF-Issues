#include "CIntermediateTextureFormat.h"

void			CIntermediateTextureFormat::unload(void)
{
	for (auto pGeneralTexture : getEntries())
	{
		for (auto pGeneralMipmap : pGeneralTexture->getEntries())
		{
			delete pGeneralMipmap;
		}
		delete pGeneralTexture;
	}
}