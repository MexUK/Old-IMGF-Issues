#ifndef CImageManager_H
#define CImageManager_H

#include "CManager.h"
#include "CSingleton.h"
#include <string>

class CRasterDataFormatManager;
class CImageFile;

class CImageManager : public CManager, public CSingleton<CImageManager>
{
public:
	CImageManager(void);
	~CImageManager(void);

	void								init(void);
	void								uninit(void);

	CRasterDataFormatManager*			getRasterDataFormatManager(void) { return m_pRasterDataFormatManager; }

	static CImageFile*					loadImageFromFile(std::string& strFilePath);

private:
	CRasterDataFormatManager*			m_pRasterDataFormatManager;
};

#endif