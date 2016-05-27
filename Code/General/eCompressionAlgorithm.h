#ifndef eCompressionAlgorithm_H
#define eCompressionAlgorithm_H

#include "Types.h"

enum eCompressionAlgorithm : uint8
{
	IMGCOMPRESSION_UNKNOWN	= 255,
	IMGCOMPRESSION_NONE		= 0,
	IMGCOMPRESSION_ZLIB		= 1,
	IMGCOMPRESSION_LZ4		= 2
};

#endif