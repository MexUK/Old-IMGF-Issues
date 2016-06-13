#ifndef CIntermediateModel_H
#define CIntermediateModel_H

#include "Pool/CVectorPool.h"
#include "Data/CIntermediateGeometry.h"
#include <vector>

class CIntermediateModel : public CVectorPool<CIntermediateGeometry*>
{
public:
	void								unload(void) {}
};

#endif