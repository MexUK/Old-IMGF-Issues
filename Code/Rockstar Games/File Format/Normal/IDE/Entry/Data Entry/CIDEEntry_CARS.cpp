#include "CIDEEntry_CARS.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

void			CIDEEntry_CARS::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 12: // GTA III
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strVehicleType = pDataReader->readTokenString();
		m_strHandlingId = pDataReader->readTokenString();
		m_strGameName = pDataReader->readTokenString();
		m_strVehicleClass = pDataReader->readTokenString();
		m_uiSpawnFrequency = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_uiWheelModelId = pDataReader->readTokenUint32();
		m_fWheelScale = pDataReader->readTokenFloat32();
		break;
	case 13: // GTA VC
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_VC);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strVehicleType = pDataReader->readTokenString();
		m_strHandlingId = pDataReader->readTokenString();
		m_strGameName = pDataReader->readTokenString();
		m_strAnimationFile = pDataReader->readTokenString();
		m_strVehicleClass = pDataReader->readTokenString();
		m_uiSpawnFrequency = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_uiWheelModelId = pDataReader->readTokenUint32();
		m_fWheelScale = pDataReader->readTokenFloat32();
		break;
	case 15: // GTA SA or GTA IV
	{
		pDataReader->setPeek(true);
		bool bIsSA = CStringUtility::isPositiveInteger(pDataReader->readTokenString());
		pDataReader->setPeek(false);
		if (bIsSA)
		{
			// GTA SA
			setFormatType(2);
			setFormatGames(GAME_FLAG_GTA_SA);
			m_uiObjectId = pDataReader->readTokenUint32();
			m_strModelName = pDataReader->readTokenString();
			m_strTXDName = pDataReader->readTokenString();
			m_strVehicleType = pDataReader->readTokenString();
			m_strHandlingId = pDataReader->readTokenString();
			m_strGameName = pDataReader->readTokenString();
			m_strAnimationFile = pDataReader->readTokenString();
			m_strVehicleClass = pDataReader->readTokenString();
			m_uiSpawnFrequency = pDataReader->readTokenUint32();
			m_uiUnknown1 = pDataReader->readTokenUint32();
			m_uiUnknown2 = pDataReader->readTokenUint32();
			m_uiWheelModelId = pDataReader->readTokenUint32();
			m_fWheelScaleFront = pDataReader->readTokenFloat32();
			m_fWheelScaleRear = pDataReader->readTokenFloat32();
			m_uiUnknown3 = pDataReader->readTokenUint32();
		}
		else
		{
			// GTA IV
			setFormatType(3);
			setFormatGames(GAME_FLAG_GTA_IV);
			m_strModelName = pDataReader->readTokenString();
			m_strTXDName = pDataReader->readTokenString();
			m_strVehicleType = pDataReader->readTokenString();
			m_strHandlingId = pDataReader->readTokenString();
			m_strGameName = pDataReader->readTokenString();
			m_strAnimationFile = pDataReader->readTokenString();
			m_strAnimationFile2 = pDataReader->readTokenString();
			m_uiSpawnFrequency = pDataReader->readTokenUint32();
			m_uiMaxVehicleCountAtOneTime = pDataReader->readTokenUint32();
			m_vecWheelRadius = pDataReader->readVector2D();
			m_fDirtLevel = pDataReader->readTokenFloat32();
			m_uiLODMultiplier = pDataReader->readTokenUint32();
			m_fSwankness = pDataReader->readTokenFloat32();
			m_uiFlags = pDataReader->readTokenUint32();
		}
		break;
	}
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIDEEntry_CARS::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScale);
		break;
	case 1:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScale);
		break;
	case 2:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScaleFront);
		pDataWriter->writeToken(m_fWheelScaleRear);
		pDataWriter->writeToken(m_uiUnknown3);
		break;
	case 3:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strAnimationFile2);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiMaxVehicleCountAtOneTime);
		pDataWriter->writeToken(m_vecWheelRadius);
		pDataWriter->writeToken(m_fDirtLevel);
		pDataWriter->writeToken(m_uiLODMultiplier);
		pDataWriter->writeToken(m_fSwankness);
		pDataWriter->writeToken(m_uiFlags);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}