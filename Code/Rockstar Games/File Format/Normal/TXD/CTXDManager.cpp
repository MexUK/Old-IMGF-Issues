#include <Windows.h>
#include <d3d11_1.h>
#include <d2d1.h>
#include <DXGIFormat.h>
#include "CTXDManager.h"
#include "CTXDFormat.h"
#include "RW/Sections/CRWSection_TextureNative.h"
#include "Data Stream/CDataReader.h"
#include "String/CStringUtility.h"
#include "Engine/RW/CRWManager.h"
#include "Testing/CDebugger.h"
#include "Image/CRasterDataFormatManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Texture/CIntermediateTextureFormat.h"
#include "Texture/CIntermediateTexture.h"
#include "Texture/Data/CIntermediateTextureMipmap.h"
#include "Image/CImageManager.h"
#include "Image/CImageUtility.h"
#include "Localization/CLocalizationManager.h"
#include "CVector2ui16.h"

using namespace std;

CTXDManager::CTXDManager(void)
{
}

void					CTXDManager::init(void)
{
}
void					CTXDManager::uninit(void)
{
}

CTXDFormat*				CTXDManager::createFormat(void)
{
	CTXDFormat *pTXDFile = new CTXDFormat;
	pTXDFile->setDeviceId(0);

	CRWSection *pTextureDictionary = pTXDFile->addSection(RW_SECTION_TEXTURE_DICTIONARY, RW_VERSION_3_4_0_3); // todo - make RW version not fixed
	CRWSection *pTextureNative = pTextureDictionary->addSection(RW_SECTION_TEXTURE_NATIVE, RW_VERSION_3_4_0_3); // todo - make RW version not fixed
	CRWSection *pExtension1 = pTextureDictionary->addSection(RW_SECTION_EXTENSION, RW_VERSION_3_4_0_3); // todo - make RW version not fixed
	CRWSection *pExtension2 = pTXDFile->addSection(RW_SECTION_EXTENSION, RW_VERSION_3_4_0_3); // todo - make RW version not fixed

	return pTXDFile;
}

CTXDFormat*				CTXDManager::convertIntermediateTextureFileToTXDFile(CIntermediateTextureFormat *pGeneralTextureFile)
{
	CTXDFormat *pTXDFile = createFormat();
	pTXDFile->setRWVersion(CRWManager::getInstance()->getVersionManager()->getRWVersionFromGame(PLATFORMED_GAME_PC_GTA_SA));

	for (CIntermediateTexture *pGeneralTexture : pGeneralTextureFile->getEntries())
	{
		CRWSection_TextureNative *pTexture = new CRWSection_TextureNative;

		string strAlphaName = "";

		// struct TextureFormat
		pTexture->setPlatformId(9);
		pTexture->setFilterFlags(0);
		pTexture->setTextureWrapUV(0);
		pTexture->setHasDiffuse(true);
		pTexture->setDiffuseName(pGeneralTexture->getName());
		pTexture->setHasAlpha(false);
		pTexture->setAlphaName(strAlphaName);

		// struct RasterFormat
		eRasterDataFormat eRasterDataFormatValue = RASTERDATAFORMAT_BGRA32;
		pTexture->setRasterDataFormat(eRasterDataFormatValue);
		pTexture->setTXDRasterDataFormat(CTXDManager::getTXDRasterDataFormatFromRasterDataFormat(eRasterDataFormatValue));
		eDXTCompressionType eDXTValue = CImageUtility::getDXTCompressionTypeFromRasterDataFormat(eRasterDataFormatValue);
		if (pTexture->getPlatformId() == 9)
		{
			// GTA SA
			pTexture->setDXTCompressionType(eDXTValue);
			pTexture->setAlpha(0);
		}
		else
		{
			// GTA III & VC
			pTexture->setAlpha(0);
		}
		CVector2ui32 vecImageSize;
		vecImageSize.m_x = pGeneralTexture->getSize().m_x;
		vecImageSize.m_y = pGeneralTexture->getSize().m_y;
		pTexture->setImageSize(vecImageSize);
		pTexture->setBPP(CImageUtility::getBPPFromRasterDataFormat(eRasterDataFormatValue));
		pTexture->setRasterType(0);
		if (pTexture->getPlatformId() == 9)
		{
			// GTA SA
			uint8 ucSAData = 0;
			pTexture->setAlpha((bool)(ucSAData & 1));
			pTexture->setCubeTexture((bool)(ucSAData & 2));
			pTexture->setAutoMipMaps((bool)(ucSAData & 4));
			pTexture->setIsNotRWCompatible((bool)(ucSAData & 8));
		}
		else
		{
			// GTA III & VC
			pTexture->setDXTCompressionType(eDXTValue);
			pTexture->setCubeTexture(false);
			pTexture->setAutoMipMaps(false);
			pTexture->setIsNotRWCompatible(false);
		}

		for (CIntermediateTextureMipmap *pGeneralMipmap : pGeneralTexture->getEntries())
		{
			CRWEntry_TextureNative_MipMap *pMipmap = new CRWEntry_TextureNative_MipMap(pTexture);

			string strMipmapRasterData = pGeneralMipmap->getRasterData();
			strMipmapRasterData = CImageUtility::convertRasterDataFormatToBGRA32(strMipmapRasterData, pGeneralTexture->getRasterDataFormat(), pGeneralTexture->getPaletteData(), pGeneralMipmap->getSize().m_x, pGeneralMipmap->getSize().m_y);
			string strPaletteDataOut = "";
			strMipmapRasterData = CImageUtility::convertBGRA32ToRasterDataFormat(strMipmapRasterData, eRasterDataFormatValue, &strPaletteDataOut, pGeneralMipmap->getSize().m_x, pGeneralMipmap->getSize().m_y);
			
			CVector2ui32 vecMipMapSize;
			vecMipMapSize.m_x = pGeneralMipmap->getSize().m_x;
			vecMipMapSize.m_y = pGeneralMipmap->getSize().m_y;

			pMipmap->setImageSize(vecMipMapSize);
			pMipmap->setRasterData(strMipmapRasterData);

			pTexture->getMipMaps().addEntry(pMipmap);
		}

		pTXDFile->getTextures().push_back(pTexture);
	}

	return pTXDFile;
}

uint32					CTXDManager::getTXDRasterDataFormatFromRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	switch (eRasterDataFormatValue)
	{
	case RASTERDATAFORMAT_DXT1:
	case RASTERDATAFORMAT_DXT2:
	case RASTERDATAFORMAT_DXT3:
	case RASTERDATAFORMAT_DXT4:
	case RASTERDATAFORMAT_DXT5:
		return TXDRASTERDATAFORMAT_565;
	case RASTERDATAFORMAT_BGRA32:
	case RASTERDATAFORMAT_RGBA32:
		return TXDRASTERDATAFORMAT_8888;
	case RASTERDATAFORMAT_BGR32:
	case RASTERDATAFORMAT_RGB32:
	case RASTERDATAFORMAT_BGR24:
	case RASTERDATAFORMAT_RGB24:
		return TXDRASTERDATAFORMAT_888;
	case RASTERDATAFORMAT_PAL8:
		return TXDRASTERDATAFORMAT_8888 | TXDRASTERDATAFORMAT_EXT_PAL4;
	case RASTERDATAFORMAT_PAL4:
		return TXDRASTERDATAFORMAT_8888 | TXDRASTERDATAFORMAT_EXT_PAL8;
	}
	return TXDRASTERDATAFORMAT_DEFAULT;
}

string					CTXDManager::getTXDRasterFormatText(uint32 uiTXDRasterDataFormatValue, eDXTCompressionType eDXTCompressionTypeValue)
{
	switch (uiTXDRasterDataFormatValue & 0xFFB)
	{
	case TXDRASTERDATAFORMAT_8888:	return "8888";
	case TXDRASTERDATAFORMAT_888:	return "888";
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_555:
	case TXDRASTERDATAFORMAT_4444:
		switch (eDXTCompressionTypeValue)
		{
		case DXT_1:	return "DXT1";
		case DXT_2:	return "DXT2";
		case DXT_3:	return "DXT3";
		case DXT_4:	return "DXT4";
		case DXT_5:	return "DXT5";
		}
	}
	return CLocalizationManager::getInstance()->getTranslatedText("UnknownTXDRasterDataFormat");
}

ePlatform				CTXDManager::getPlatformFromTXDPlatformId(uint32 uiTXDPlatformId)
{
	switch (uiTXDPlatformId)
	{
	case 8: // PC - GTA III & GTA VC
	case 9: // PC - GTA SA
		return PLATFORM_PC;
	case 5: // Xbox - GTA III, GTA VC & GTA SA
		return PLATFORM_XBOX;
	case 12:
		return PLATFORM_ANDROID;
	case 0x00325350:
		return PLATFORM_PS2;
	}
	return _PLATFORM_UNKNOWN;
}