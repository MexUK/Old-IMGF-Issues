#include "CIPLEntry_LINK.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_LINK::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIPLEntry_LINK::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}