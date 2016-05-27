#include "CIDEEntry_2DFX_Particle.h"
#include "eExceptionCode.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

void			CIDEEntry_2DFX_Particle::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	CIDEEntry_2DFX::unserialize();
	
	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatType(1);
		m_strParticleName = pDataReader->readTokenString();
		m_uiDestructionStageCount = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_fSize = pDataReader->readTokenFloat32();
		m_uiDrawDistance = pDataReader->readTokenUint32();
		m_uiUnknown3 = pDataReader->readTokenUint32();
		m_vecColour2.m_x = pDataReader->readTokenUint32();
		m_vecColour2.m_y = pDataReader->readTokenUint32();
		m_vecColour2.m_z = pDataReader->readTokenUint32();
	}
	else
	{
		setFormatType(0);
		m_uiParticleType = pDataReader->readTokenUint32();
		m_vecStrength = pDataReader->readVector3D();
		m_fSize = pDataReader->readTokenFloat32();
	}
}

void			CIDEEntry_2DFX_Particle::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	CIDEEntry_2DFX::serialize();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strParticleName);
		pDataWriter->writeToken(m_uiDestructionStageCount);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_fSize);
		pDataWriter->writeToken(m_uiDrawDistance);
		pDataWriter->writeToken(m_uiUnknown3);
		pDataWriter->writeToken(m_vecColour2.m_x);
		pDataWriter->writeToken(m_vecColour2.m_y);
		pDataWriter->writeToken(m_vecColour2.m_z);
		break;
	case 1:
		pDataWriter->writeToken(m_uiParticleType);
		pDataWriter->writeToken(m_vecStrength);
		pDataWriter->writeToken(m_fSize);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}