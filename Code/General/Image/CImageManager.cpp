#include "CImageManager.h"
#include "CRasterDataFormatManager.h"
#include "CImageFile.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "BMP/CBMPManager.h"
#include "BMP/CBMPFormat.h"
#include "DDS/CDDSFormat.h"
#include "Image/CImageUtility.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

CImageManager::CImageManager(void)
{
	m_pRasterDataFormatManager = new CRasterDataFormatManager;
}
CImageManager::~CImageManager(void)
{
	delete m_pRasterDataFormatManager;
}

void					CImageManager::init(void)
{
	m_pRasterDataFormatManager->init();
}
void					CImageManager::uninit(void)
{
	m_pRasterDataFormatManager->uninit();
}

CImageFile*				CImageManager::loadImageFromFile(string& strFilePath)
{
	string strFileExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFilePath));
	CImageFile *pImageFile = new CImageFile;

	if (strFileExtensionUpper == "BMP")
	{
		CBMPFormat *pBMPFormat = CBMPManager::getInstance()->parseViaFile(strFilePath);
		pImageFile->m_uiImageWidth = pBMPFormat->getWidth();
		pImageFile->m_uiImageHeight = pBMPFormat->getHeight();
		pImageFile->m_uiBPP = pBMPFormat->getBPP();
		pImageFile->m_strRasterDataBGRA32 = pBMPFormat->getRasterDataBGRA32();
		delete pBMPFormat;
	}
	else if (strFileExtensionUpper == "PNG"
		  || strFileExtensionUpper == "GIF"
		  || strFileExtensionUpper == "JPG"
		  || strFileExtensionUpper == "JPEG"
		  || strFileExtensionUpper == "TGA"
		  || strFileExtensionUpper == "PGM"
		  || strFileExtensionUpper == "PPM"
		  || strFileExtensionUpper == "HDR")
	{
		int w, h, n;
		uint8 *pBGRAData = stbi_load(strFilePath.c_str(), &w, &h, &n, 4);
		if (pBGRAData == NULL)
		{
			stbi_image_free(pBGRAData);
			return nullptr;
		}
		pImageFile->m_uiImageWidth = w;
		pImageFile->m_uiImageHeight = h;
		pImageFile->m_uiBPP = 32;
		pImageFile->m_strRasterDataBGRA32 = CImageUtility::convertRGBA32ToBGRA32(string((char*)pBGRAData, w * h * 4));
		stbi_image_free(pBGRAData);
	}
	else if (strFileExtensionUpper == "DDS")
	{
		CDDSFormat ddsFormat;
		ddsFormat.unserializeViaFile(strFilePath);
		pImageFile->m_uiImageWidth = ddsFormat.m_uiWidth;
		pImageFile->m_uiImageHeight = ddsFormat.m_uiHeight;
		pImageFile->m_uiBPP = 32;
		//pImageFile->m_strRasterDataBGRA32 = ddsFile.m_strRasterData;
		string strPaletteDataIn = "";
		pImageFile->m_strRasterDataBGRA32 = CImageUtility::convertRasterDataFormatToBGRA32(ddsFormat.m_strRasterData, RASTERDATAFORMAT_DXT1, strPaletteDataIn, ddsFormat.m_uiWidth, ddsFormat.m_uiHeight);
	}
	return pImageFile;
}