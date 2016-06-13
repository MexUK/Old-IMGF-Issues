#ifndef CWTDMipmap_H
#define CWTDMipmap_H

#include "Image/eRasterDataFormat.h"
#include <string>

class CWTDEntry;

class CWTDMipmap
{
public:
	CWTDMipmap(CWTDEntry *pWTDEntry);

	void						unload(void) {}

	void						setWTDEntry(CWTDEntry *pWTDEntry) { m_pWTDEntry = pWTDEntry; }
	CWTDEntry*					getWTDEntry(void) { return m_pWTDEntry; }

	void						setRasterData(std::string strRasterData) { m_strRasterData = strRasterData; }
	std::string					getRasterData(void) { return m_strRasterData; }

	void						setImageSize(bool bIsWidth, uint16 usImageSize) { m_usImageSize[bIsWidth ? 0 : 1] = usImageSize; }
	uint16				getImageSize(bool bIsWidth) { return m_usImageSize[bIsWidth ? 0 : 1]; }

	eRasterDataFormat			getRasterDataFormat(void);

	std::string					getRasterDataBGRA32(void);

private:
	CWTDEntry*					m_pWTDEntry;
	std::string					m_strRasterData;
	uint16				m_usImageSize[2];
};

#endif