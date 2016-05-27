#ifndef CMemoryUtility_H
#define CMemoryUtility_H

#include "Types.h"

class CMemoryUtility
{
public:
	static void					copyAndDeleteUint8Array(void* pDestination, uint8* pSource, uint64 uiByteCountToCopy);
};

#endif