#include "CIDEEntry_WEAP.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_WEAP::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 7:
		// GTA VC, GTA SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 6:
		// GTA IV
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIDEEntry_WEAP::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 1:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}