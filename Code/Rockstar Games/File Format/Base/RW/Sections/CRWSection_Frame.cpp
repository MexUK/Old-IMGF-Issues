#include "CRWSection_Frame.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_Frame::CRWSection_Frame(void)
{
	setSectionId(RW_SECTION_FRAME);
}

void							CRWSection_Frame::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							CRWSection_Frame::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_strData);
}