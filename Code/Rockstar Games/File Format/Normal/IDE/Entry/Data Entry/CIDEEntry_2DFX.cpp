#include "CIDEEntry_2DFX.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_vecPosition = pDataReader->readVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_vecRotation = pDataReader->readVector4D();
	}
	else
	{
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readVector3D();
		m_vecColour.m_x = pDataReader->readTokenUint32();
		m_vecColour.m_y = pDataReader->readTokenUint32();
		m_vecColour.m_z = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_ui2dfxType = pDataReader->readTokenUint32();
	}
}

void			CIDEEntry_2DFX::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	if (doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		pDataWriter->write(m_strModelName);
		pDataWriter->write(m_vecPosition);
		pDataWriter->write(m_uiUnknown1);
		pDataWriter->write(m_vecRotation);
	}
	else
	{
		pDataWriter->write(m_uiObjectId);
		pDataWriter->write(m_vecPosition);
		pDataWriter->write(m_vecColour.m_x);
		pDataWriter->write(m_vecColour.m_y);
		pDataWriter->write(m_vecColour.m_z);
		pDataWriter->write(m_uiUnknown1);
		pDataWriter->write(m_ui2dfxType);
	}
}