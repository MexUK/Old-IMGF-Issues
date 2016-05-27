#include "CMemoryUtility.h"
#include <cstring>

void					CMemoryUtility::copyAndDeleteUint8Array(void* pDestination, uint8* pSource, uint64 uiByteCountToCopy)
{
	memcpy(pDestination, pSource, uiByteCountToCopy);
	delete[] pSource;
}