#include "CRWSection_Light.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_Light::CRWSection_Light(void)
{
	setSectionId(RW_SECTION_LIGHT);
}

void							CRWSection_Light::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_strData = pDataReader->readString(24);
}

void							CRWSection_Light::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeString(m_strData, 24);
}