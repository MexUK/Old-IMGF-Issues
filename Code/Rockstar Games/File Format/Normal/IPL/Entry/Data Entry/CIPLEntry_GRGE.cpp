#include "CIPLEntry_GRGE.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_GRGE::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 10:
	case 11:
		setFormatType(uiLineTokenCount == 10 ? 0 : 1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecLine = pDataReader->readTokenVector2D();
		m_vecCubePosition = pDataReader->readTokenVector3D();
		m_uiGarageFlags = pDataReader->readTokenUint32();
		m_uiGarageType = pDataReader->readTokenUint32();
		if (getFormatType() == 1)
		{
			m_strGarageName = pDataReader->readString();
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_GRGE::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
	case 1:
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_vecLine);
		pDataWriter->writeToken(m_vecCubePosition);
		pDataWriter->writeToken(m_uiGarageFlags);
		pDataWriter->writeToken(m_uiGarageType);
		if (getFormatType() == 1)
		{
			pDataWriter->writeToken(m_strGarageName);
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}