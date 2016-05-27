#include "CIDEEntry_2DFX_WorldPoint.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_WorldPoint::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_2DFX_WorldPoint::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}