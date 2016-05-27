#ifndef CLSTProcessingManager_H
#define CLSTProcessingManager_H

#include "CManager.h"
#include <string>

struct CLSTFormat;

class CLSTProcessingManager : public CManager
{
public:
	void							init(void);
	void							uninit(void);

	void							process(CLSTFormat *pLSTFile);
};

#endif