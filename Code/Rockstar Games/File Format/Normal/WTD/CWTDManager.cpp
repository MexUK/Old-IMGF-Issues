#include "CWTDManager.h"
#include "CWTDEntry.h"
#include "File/CFileUtility.h"
#include "String/CStringUtility.h"
//#include "CIMGManager.h" // WTD Manager project only relies upon IMG Manager project for it's decompressZLib function.
#include "Testing/CDebugger.h"
#include "CWTDMipmap.h"
#include "Texture/CIntermediateTextureFormat.h"
#include "Texture/CIntermediateTexture.h"
#include "Texture/Data/CIntermediateTextureMipmap.h"
#include "Image/CImageManager.h"
#include "Image/CImageUtility.h"
#include "Compression/CCompressionUtility.h"
#include "Image/eRasterDataFormat.h"

using namespace std;

CWTDManager::CWTDManager(void)
{
}

void					CWTDManager::init(void)
{
}
void					CWTDManager::uninit(void)
{
}

uint32			CWTDManager::getImageDataSize(CWTDEntry *pWTDEntry, bool bIncludeLevels)
{
	uint32 uiImageDataSize = 0;
	switch (pWTDEntry->getD3DFormat())
	{
	case D3DFMT_DXT1:
		uiImageDataSize = pWTDEntry->getImageSize(true) * pWTDEntry->getImageSize(false) / 2;
		break;
	case D3DFMT_DXT3:
	case D3DFMT_DXT5:
		uiImageDataSize = pWTDEntry->getImageSize(true) * pWTDEntry->getImageSize(false);
		break;
	}

	if (bIncludeLevels)
	{
		int levels = pWTDEntry->getLevels();
		int levelDataSize = uiImageDataSize;
		while (levels > 1)
		{
			uiImageDataSize += (levelDataSize / 4);

			levelDataSize /= 4;

			// clamp to 16 bytes
			if (levelDataSize < 16)
			{
				if (pWTDEntry->getD3DFormat() == D3DFMT_DXT1 && levelDataSize < 8)
				{
					levelDataSize = 8;
				}
				else
				{
					levelDataSize = 16;
				}
			}

			levels--;
		}
	}

	return uiImageDataSize;
}

CWTDFormat*				CWTDManager::convertIntermediateTextureFileToWTDFile(CIntermediateTextureFormat *pGeneralTextureFile)
{
	CWTDFormat *pWTDFile = new CWTDFormat;

	for (CIntermediateTexture *pGeneralTexture : pGeneralTextureFile->getEntries())
	{
		CWTDEntry *pWTDEntry = new CWTDEntry;

		pWTDEntry->setEntryName(pGeneralTexture->getName());
		pWTDEntry->setD3DFormat(CImageUtility::getD3DFormatFromRasterDataFormat(pGeneralTexture->getRasterDataFormat()));
		pWTDEntry->setImageSize(true, pGeneralTexture->getSize().m_x);
		pWTDEntry->setImageSize(false, pGeneralTexture->getSize().m_y);
		
		for (CIntermediateTextureMipmap *pGeneralMipmap : pGeneralTexture->getEntries())
		{
			CWTDMipmap *pWTDMipmap = new CWTDMipmap(pWTDEntry);

			pWTDMipmap->setRasterData(pGeneralMipmap->getRasterData());
			pWTDMipmap->setImageSize(true, pGeneralMipmap->getSize().m_x);
			pWTDMipmap->setImageSize(false, pGeneralMipmap->getSize().m_y);

			pWTDEntry->addEntry(pWTDMipmap);
		}

		pWTDFile->addEntry(pWTDEntry);
	}

	return pWTDFile;
}