#include "CIDEEntry_UnknownSection.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_UnknownSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	setLine(*pDataReader->getActiveLine());
}

void			CIDEEntry_UnknownSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeString(m_strLine);
}