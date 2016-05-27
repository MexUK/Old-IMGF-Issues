#include "CIDEEntry_2DFX_Light.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

void			CIDEEntry_2DFX_Light::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatType(1);
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
	else
	{
		setFormatType(0);

		CIDEEntry_2DFX::unserialize();
		
		m_strCoronaTexture = CStringUtility::trim(CStringUtility::trim(pDataReader->readTokenString(), "\""));
		m_strShadowTexture = CStringUtility::trim(CStringUtility::trim(pDataReader->readTokenString(), "\""));
		m_fViewDistance = pDataReader->readTokenFloat32();
		m_fOuterRange = pDataReader->readTokenFloat32();
		m_fCoronaSize = pDataReader->readTokenFloat32();
		m_fInnerRange = pDataReader->readTokenFloat32();
		m_uiIDEFlag = pDataReader->readTokenUint32();
		m_uiFlash = pDataReader->readTokenUint32();
		m_uiWet = pDataReader->readTokenUint32();
		m_uiFlare = pDataReader->readTokenUint32();
		m_uiDust = pDataReader->readTokenUint32();
	}
}

void			CIDEEntry_2DFX_Light::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		CIDEEntry_2DFX::serialize();

		pDataWriter->writeToken("\"" + m_strCoronaTexture + "\"");
		pDataWriter->writeToken("\"" + m_strShadowTexture + "\"");
		pDataWriter->writeToken(m_fViewDistance);
		pDataWriter->writeToken(m_fOuterRange);
		pDataWriter->writeToken(m_fCoronaSize);
		pDataWriter->writeToken(m_fInnerRange);
		pDataWriter->writeToken(m_uiIDEFlag);
		pDataWriter->writeToken(m_uiFlash);
		pDataWriter->writeToken(m_uiWet);
		pDataWriter->writeToken(m_uiFlare);
		pDataWriter->writeToken(m_uiDust);
		break;
	case 1:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}