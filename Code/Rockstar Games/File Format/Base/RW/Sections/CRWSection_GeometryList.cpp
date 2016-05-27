#include "CRWSection_GeometryList.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_GeometryList::CRWSection_GeometryList(void) :
	m_uiGeometryCount(0)
{
	setSectionId(RW_SECTION_GEOMETRY_LIST);
}

void							CRWSection_GeometryList::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiGeometryCount = pDataReader->readUint32();
}

void							CRWSection_GeometryList::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_uiGeometryCount);
}