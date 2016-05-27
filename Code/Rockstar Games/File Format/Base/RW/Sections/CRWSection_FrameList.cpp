#include "CRWSection_FrameList.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_FrameList::CRWSection_FrameList(void) :
	m_uiFrameCount(0)
{
	setSectionId(RW_SECTION_FRAME_LIST);
}

void							CRWSection_FrameList::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiFrameCount = pDataReader->readUint32();
	m_strFrameData = pDataReader->readString(56 * m_uiFrameCount);
}

void							CRWSection_FrameList::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_uiFrameCount);
	pDataWriter->write(m_strFrameData);
}