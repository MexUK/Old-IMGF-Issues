#include "CRWEntry_2dEffect_PedAttractor.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

CRWEntry_2dEffect_PedAttractor::CRWEntry_2dEffect_PedAttractor(void) :
	C2dEffect(_2DFX_PED_ATTRACTOR),
	m_iType(0),
	m_iPedExistingProbability(0),
	m_ucUnknown1(0),
	m_ucNotUsed1(0),
	m_ucUnknown2(0),
	m_ucNotUsed2(0)
{
	m_vecRotation[0] = { 0.0f, 0.0f, 0.0f };
	m_vecRotation[1] = { 0.0f, 0.0f, 0.0f };
	m_vecRotation[2] = { 0.0f, 0.0f, 0.0f };
};

void							CRWEntry_2dEffect_PedAttractor::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_iType = pDataReader->readInt32();
	m_vecRotation[0] = pDataReader->readVector3D();
	m_vecRotation[1] = pDataReader->readVector3D();
	m_vecRotation[2] = pDataReader->readVector3D();
	m_strExternalScriptName = CStringUtility::rtrimFromLeft(pDataReader->readString(8));
	m_iPedExistingProbability = pDataReader->readInt32();
	m_ucUnknown1 = pDataReader->readUint8();
	m_ucNotUsed1 = pDataReader->readUint8();
	m_ucUnknown2 = pDataReader->readUint8();
	m_ucNotUsed2 = pDataReader->readUint8();
}

void							CRWEntry_2dEffect_PedAttractor::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->write(m_iType);
	pDataWriter->write(m_vecRotation[0]);
	pDataWriter->write(m_vecRotation[1]);
	pDataWriter->write(m_vecRotation[2]);
	pDataWriter->write(m_strExternalScriptName, 8);
	pDataWriter->write(m_iPedExistingProbability);
	pDataWriter->write(m_ucUnknown1);
	pDataWriter->write(m_ucNotUsed1);
	pDataWriter->write(m_ucUnknown2);
	pDataWriter->write(m_ucNotUsed2);
}