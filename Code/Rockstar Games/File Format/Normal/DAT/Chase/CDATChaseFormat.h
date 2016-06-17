#ifndef CDATChaseFormat_H
#define CDATChaseFormat_H

#include "Types.h"
#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "CDATChaseEntry.h"
#include <vector>

class CDATChaseFormat : public CFormat, public CVectorPool<CDATChaseEntry*>
{
public:
	CDATChaseFormat(void);
	
	void					unserialize(void);
	void					serialize(void);
};

#endif