#include "CRWSection_Material.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWSection_Material::CRWSection_Material(void) :
	m_uiUnknown1(0),
	m_uiColour(0),
	m_uiUnknown2(0),
	m_uiUsesTextures(0),
	m_fAmbient(0.0f),
	m_fSpecular(0.0f),
	m_fDiffuse(0.0f)
{
	setSectionId(RW_SECTION_MATERIAL);
}

void							CRWSection_Material::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_uiUnknown1 = pDataReader->readUint32();
	m_uiColour = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
	m_uiUsesTextures = pDataReader->readUint32();
	m_fAmbient = pDataReader->readFloat32();
	m_fSpecular = pDataReader->readFloat32();
	m_fDiffuse = pDataReader->readFloat32();
}

void							CRWSection_Material::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeUint32(m_uiUnknown1);
	pDataWriter->writeUint32(m_uiColour);
	pDataWriter->writeUint32(m_uiUnknown2);
	pDataWriter->writeUint32(m_uiUsesTextures);
	pDataWriter->writeFloat32(m_fAmbient);
	pDataWriter->writeFloat32(m_fSpecular);
	pDataWriter->writeFloat32(m_fDiffuse);
}