#include "CICOManager.h"
#include "CICOFormat.h"

CICOManager::CICOManager(void)
{
}

void			CICOManager::init(void)
{
}
void			CICOManager::uninit(void)
{
}

CICOFormat*		CICOManager::createFormatFromBMP(CBMPFormat *pBMPFormat)
{
	CICOFormat *pICOFormat = new CICOFormat;
	pICOFormat->setBMPVersion(pBMPFormat->getBMPVersion());
	pICOFormat->setBPP(pBMPFormat->getBPP());
	pICOFormat->setColourPlaneCount(pBMPFormat->getColourPlaneCount());
	pICOFormat->setFileSize(pBMPFormat->getFileSize());
	pICOFormat->setFileType(pBMPFormat->getFileType());
	pICOFormat->setWidth(pBMPFormat->getWidth());
	pICOFormat->setHeight(pBMPFormat->getHeight());
	pICOFormat->setHasPalette(pBMPFormat->doesHavePalette());
	pICOFormat->setPaletteData(pBMPFormat->getPaletteData());
	pICOFormat->setRasterData(pBMPFormat->getRasterData());
	return pICOFormat;
}