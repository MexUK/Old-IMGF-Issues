#ifndef CIntermediateTexture_H
#define CIntermediateTexture_H

#include "Types.h"
#include "Pool/CVectorPool.h"
#include "Image/eRasterDataFormat.h"
#include "Data/CIntermediateTextureMipmap.h"
#include "CVector2ui16.h"
#include <string>

class CIntermediateTexture : public CVectorPool<CIntermediateTextureMipmap*>
{
public:
	CIntermediateTexture(void) :
		m_eRasterDataFormat(RASTERDATAFORMAT_UNKNOWN)
	{
		m_vecSize.m_x = 0;
		m_vecSize.m_y = 0;
	};

	void						unload(void) {};

	void						setName(std::string& strName) { m_strName = strName; }
	std::string&				getName(void) { return m_strName; }

	void						setRasterDataFormat(eRasterDataFormat eRasterDataFormatValue) { m_eRasterDataFormat = eRasterDataFormatValue; }
	eRasterDataFormat			getRasterDataFormat(void) { return m_eRasterDataFormat; }

	void						setSize(CVector2ui16& vecSize) { m_vecSize = vecSize; }
	CVector2ui16&				getSize(void) { return m_vecSize; }

	void						setPaletteData(std::string& strPaletteData) { m_strPaletteData = strPaletteData; }
	std::string&				getPaletteData(void) { return m_strPaletteData; }

private:
	std::string					m_strName;
	eRasterDataFormat			m_eRasterDataFormat;
	CVector2ui16				m_vecSize;
	std::string					m_strPaletteData;
};

#endif