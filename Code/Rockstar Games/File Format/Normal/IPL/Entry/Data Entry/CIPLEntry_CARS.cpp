#include "CIPLEntry_CARS.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "IPL/CIPLFormat.h"

void			CIPLEntry_CARS::unserialize(void)
{
	if (getFormat()->isBinary())
	{
		unserialize_Binary();
	}
	else
	{
		unserialize_Text();
	}
}

void			CIPLEntry_CARS::unserialize_Text(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokenCount())
	{
	case 12:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPosition = pDataReader->readTokenVector3D();
		m_zRotation = pDataReader->readTokenFloat32();
		m_iVehicleModelId = pDataReader->readTokenInt32();
		m_iVehicleColour.m_x = pDataReader->readTokenInt32();
		m_iVehicleColour.m_y = pDataReader->readTokenInt32();
		m_uiForceSpawn = pDataReader->readTokenUint32();
		m_uiAlarmTriggerProbability = pDataReader->readTokenUint32();
		m_uiDoorLockProbability = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_CARS::unserialize_Binary(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	setFormatType(0);
	setFormatGames(GAME_FLAG_GTA_SA);

	m_vecPosition = pDataReader->readVector3D();
	m_zRotation = pDataReader->readFloat32();
	m_iVehicleModelId = pDataReader->readInt32();
	m_iVehicleColour.m_x = pDataReader->readInt32();
	m_iVehicleColour.m_y = pDataReader->readInt32();
	m_uiForceSpawn = pDataReader->readUint32();
	m_uiAlarmTriggerProbability = pDataReader->readUint32();
	m_uiDoorLockProbability = pDataReader->readUint32();
	m_uiUnknown1 = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
}

void			CIPLEntry_CARS::serialize(void)
{
	if (getFormat()->isBinary())
	{
		serialize_Binary();
	}
	else
	{
		serialize_Text();
	}
}

void			CIPLEntry_CARS::serialize_Text(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_zRotation);
		pDataWriter->writeToken(m_iVehicleModelId);
		pDataWriter->writeToken(m_iVehicleColour.m_x);
		pDataWriter->writeToken(m_iVehicleColour.m_y);
		pDataWriter->writeToken(m_uiForceSpawn);
		pDataWriter->writeToken(m_uiAlarmTriggerProbability);
		pDataWriter->writeToken(m_uiDoorLockProbability);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_CARS::serialize_Binary(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeFloat32(m_zRotation);
		pDataWriter->writeInt32(m_iVehicleModelId);
		pDataWriter->writeInt32(m_iVehicleColour.m_x);
		pDataWriter->writeInt32(m_iVehicleColour.m_y);
		pDataWriter->writeUint32(m_uiForceSpawn);
		pDataWriter->writeUint32(m_uiAlarmTriggerProbability);
		pDataWriter->writeUint32(m_uiDoorLockProbability);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_uiUnknown2);
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}