#include "CRWEntry_2dEffect_Escalator.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CRWEntry_2dEffect_Escalator::CRWEntry_2dEffect_Escalator(void) :
	C2dEffect(_2DFX_ESCALATOR),
	m_vecEscalatorBottom({ 0.0f, 0.0f, 0.0f }),
	m_vecEscalatorTop({ 0.0f, 0.0f, 0.0f }),
	m_vecEscalatorEnd({ 0.0f, 0.0f, 0.0f }),
	m_uiDirection(0)
{
};

void							CRWEntry_2dEffect_Escalator::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	m_vecEscalatorBottom = pDataReader->readVector3D();
	m_vecEscalatorTop = pDataReader->readVector3D();
	m_vecEscalatorEnd = pDataReader->readVector3D();
	m_uiDirection = pDataReader->readUint32();
}

void							CRWEntry_2dEffect_Escalator::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->writeVector3D(m_vecEscalatorBottom);
	pDataWriter->writeVector3D(m_vecEscalatorTop);
	pDataWriter->writeVector3D(m_vecEscalatorEnd);
	pDataWriter->writeUint32(m_uiDirection);
}