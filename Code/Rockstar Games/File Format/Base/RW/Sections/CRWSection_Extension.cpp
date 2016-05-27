#include "CRWSection_Extension.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void							CRWSection_Extension::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// no body data to read, contained RW sections are unserialized elsewhere
}

void							CRWSection_Extension::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// no body data to write
}