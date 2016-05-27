#ifndef CBMPManager_H
#define CBMPManager_H

#include "CFormatManager.h"
#include "CSingleton.h"

class CBMPFormat;

class CBMPManager : public CFormatManager<CBMPFormat>, public CSingleton<CBMPManager>
{
public:
	CBMPManager(void);

	void						init(void);
	void						uninit(void);
};

#endif