#include "CIDEEntry_TOBJ.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_TOBJ::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 4: // Type 0: GTA III, VC, SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		break;
	case 8: // Type 1: GTA III, VC, SA
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances = {
			pDataReader->readTokenFloat32()
		};
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	case 9: // Type 2: GTA III, VC, SA
		setFormatType(2);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances = {
			pDataReader->readTokenFloat32(),
			pDataReader->readTokenFloat32()
		};
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	case 10: // Type 3: GTA III, VC, SA
		setFormatType(3);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_vecDrawDistances = {
			pDataReader->readTokenFloat32(),
			pDataReader->readTokenFloat32(),
			pDataReader->readTokenFloat32()
		};
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	case 7: // Type 4: GTA SA
		setFormatType(4);
		setFormatGames(GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_vecDrawDistances = {
			pDataReader->readTokenFloat32()
		};
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	case 17:
		// TOBJ - type 5: GTA IV [type 1 for GTA IV]
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
		m_uiNightFlags = pDataReader->readTokenUint32();
		break;
	default:
	{
		uint32 uiLineTokenCount = pDataReader->getLineTokenCount();
		uint32 uiTokenIndex = 4; // starting at 1, so it can be compared directly with line token count

		setFormatType(6);
		setTokenCount(uiLineTokenCount);

		// first 4 tokens
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();

		// draw distances
		if (uiLineTokenCount < (uiTokenIndex + m_uiClumpCount))
		{
			throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
		}
		m_vecDrawDistances.resize(m_uiClumpCount);
		for (uint32 i = 0; i < m_uiClumpCount; i++)
		{
			m_vecDrawDistances[i] = pDataReader->readTokenFloat32();;
		}
		uiTokenIndex += m_uiClumpCount;

		// flags
		if (uiLineTokenCount < (uiTokenIndex + 1))
		{
			throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
		}
		m_uiFlags = pDataReader->readTokenUint32();
		uiTokenIndex++;

		if (uiLineTokenCount >= (uiTokenIndex + 1))
		{
			// time on - optional
			m_uiTimeOn = pDataReader->readTokenUint32();
			uiTokenIndex++;

			if (uiLineTokenCount >= (uiTokenIndex + 1))
			{
				// time off - optional
				m_uiTimeOff = pDataReader->readTokenUint32();
				uiTokenIndex++;
			}
		}
		break;
	}
	}
}

void			CIDEEntry_TOBJ::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiUnknown2);
		break;
	case 1:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	case 2:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_vecDrawDistances[1]);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	case 3:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_vecDrawDistances[1]);
		pDataWriter->writeToken(m_vecDrawDistances[2]);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	case 4:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_vecDrawDistances[0]);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	case 5:
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
		pDataWriter->writeToken(m_uiNightFlags);
		break;
	case 6:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_uiClumpCount);
		for (auto fDrawDistance : m_vecDrawDistances)
		{
			pDataWriter->writeToken(fDrawDistance);
		}
		pDataWriter->writeToken(m_uiFlags);

		if (m_uiTokenCount >= (6 + m_vecDrawDistances.size()))
		{
			pDataWriter->writeToken(m_uiTimeOn);
			if (m_uiTokenCount >= (7 + m_vecDrawDistances.size()))
			{
				pDataWriter->writeToken(m_uiTimeOff);
			}
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}