#ifndef CTXDManager_H
#define CTXDManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include "eTXDRasterDataFormat.h"
#include "Image/eRasterDataFormat.h"
#include "Game/ePlatformedGame.h"
#include "Image/eDXTCompressionType.h"
#include "Platform/ePlatform.h"
#include <string>
#include <vector>

class CTXDFormat;
class CTexture;
class CIntermediateTextureFormat;

class CTXDManager : public CFormatManager<CTXDFormat>, public CSingleton<CTXDManager>
{
public:
	CTXDManager(void);

	void						init(void);
	void						uninit(void);

	CTXDFormat*					createFormat(void);

	CTXDFormat*					convertIntermediateTextureFileToTXDFile(CIntermediateTextureFormat *pGeneralTextureFile);

	static uint32				getTXDRasterDataFormatFromRasterDataFormat(eRasterDataFormat eRasterDataFormatValue);
	static std::string			getTXDRasterFormatText(uint32 uiTXDRasterDataFormatValue, eDXTCompressionType eDXTCompressionTypeValue);
	static ePlatform			getPlatformFromTXDPlatformId(uint32 uiTXDPlatformId);
};

#endif