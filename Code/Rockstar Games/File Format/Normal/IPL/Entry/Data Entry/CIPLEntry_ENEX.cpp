#include "CIPLEntry_ENEX.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_ENEX::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 18:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecEntrancePosition = pDataReader->readTokenVector3D();
		m_fRotation = pDataReader->readTokenFloat32();
		m_vecSize = pDataReader->readTokenVector2D();
		m_uiConstant8 = pDataReader->readTokenUint32();
		m_vecExitPosition = pDataReader->readTokenVector3D();
		m_fExitRotation = pDataReader->readTokenFloat32();
		m_uiTargetInterior = pDataReader->readTokenUint32();
		m_uiMarkerType = pDataReader->readTokenUint32();
		m_strInteriorName = pDataReader->readTokenString();
		m_uiSkyColourChanger = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_ENEX::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecEntrancePosition);
		pDataWriter->writeToken(m_fRotation);
		pDataWriter->writeToken(m_vecSize);
		pDataWriter->writeToken(m_uiConstant8);
		pDataWriter->writeToken(m_vecExitPosition);
		pDataWriter->writeToken(m_fExitRotation);
		pDataWriter->writeToken(m_uiTargetInterior);
		pDataWriter->writeToken(m_uiMarkerType);
		pDataWriter->writeToken(m_strInteriorName);
		pDataWriter->writeToken(m_uiSkyColourChanger);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}