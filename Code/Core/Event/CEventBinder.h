#ifndef CEventBinder_H
#define CEventBinder_H

#include "Pool/CVectorPool.h"
#include "CEventBoundFunction.h"

class CEventBinder
{
public:
	virtual void							bindEvents(void) = 0;
	void									unbindEvents(void);
	void									storeEventBoundFunction(CEventBoundFunction *pEventBoundFunction);

private:
	CVectorPool<CEventBoundFunction*>		m_vecEventBoundFunctions;
};

#endif