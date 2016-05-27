#include "CIDEEntry_TXDP.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_TXDP::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:
		// GTA SA, GTA IV
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);
		m_strTXDName = pDataReader->readTokenString();
		m_strParentTXDName = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIDEEntry_TXDP::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strParentTXDName);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}