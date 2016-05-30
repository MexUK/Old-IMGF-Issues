#include "CCURManager.h"
#include "CCURFormat.h"

CCURManager::CCURManager(void)
{
}

void			CCURManager::init(void)
{
}
void			CCURManager::uninit(void)
{
}

CCURFormat*		CCURManager::createFormatFromBMP(CBMPFormat *pBMPFormat)
{
	CCURFormat *pCURFormat = new CCURFormat;
	pCURFormat->setBMPVersion(pBMPFormat->getBMPVersion());
	pCURFormat->setBPP(pBMPFormat->getBPP());
	pCURFormat->setColourPlaneCount(pBMPFormat->getColourPlaneCount());
	pCURFormat->setFileSize(pBMPFormat->getFileSize());
	pCURFormat->setFileType(pBMPFormat->getFileType());
	pCURFormat->setWidth(pBMPFormat->getWidth());
	pCURFormat->setHeight(pBMPFormat->getHeight());
	pCURFormat->setHasPalette(pBMPFormat->doesHavePalette());
	pCURFormat->setPaletteData(pBMPFormat->getPaletteData());
	pCURFormat->setRasterData(pBMPFormat->getRasterData());
	return pCURFormat;
}