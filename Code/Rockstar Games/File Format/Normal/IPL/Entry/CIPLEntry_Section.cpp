#include "CIPLEntry_Section.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "IPL/CIPLFormat.h"

void		CIPLEntry_Section::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	// setSectionType() has already been called, as the eIPLSectionType has already been detected.
}

void		CIPLEntry_Section::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeString(getFormat()->getSectionText(getSectionType()));
}