#include "CRWSection_Texture.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_Texture::CRWSection_Texture(void) :
	m_usFilterFlags(0),
	m_usUnknown1(0)
{
	setSectionId(RW_SECTION_TEXTURE);
}

void							CRWSection_Texture::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_usFilterFlags = pDataReader->readUint16();
	m_usUnknown1 = pDataReader->readUint16();
}

void							CRWSection_Texture::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_usFilterFlags);
	pDataWriter->write(m_usUnknown1);
}