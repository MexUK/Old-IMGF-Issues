#include "CRWEntry_TextureNative_MipMap.h"
#include "RW/Sections/CRWSection_TextureNative.h"
#include "String/CStringUtility.h"
#include "Image/CImageUtility.h"

using namespace std;

string				CRWEntry_TextureNative_MipMap::getRasterDataBGRA32(void)
{
	if (getRasterDataFormat() == RASTERDATAFORMAT_BGRA32)
	{
		// the mipmap's raster data is currently stored in format BGRA32
		return m_strRasterData;
	}
	else
	{
		// the mipmap's raster data is currently stored in a format that is not BGRA32
		return CImageUtility::convertRasterDataFormatToBGRA32(m_strRasterData, getRasterDataFormat(), getTexture()->getPaletteData(), getImageSize().m_x, getImageSize().m_y);
	}
}

string				CRWEntry_TextureNative_MipMap::getRasterDataRGBA32(void)
{
	return CImageUtility::convertBGRA32ToRGBA32(getRasterDataBGRA32());
}

void				CRWEntry_TextureNative_MipMap::convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue)
{
	if (getRasterDataFormat() == eRasterDataFormatValue)
	{
		// raster data is already in the requested format
		return;
	}

	// convert raster data to requested format
	string strPaletteData = "";
	setRasterData(CImageUtility::convertBGRA32ToRasterDataFormat(getRasterDataBGRA32(), eRasterDataFormatValue, &strPaletteData, getImageSize().m_x, getImageSize().m_y));
}

bool				CRWEntry_TextureNative_MipMap::canRasterDataBeConverted(void)
{
	if (getImageSize().m_x < 4 || getImageSize().m_y < 4)
	{
		return false;
	}

	return true;
}

eRasterDataFormat	CRWEntry_TextureNative_MipMap::getRasterDataFormat(void)
{
	return getTexture()->getRasterDataFormat();
}