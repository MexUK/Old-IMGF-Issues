#include "CRasterDataFormatManager.h"
#include "CRasterDataFormat.h"

void			CRasterDataFormatManager::init(void)
{
	initRasterDataFormats();
}
void			CRasterDataFormatManager::uninit(void)
{
	removeAllEntries();
}

void			CRasterDataFormatManager::initRasterDataFormats(void)
{
	CRasterDataFormat *pRasterDataFormat = nullptr;

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("BGRA 32BPP");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGRA32);
	pRasterDataFormat->setLocalizationKey("TextureFormat_1");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("BGR 32BPP");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGR32);
	pRasterDataFormat->setLocalizationKey("TextureFormat_2");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("BGR 24BPP");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_BGR24);
	pRasterDataFormat->setLocalizationKey("TextureFormat_3");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("DXT1 (Compressed)");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT1);
	pRasterDataFormat->setLocalizationKey("TextureFormat_4");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("DXT3 (Compressed)");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT3);
	pRasterDataFormat->setLocalizationKey("TextureFormat_5");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("DXT5 (Compressed)");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_DXT5);
	pRasterDataFormat->setLocalizationKey("TextureFormat_6");
	addEntry(pRasterDataFormat);

	/*
	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("RGBA 4BPP");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_PAL4);
	pRasterDataFormat->setLocalizationKey("TextureFormat_7");
	addEntry(pRasterDataFormat);

	pRasterDataFormat = new CRasterDataFormat;
	pRasterDataFormat->setText("RGBA 8BPP");
	pRasterDataFormat->setRasterDataFormatId(RASTERDATAFORMAT_PAL8);
	pRasterDataFormat->setLocalizationKey("TextureFormat_8");
	addEntry(pRasterDataFormat);
	*/
}