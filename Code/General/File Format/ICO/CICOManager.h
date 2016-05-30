#ifndef CICOManager_H
#define CICOManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CICOFormat;
class CBMPFormat;

class CICOManager : public CFormatManager<CICOFormat>, public CSingleton<CICOManager>
{
public:
	CICOManager(void);

	void						init(void);
	void						uninit(void);

	CICOFormat*					createFormatFromBMP(CBMPFormat *pBMPFormat);
};

#endif