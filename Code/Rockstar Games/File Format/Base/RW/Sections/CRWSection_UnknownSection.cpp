#include "CRWSection_UnknownSection.h"
#include "Engine/RW/CRWManager.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_UnknownSection::CRWSection_UnknownSection(void) :
	m_uiActualSectionId(0)
{
	setSectionId(RW_SECTION_UNKNOWN);
}

void							CRWSection_UnknownSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiActualSectionId = m_eSectionId;
	m_strData = pDataReader->readString(m_uiSectionSize);
}

void							CRWSection_UnknownSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	uint32 uiRWVersionCC = CRWManager::getInstance()->getRWVersionCCForSerialization();

	pDataWriter->write(m_uiActualSectionId);
	pDataWriter->write((uint32)m_strData.length());
	pDataWriter->write(uiRWVersionCC);

	pDataWriter->write(m_strData);
}