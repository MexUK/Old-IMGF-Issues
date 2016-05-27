#ifndef CTXDFormat_H
#define CTXDFormat_H

#include "RW/CRWFormat.h"
#include "RW/CRWSection.h"
#include "Game/ePlatformedGame.h"
#include "Image/eRasterDataFormat.h"
#include "Engine/RW/eRWVersion.h"
#include <string>
#include <vector>

class CRWSection_TextureNative;
class CIntermediateTextureFormat;

class CTXDFormat : public CRWFormat
{
public:
	CTXDFormat(void);

	CRWSection_TextureNative*					addTextureViaFile(std::string& strFilePath, std::string& strTextureDiffuseName, std::string strTextureAlphaName = "");
	std::vector<std::string>					getTextureNames(void);
	std::vector<CRWSection_TextureNative*>		getTextures(void);

	bool							doesHaveTextureWithInvalidTXDRasterDataFormat(void);

	void							convertToGame(ePlatformedGame ePlatformedGame, std::vector<std::string>& vecMipmapsRemoved);
	void							convertToRasterDataFormat(eRasterDataFormat eRasterDataFormatValue, std::vector<std::string>& vecMipmapsRemoved);
	CIntermediateTextureFormat*		convertToIntermediateFormat(void);

	static bool						isTextureNameValid(std::string& strTextureName, bool bIsAlphaTexture = false);
	static bool						isTextureResolutionValid(uint16 usWidth, uint16 usHeight, std::vector<ePlatformedGame>& vecGames);
	static bool						isTXDSizeValid(uint32 uiFileSize);
	static bool						isTextureCountValid(uint32 uiTextureCount, std::vector<ePlatformedGame>& vecGames);
	static uint32					getMaxTextureCountForGame(ePlatformedGame ePlatformedGame);

	void							setGames(std::vector<ePlatformedGame>& vecGames) { m_vecGames = vecGames; }
	std::vector<ePlatformedGame>&	getGames(void) { return m_vecGames; }

	void							setDeviceId(uint16 usDeviceId) { m_usDeviceId = usDeviceId; }
	uint16							getDeviceId(void) { return m_usDeviceId; }

private:
	std::vector<ePlatformedGame>	m_vecGames;
	uint16							m_usDeviceId;
};

#endif