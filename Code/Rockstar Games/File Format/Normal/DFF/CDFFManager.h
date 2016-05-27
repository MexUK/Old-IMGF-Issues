#ifndef CDFFManager_H
#define CDFFManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CDFFFormat;

class CDFFManager : public CFormatManager<CDFFFormat>, public CSingleton<CDFFManager>
{
public:
	CDFFManager(void);

	void						init(void);
	void						uninit(void);
};

#endif