#ifndef CDATScriptPathFormat_H
#define CDATScriptPathFormat_H

#include "Types.h"
#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "CDATScriptPathEntry.h"
#include <vector>

class CDATScriptPathFormat : public CFormat, public CVectorPool<CDATScriptPathEntry*>
{
public:
	CDATScriptPathFormat(void);
	
	void					unserialize(void);
	void					serialize(void); // todo
};

#endif