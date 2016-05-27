#include "CIDEEntry_2DFX_Ladder.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_Ladder::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	m_vecLadderStartPosition = pDataReader->readVector3D();
	m_vecLadderEndPosition = pDataReader->readVector3D();
	m_vecClimbDirectionPosition = pDataReader->readVector3D();
	m_uiFlags = pDataReader->readTokenUint32();
}


void			CIDEEntry_2DFX_Ladder::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecLadderStartPosition);
	pDataWriter->writeToken(m_vecLadderEndPosition);
	pDataWriter->writeToken(m_vecClimbDirectionPosition);
	pDataWriter->writeToken(m_uiFlags);
}