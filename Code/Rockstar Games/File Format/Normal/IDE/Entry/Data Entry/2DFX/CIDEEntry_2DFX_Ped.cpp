#include "CIDEEntry_2DFX_Ped.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_Ped::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiBehaviour = pDataReader->readTokenUint32();
	m_vecUnknown2 = pDataReader->readVector3D();
	m_vecPedRotation = pDataReader->readVector3D();
}

void			CIDEEntry_2DFX_Ped::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiBehaviour);
	pDataWriter->writeToken(m_vecUnknown2);
	pDataWriter->writeToken(m_vecPedRotation);
}