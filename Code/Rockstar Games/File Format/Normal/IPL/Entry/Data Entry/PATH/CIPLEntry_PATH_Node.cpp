#include "CIPLEntry_PATH_Node.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CIPLEntry_PATH_Group.h"

void			CIPLEntry_PATH_Node::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	if (getLatestPathGroup() == nullptr)
	{
		// path node found before path group
		throw EXCEPTION_INVALID_DATA_ORDER;
	}

	switch (pDataReader->getLineTokens().size())
	{
	case 12:
		CIPLEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_uiNodeType = pDataReader->readTokenUint32();
		m_uiNextNode = pDataReader->readTokenUint32();
		m_uiIsCrossRoad = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D() / 16.0f;
		m_fMedian = pDataReader->readTokenFloat32();
		m_uiLeftLaneCount = pDataReader->readTokenUint32();
		m_uiRightLaneCount = pDataReader->readTokenUint32();
		m_uiSpeedLimit = pDataReader->readTokenUint32();
		m_uiFlags = pDataReader->readTokenUint32();
		m_fSpawnRate = pDataReader->readTokenFloat32();
		break;
	}

	getLatestPathGroup()->addEntry(this);
}

void			CIPLEntry_PATH_Node::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		CIPLEntry_PATH::serialize();

		pDataWriter->writeUint32(m_uiNodeType);
		pDataWriter->writeUint32(m_uiNextNode);
		pDataWriter->writeUint32(m_uiIsCrossRoad);
		pDataWriter->writeVector3D(m_vecPosition * 16.0f);
		pDataWriter->writeFloat32(m_fMedian);
		pDataWriter->writeUint32(m_uiLeftLaneCount);
		pDataWriter->writeUint32(m_uiRightLaneCount);
		pDataWriter->writeUint32(m_uiSpeedLimit);
		pDataWriter->writeUint32(m_uiFlags);
		pDataWriter->writeFloat32(m_fSpawnRate);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}