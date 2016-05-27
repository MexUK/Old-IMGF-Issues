#include "CRWSection_Atomic.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_Atomic::CRWSection_Atomic(void) :
	m_uiFrameIndex(0),
	m_uiGeometryIndex(0),
	m_uiUnknown1(0),
	m_uiUnknown2(0)
{
	setSectionId(RW_SECTION_ATOMIC);
}

void							CRWSection_Atomic::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiFrameIndex = pDataReader->readUint32();
	m_uiGeometryIndex = pDataReader->readUint32();
	m_uiUnknown1 = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
}

void							CRWSection_Atomic::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_uiFrameIndex);
	pDataWriter->write(m_uiGeometryIndex);
	pDataWriter->write(m_uiUnknown1);
	pDataWriter->write(m_uiUnknown2);
}