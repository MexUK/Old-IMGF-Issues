#include "CIPLEntry_INST.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "IPL/CIPLFormat.h"

void			CIPLEntry_INST::unserialize(void)
{
	if (getFormat()->isBinary())
	{
		unserialize_Binary();
	}
	else
	{
		unserialize_Text();
	}
}

void			CIPLEntry_INST::unserialize_Text(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokenCount())
	{
	case 12:
		// INST - GTA 3
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III);

		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecScale = pDataReader->readTokenVector3D();
		m_vecRotation = pDataReader->readTokenVector4D();
		break;
	case 13:
		// INST - VC
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_uiInterior = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecScale = pDataReader->readTokenVector3D();
		m_vecRotation = pDataReader->readTokenVector4D();
		break;
	case 11:
		// INST - SA
		setFormatType(2);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_uiInterior = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecRotation = pDataReader->readTokenVector4D();
		m_iLOD = pDataReader->readTokenInt32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_INST::unserialize_Binary(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	setFormatType(0);
	setFormatGames(GAME_FLAG_GTA_SA);

	m_vecPosition = pDataReader->readVector3D();
	m_vecRotation = pDataReader->readVector4D();
	m_uiObjectId = pDataReader->readUint32();
	m_uiInterior = pDataReader->readUint32();
	m_iLOD = pDataReader->readUint32();
}

void			CIPLEntry_INST::serialize(void)
{
	if (getFormat()->isBinary())
	{
		serialize_Binary();
	}
	else
	{
		serialize_Text();
	}
}

void			CIPLEntry_INST::serialize_Text(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0: // GTA III
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_vecScale);
		pDataWriter->writeToken(m_vecRotation);
		break;
	case 1: // GTA VC
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_uiInterior);
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_vecScale);
		pDataWriter->writeToken(m_vecRotation);
		break;
	case 2: // GTA SA
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_uiInterior);
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_vecRotation);
		pDataWriter->writeToken(m_iLOD);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_INST::serialize_Binary(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->write(m_vecPosition);
		pDataWriter->write(m_vecRotation);
		pDataWriter->write(m_uiObjectId);
		pDataWriter->write(m_uiInterior);
		pDataWriter->write(m_iLOD);
		break;
	}
}