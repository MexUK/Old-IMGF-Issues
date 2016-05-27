#include "CRWSection_MaterialList.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_MaterialList::CRWSection_MaterialList(void) :
	m_uiMaterialCount(0)
{
	setSectionId(RW_SECTION_MATERIAL_LIST);
}

void							CRWSection_MaterialList::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiMaterialCount = pDataReader->readUint32();
	m_strReservedValues = pDataReader->readString(4 * m_uiMaterialCount);
}

void							CRWSection_MaterialList::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_uiMaterialCount);
	pDataWriter->write(m_strReservedValues);
}