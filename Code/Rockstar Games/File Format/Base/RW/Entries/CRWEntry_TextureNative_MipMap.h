#ifndef CRWEntry_TextureNative_MipMap_H
#define CRWEntry_TextureNative_MipMap_H

#include "Types.h"
#include "CSize2D.h"
#include "Image/eRasterDataFormat.h"
#include <string>

class CRWSection_TextureNative;

class CRWEntry_TextureNative_MipMap
{
public:
	CRWEntry_TextureNative_MipMap(CRWSection_TextureNative *pTextureNative) :
		m_pTextureNative(pTextureNative)
	{
		m_vecImageSize.m_x = 0;
		m_vecImageSize.m_y = 0;
		m_vecSwizzledImageSize.m_x = 0;
		m_vecSwizzledImageSize.m_y = 0;
	};

	void						unload(void) {}

	bool						canRasterDataBeConverted(void);
	eRasterDataFormat			getRasterDataFormat(void);
	std::string					getRasterDataBGRA32(void);
	std::string					getRasterDataRGBA32(void);

	void						convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue);

	void						setRasterData(std::string& strData) { m_strRasterData = strData; }
	std::string&				getRasterData(void) { return m_strRasterData; }

	void						setImageSize(CSize2D& vecImageSize) { m_vecImageSize = vecImageSize; }
	CSize2D&					getImageSize(void) { return m_vecImageSize; }

	void						setSwizzledImageSize(CSize2D& vecSwizzledImageSize) { m_vecSwizzledImageSize = vecSwizzledImageSize; }
	CSize2D&					getSwizzledImageSize(void) { return m_vecSwizzledImageSize; }

	void						setTexture(CRWSection_TextureNative *pTextureNative) { m_pTextureNative = pTextureNative; }
	CRWSection_TextureNative*	getTexture(void) { return m_pTextureNative; }

private:
	CRWSection_TextureNative*	m_pTextureNative;
	CSize2D						m_vecImageSize;
	CSize2D						m_vecSwizzledImageSize;
	std::string					m_strRasterData;
};

#endif