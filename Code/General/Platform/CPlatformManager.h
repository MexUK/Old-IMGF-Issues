#ifndef CPlatformManager_H
#define CPlatformManager_H

#include "CManager.h"
#include "CSingleton.h"
#include "Pool/CVectorPool.h"
#include "CPlatform.h"
#include "ePlatform.h"
#include <string>

class CPlatformManager : public CManager, public CSingleton<CPlatformManager>, public CVectorPool<CPlatform*>
{
public:
	void											init(void);
	void											uninit(void);

	std::string										getPlatformName(ePlatform ePlatformValue);

private:
	void											initPlatforms(void);
	void											uninitPlatforms(void);
};

#endif