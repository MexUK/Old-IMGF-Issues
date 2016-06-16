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
		pDataWriter->writeString(m_strModelName);
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeVector4D(m_vecRotation);
	}
	else
	{
		pDataWriter->writeUint32(m_uiObjectId);
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeVector3ui32(m_vecColour);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_ui2dfxType);
	}
}