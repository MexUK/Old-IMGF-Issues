#ifndef CIntermediateModelFormat_H
#define CIntermediateModelFormat_H

#include "Pool/CVectorPool.h"
#include "CIntermediateModel.h"
#include <vector>

class CIntermediateModelFormat : public CVectorPool<CIntermediateModel*>
{
public:
	void								unload(void);
};

#endif