#ifndef CCURManager_H
#define CCURManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CCURFormat;
class CBMPFormat;

class CCURManager : public CFormatManager<CCURFormat>, public CSingleton<CCURManager>
{
public:
	CCURManager(void);

	void						init(void);
	void						uninit(void);

	CCURFormat*					createFormatFromBMP(CBMPFormat *pBMPFormat);
};

#endif