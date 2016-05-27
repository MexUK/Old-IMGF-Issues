#include "CRWEntry_2dEffect_Light.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

CRWEntry_2dEffect_Light::CRWEntry_2dEffect_Light(void) :
	C2dEffect(_2DFX_LIGHT),
	m_uiColor(0),
	m_fCoronaFarClip(0.0f),
	m_fPointlightRange(0.0f),
	m_fCoronaSize(0.0f),
	m_fShadowSize(0.0f),
	m_ucCoronaShowMode(0),
	m_ucCoronaEnableReflection(0),
	m_ucCoronaFlareType(0),
	m_ucShadowColorMultiplier(0),
	m_ucFlags1(0),
	m_ucShadowZDistance(0),
	m_ucFlags2(0)
{
	m_vecLookDirection.m_x = 0;
	m_vecLookDirection.m_y = 0;
	m_vecLookDirection.m_z = 0;
	m_vecPadding.m_x = 0;
	m_vecPadding.m_y = 0;
};

void							CRWEntry_2dEffect_Light::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_uiColor = pDataReader->readUint32();
	m_fCoronaFarClip = pDataReader->readFloat32();
	m_fPointlightRange = pDataReader->readFloat32();
	m_fCoronaSize = pDataReader->readFloat32();
	m_fShadowSize = pDataReader->readFloat32();
	m_ucCoronaShowMode = pDataReader->readUint8();
	m_ucCoronaEnableReflection = pDataReader->readUint8();
	m_ucCoronaFlareType = pDataReader->readUint8();
	m_ucShadowColorMultiplier = pDataReader->readUint8();
	m_ucFlags1 = pDataReader->readUint8();
	m_strCoronaTexName = CStringUtility::rtrimFromLeft(pDataReader->readString(24));
	m_strShadowTexName = CStringUtility::rtrimFromLeft(pDataReader->readString(24));
	m_ucShadowZDistance = pDataReader->readUint8();
	m_ucFlags2 = pDataReader->readUint8();
	if (m_uiDataSize == 76)
	{
		m_vecPadding.m_x = pDataReader->readUint8();
		m_vecPadding.m_y = 0;
	}
	else if (m_uiDataSize == 80)
	{
		m_vecLookDirection.m_x = pDataReader->readUint8();
		m_vecLookDirection.m_y = pDataReader->readUint8();
		m_vecLookDirection.m_z = pDataReader->readUint8();
		m_vecPadding.m_x = pDataReader->readUint8();
		m_vecPadding.m_y = pDataReader->readUint8();
	}
}

void							CRWEntry_2dEffect_Light::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->write(m_uiColor);
	pDataWriter->write(m_fCoronaFarClip);
	pDataWriter->write(m_fPointlightRange);
	pDataWriter->write(m_fCoronaSize);
	pDataWriter->write(m_fShadowSize);
	pDataWriter->write(m_ucCoronaShowMode);
	pDataWriter->write(m_ucCoronaEnableReflection);
	pDataWriter->write(m_ucCoronaFlareType);
	pDataWriter->write(m_ucShadowColorMultiplier);
	pDataWriter->write(m_ucFlags1);
	pDataWriter->write(m_strCoronaTexName, 24);
	pDataWriter->write(m_strShadowTexName, 24);
	pDataWriter->write(m_ucShadowZDistance);
	pDataWriter->write(m_ucFlags2);
	if (m_uiDataSize == 76)
	{
		pDataWriter->write(m_vecPadding.m_x);
	}
	else if (m_uiDataSize == 80)
	{
		pDataWriter->write(m_vecLookDirection.m_x);
		pDataWriter->write(m_vecLookDirection.m_y);
		pDataWriter->write(m_vecLookDirection.m_z);
		pDataWriter->write(m_vecPadding.m_x);
		pDataWriter->write(m_vecPadding.m_y);
	}
}