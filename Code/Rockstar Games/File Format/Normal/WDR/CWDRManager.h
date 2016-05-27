#ifndef CWDRManager_H
#define CWDRManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include <string>

class CWDRFormat;
class CIntermediateModelFormat;

class CWDRManager : public CFormatManager<CWDRFormat>, public CSingleton<CWDRManager>
{
public:
	CWDRManager(void);

	void								init(void);
	void								uninit(void);

	static CWDRFormat*					convertIntermediateModelFileToWDRFile(CIntermediateModelFormat *pGeneralModelFile);

	static uint32						getPackedOffset(uint32 uiOffset);
	static uint32						getPackedDataOffset(uint32 uiOffset);
};

#endif