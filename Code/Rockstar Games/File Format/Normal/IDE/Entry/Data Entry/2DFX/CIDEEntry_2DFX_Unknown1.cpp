#include "CIDEEntry_2DFX_Unknown1.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_Unknown1::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_vecUnknown3 = pDataReader->readVector3D();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_Unknown1::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_vecUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}