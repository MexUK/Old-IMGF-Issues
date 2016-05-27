#include "CIDEEntry_HIER.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_HIER::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 3:
		// GTA III, GTA VC, GTA SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIDEEntry_HIER::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}