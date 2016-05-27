#include "CIDEEntry_PATH_Node.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CIDEEntry_PATH_Group.h"

void			CIDEEntry_PATH_Node::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	if (getLatestPathGroup() == nullptr)
	{
		// path node found before path group
		throw EXCEPTION_INVALID_DATA_ORDER;
	}

	switch (pDataReader->getLineTokens().size())
	{
	case 9:
		CIDEEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_uiNodeType = pDataReader->readTokenUint32();
		m_uiNextNode = pDataReader->readTokenUint32();
		m_uiIsCrossRoad = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readVector3D() / 16.0f;
		m_fUnknown1 = pDataReader->readTokenFloat32();
		m_uiLeftLaneCount = pDataReader->readTokenUint32();
		m_uiRightLaneCount = pDataReader->readTokenUint32();
		break;
	}

	getLatestPathGroup()->addEntry(this);
}

void			CIDEEntry_PATH_Node::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		CIDEEntry_PATH::serialize();

		pDataWriter->writeToken(m_uiNodeType);
		pDataWriter->writeToken(m_uiNextNode);
		pDataWriter->writeToken(m_uiIsCrossRoad);
		pDataWriter->writeToken(m_vecPosition * 16.0f);
		pDataWriter->writeToken(m_fUnknown1);
		pDataWriter->writeToken(m_uiLeftLaneCount);
		pDataWriter->writeToken(m_uiRightLaneCount);
		break;
	}
}