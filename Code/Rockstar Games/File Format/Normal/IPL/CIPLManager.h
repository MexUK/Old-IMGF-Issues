#ifndef CIPLManager_H
#define CIPLManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CIPLFormat;

class CIPLManager : public CFormatManager<CIPLFormat>, public CSingleton<CIPLManager>
{
public:
	CIPLManager(void);
	
	void					init(void);
	void					uninit(void);
};

#endif