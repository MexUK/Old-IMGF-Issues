#include "CIDEEntry_TREE.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_TREE::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			CIDEEntry_TREE::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}