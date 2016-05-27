#include "CIPLEntry_MZON.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_MZON::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 10:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_IV);

		m_strZoneName = pDataReader->readTokenString();
		m_uiZoneType = pDataReader->readTokenUint32();
		m_vecBottomLeftPosition = pDataReader->readTokenVector3D();
		m_vecTopRightPosition = pDataReader->readTokenVector3D();
		m_uiLevel = pDataReader->readTokenUint32();
		m_strZoneText = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_MZON::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiZoneType);
		pDataWriter->writeToken(m_vecBottomLeftPosition);
		pDataWriter->writeToken(m_vecTopRightPosition);
		pDataWriter->writeToken(m_uiLevel);
		pDataWriter->writeToken(m_strZoneText);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}