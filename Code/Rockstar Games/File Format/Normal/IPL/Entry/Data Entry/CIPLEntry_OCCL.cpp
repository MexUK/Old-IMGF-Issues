#include "CIPLEntry_OCCL.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_OCCL::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 7:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_vecMidPosition = pDataReader->readTokenVector2D();
		m_fBottomHeightZ = pDataReader->readTokenFloat32();
		m_vecWidth = pDataReader->readTokenVector2D();
		m_fHeight = pDataReader->readTokenFloat32();
		m_fRotation = pDataReader->readTokenFloat32();
		break;
	case 10:
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecMidPosition = pDataReader->readTokenVector2D();
		m_fBottomHeightZ = pDataReader->readTokenFloat32();
		m_vecWidth = pDataReader->readTokenVector2D();
		m_fHeight = pDataReader->readTokenFloat32();
		m_fRotation = pDataReader->readTokenFloat32();
		m_fUnknown1 = pDataReader->readTokenFloat32();
		m_fUnknown2 = pDataReader->readTokenFloat32();
		m_uiUnknown3 = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_OCCL::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecMidPosition);
		pDataWriter->writeToken(m_fBottomHeightZ);
		pDataWriter->writeToken(m_vecWidth);
		pDataWriter->writeToken(m_fHeight);
		pDataWriter->writeToken(m_fRotation);
		break;
	case 1:
		pDataWriter->writeToken(m_vecMidPosition);
		pDataWriter->writeToken(m_fBottomHeightZ);
		pDataWriter->writeToken(m_vecWidth);
		pDataWriter->writeToken(m_fHeight);
		pDataWriter->writeToken(m_fRotation);
		pDataWriter->writeToken(m_fUnknown1);
		pDataWriter->writeToken(m_fUnknown2);
		pDataWriter->writeToken(m_uiUnknown3);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}