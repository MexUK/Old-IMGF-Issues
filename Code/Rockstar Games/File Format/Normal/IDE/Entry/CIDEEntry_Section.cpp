#include "CIDEEntry_Section.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "IDE/CIDEFormat.h"

void		CIDEEntry_Section::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// setSectionType() has already been called, as the eIDESectionType has already been detected.
}

void		CIDEEntry_Section::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}