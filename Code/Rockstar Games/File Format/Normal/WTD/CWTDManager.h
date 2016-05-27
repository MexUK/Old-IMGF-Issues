#ifndef CWTDManager_H
#define CWTDManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include "CWTDFormat.h"
#include "d3d9.h"
#include <string>

class CWTDEntry;
class CGeneralTextureFile;

class CWTDManager : public CFormatManager<CWTDFormat>, public CSingleton<CWTDManager>
{
public:
	CWTDManager(void);
	
	void						init(void);
	void						uninit(void);

	static uint32				getImageDataSize(CWTDEntry *pWTDEntry, bool bIncludeLevels);

	CWTDFormat*					convertIntermediateTextureFileToWTDFile(CIntermediateTextureFormat *pGeneralTextureFile);
};

#endif