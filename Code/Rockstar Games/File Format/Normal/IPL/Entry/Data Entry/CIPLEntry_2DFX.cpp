#include "CIPLEntry_2DFX.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_2DFX::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_2DFX::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}