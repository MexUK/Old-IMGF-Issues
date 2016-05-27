#include "CIDEEntry_OBJS.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_OBJS::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 4:
		// OBJS - type 0 (3, VC, SA)
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		break;
	case 6:
		// OBJS - type 1 (3, VC, SA)
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 7:
		// OBJS - type 2 (3, VC, SA)
		setFormatType(2);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 8:
		// OBJS - type 3 (3, VC, SA)
		setFormatType(3);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 5:
		// OBJS - type 4 (SA)
		setFormatType(4);
		setFormatGames(GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 16:
		// OBJS - type 5 (GTA IV) [type 1 for GTA IV]
		setFormatType(5);
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_vecDrawDistances.push_back(pDataReader->readTokenFloat32());
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_vecBoundingBoxLowerLeftCorner = pDataReader->readVector3D();
		m_vecBoundingBoxUpperRightCorner = pDataReader->readVector3D();
		m_vecBoundingSphereCenterPosition = pDataReader->readVector3D();
		m_fBoundingSphereRadius = pDataReader->readTokenFloat32();
		m_strLODModel = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIDEEntry_OBJS::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0: // type 0 - GTA 3, VC, SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		break;
	case 1: // type 1 - GTA 3, VC, SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 2: // type 2 - GTA 3, VC, SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_vecDrawDistances[1]);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 3: // type 3 - GTA 3, VC, SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_vecDrawDistances[1]);
		pDataWriter->writeToken(m_vecDrawDistances[2]);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 4: // type 4 - SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 5: // type 5 - GTA IV [type 1 for GTA IV]
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_vecBoundingBoxLowerLeftCorner);
		pDataWriter->writeToken(m_vecBoundingBoxUpperRightCorner);
		pDataWriter->writeToken(m_vecBoundingSphereCenterPosition);
		pDataWriter->writeToken(m_fBoundingSphereRadius);
		pDataWriter->writeToken(m_strLODModel);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}