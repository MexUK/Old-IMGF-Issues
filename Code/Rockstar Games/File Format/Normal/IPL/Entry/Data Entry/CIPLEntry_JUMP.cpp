#include "CIPLEntry_JUMP.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_JUMP::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 16:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecStartLowerPosition = pDataReader->readTokenVector3D();
		m_vecStartUpperPosition = pDataReader->readTokenVector3D();
		m_vecTargetLowerPosition = pDataReader->readTokenVector3D();
		m_vecTargetUpperPosition = pDataReader->readTokenVector3D();
		m_vecCameraPosition = pDataReader->readTokenVector3D();
		m_uiReward = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_JUMP::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecStartLowerPosition);
		pDataWriter->writeToken(m_vecStartUpperPosition);
		pDataWriter->writeToken(m_vecTargetLowerPosition);
		pDataWriter->writeToken(m_vecTargetUpperPosition);
		pDataWriter->writeToken(m_vecCameraPosition);
		pDataWriter->writeToken(m_uiReward);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}