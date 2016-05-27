#include "CRWEntry_2dEffect_ParticleEffect.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

CRWEntry_2dEffect_ParticleEffect::CRWEntry_2dEffect_ParticleEffect(void) :
	C2dEffect(_2DFX_PARTICLE)
{
};

void							CRWEntry_2dEffect_ParticleEffect::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_strParticleEffectName = CStringUtility::rtrimFromLeft(pDataReader->readString(24));
}

void							CRWEntry_2dEffect_ParticleEffect::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->write(m_strParticleEffectName, 24);
}