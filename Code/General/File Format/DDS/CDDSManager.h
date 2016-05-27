#ifndef CDDSManager_H
#define CDDSManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CDDSFormat;

class CDDSManager : public CFormatManager<CDDSFormat>, public CSingleton<CDDSManager>
{
public:
	CDDSManager(void);

	void						init(void);
	void						uninit(void);
};

#endif