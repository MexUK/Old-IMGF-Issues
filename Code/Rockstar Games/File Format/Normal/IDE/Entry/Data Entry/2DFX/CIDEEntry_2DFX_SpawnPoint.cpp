#include "CIDEEntry_2DFX_SpawnPoint.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_SpawnPoint::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	m_vecFlags.m_x = pDataReader->readTokenUint32();
	m_vecFlags.m_y = pDataReader->readTokenUint32();
	m_vecFlags.m_z = pDataReader->readTokenUint32();
	m_vecFlags.m_w = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_SpawnPoint::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecFlags.m_x);
	pDataWriter->writeToken(m_vecFlags.m_y);
	pDataWriter->writeToken(m_vecFlags.m_z);
	pDataWriter->writeToken(m_vecFlags.m_w);
}