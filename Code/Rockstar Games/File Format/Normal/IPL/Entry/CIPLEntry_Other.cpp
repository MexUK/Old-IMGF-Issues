#include "CIPLEntry_Other.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void		CIPLEntry_Other::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	setLine(*pDataReader->getActiveLine());
}

void		CIPLEntry_Other::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeString(getLine());
}