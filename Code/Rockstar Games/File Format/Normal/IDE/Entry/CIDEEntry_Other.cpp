#include "CIDEEntry_Other.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void		CIDEEntry_Other::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	setLine(*pDataReader->getActiveLine());
}

void		CIDEEntry_Other::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(getLine());
}