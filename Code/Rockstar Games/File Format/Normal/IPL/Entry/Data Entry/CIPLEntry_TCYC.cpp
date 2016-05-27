#include "CIPLEntry_TCYC.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_TCYC::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 11:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPositionLower = pDataReader->readTokenVector3D();
		m_vecPositionUpper = pDataReader->readTokenVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiWeatherId = pDataReader->readTokenUint32();
		m_iBrightness = pDataReader->readTokenInt32();
		m_uiTime = pDataReader->readTokenUint32();
		m_uiDrawDistance = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_TCYC::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->write(m_vecPositionLower);
		pDataWriter->write(m_vecPositionUpper);
		pDataWriter->write(m_uiUnknown1);
		pDataWriter->write(m_uiWeatherId);
		pDataWriter->write(m_iBrightness);
		pDataWriter->write(m_uiTime);
		pDataWriter->write(m_uiDrawDistance);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}