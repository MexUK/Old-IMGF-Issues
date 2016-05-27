#include "CIPLEntry_PICK.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIPLEntry_PICK::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 4:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_uiWeaponId = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_PICK::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiWeaponId);
		pDataWriter->writeToken(m_vecPosition);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}