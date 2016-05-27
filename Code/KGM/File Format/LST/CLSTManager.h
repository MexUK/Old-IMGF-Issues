#ifndef CLSTManager_H
#define CLSTManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include <string>

struct CLSTFormat;

class CLSTManager : public CFormatManager<CLSTFormat>, public CSingleton<CLSTManager>
{
public:
	CLSTManager(void);
	
	void				init(void);
	void				uninit(void);
};

#endif