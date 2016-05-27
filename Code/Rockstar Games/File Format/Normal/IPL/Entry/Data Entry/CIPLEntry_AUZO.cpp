#include "CIPLEntry_AUZO.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_AUZO::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 9:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_strZoneName = pDataReader->readTokenString();
		m_uiSoundId = pDataReader->readTokenUint32();
		m_uiSwitch = pDataReader->readTokenUint32();
		m_vecLowerLeftPosition = pDataReader->readTokenVector3D();
		m_vecUpperRightPosition = pDataReader->readTokenVector3D();
		break;
	case 7:
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_strZoneName = pDataReader->readTokenString();
		m_uiSoundId = pDataReader->readTokenUint32();
		m_uiSwitch = pDataReader->readTokenUint32();
		m_vecCenterPosition = pDataReader->readTokenVector3D();
		m_fRadius = pDataReader->readTokenFloat32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_AUZO::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiSoundId);
		pDataWriter->writeToken(m_uiSwitch);
		pDataWriter->writeToken(m_vecLowerLeftPosition);
		pDataWriter->writeToken(m_vecUpperRightPosition);
		break;
	case 1:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiSoundId);
		pDataWriter->writeToken(m_uiSwitch);
		pDataWriter->writeToken(m_vecLowerLeftPosition);
		pDataWriter->writeToken(m_fRadius);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}