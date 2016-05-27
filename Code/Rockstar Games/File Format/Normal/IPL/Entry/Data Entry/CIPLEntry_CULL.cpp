#include "CIPLEntry_CULL.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_CULL::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 11:
	{
		pDataReader->setPeek(true);
		pDataReader->setActiveLineTokenIndex(7);
		uint32 uiToken8 = pDataReader->readTokenUint32();
		pDataReader->setPeek(false);
		pDataReader->setActiveLineTokenIndex(0);

		if (uiToken8 == 0)
		{
			// GTA SA & GTA IV
			setFormatType(1);
			setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);

			m_vecCenterPosition = pDataReader->readTokenVector3D();
			m_uiUnknown1 = pDataReader->readTokenUint32();
			m_uiLength = pDataReader->readTokenUint32();
			m_uiBottom = pDataReader->readTokenUint32();
			m_uiWidth = pDataReader->readTokenUint32();
			m_uiUnknown2 = pDataReader->readTokenUint32();
			m_uiTop = pDataReader->readTokenUint32();
			m_uiFlags = pDataReader->readTokenUint32();
			m_uiUnknown3 = pDataReader->readTokenUint32();
		}
		else
		{
			// GTA III & GTA VC
			setFormatType(0);
			setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

			m_vecCenterPosition = pDataReader->readTokenVector3D();
			m_vecLowerLeftPosition = pDataReader->readTokenVector3D();
			m_vecUpperRightPosition = pDataReader->readTokenVector3D();
			m_uiFlags = pDataReader->readTokenUint32();
			m_uiUnknown1 = pDataReader->readTokenUint32();
		}
		break;
	}
	case 14: // GTA SA & GTA IV
		setFormatType(2);
		setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);

		m_vecCenterPosition = pDataReader->readTokenVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiLength = pDataReader->readTokenUint32();
		m_uiBottom = pDataReader->readTokenUint32();
		m_uiWidth = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_uiTop = pDataReader->readTokenUint32();
		m_uiFlags = pDataReader->readTokenUint32();
		m_vecMirrorParameters = pDataReader->readTokenVector4D();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_CULL::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0: // GTA 3, VC
		pDataWriter->writeToken(m_vecCenterPosition);
		pDataWriter->writeToken(m_vecLowerLeftPosition);
		pDataWriter->writeToken(m_vecUpperRightPosition);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiUnknown1);
		break;
	case 1: // SA format 1
		pDataWriter->writeToken(m_vecCenterPosition);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiLength);
		pDataWriter->writeToken(m_uiBottom);
		pDataWriter->writeToken(m_uiWidth);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiTop);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiUnknown3);
		break;
	case 2: // SA format 2
		pDataWriter->writeToken(m_vecCenterPosition);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiLength);
		pDataWriter->writeToken(m_uiBottom);
		pDataWriter->writeToken(m_uiWidth);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiTop);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_vecMirrorParameters);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}