#include "CIDEEntry_2DFX_Explosion.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_Explosion::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiUnknown1 = pDataReader->readTokenUint32();
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_uiUnknown3 = pDataReader->readTokenUint32();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_Explosion::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown1);
	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_uiUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}