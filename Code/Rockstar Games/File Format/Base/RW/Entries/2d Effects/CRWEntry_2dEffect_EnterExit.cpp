#include "CRWEntry_2dEffect_EnterExit.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

CRWEntry_2dEffect_EnterExit::CRWEntry_2dEffect_EnterExit(void) :
	C2dEffect(_2DFX_ENTER_EXIT),
	m_fEnterMarkerRotationAngle(0.0f),
	m_vecMarkerApporximationRadius({ 0.0f, 0.0f }),
	m_vecExitMarkerPosition({ 0.0f, 0.0f, 0.0f }),
	m_fExitMarkerAngleRotation(0.0f),
	m_iInteriorCount(0),
	m_iFlags(0),
	m_iSkyColour(0)
{
}

void							CRWEntry_2dEffect_EnterExit::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_fEnterMarkerRotationAngle = pDataReader->readFloat32();
	m_vecMarkerApporximationRadius = pDataReader->readVector2D();
	m_vecExitMarkerPosition = pDataReader->readVector3D();
	m_fExitMarkerAngleRotation = pDataReader->readFloat32();
	m_iInteriorCount = pDataReader->readInt8();
	m_iFlags = pDataReader->readInt8();
	m_strInteriorName = CStringUtility::rtrimFromLeft(pDataReader->readString(8));
	m_iSkyColour = pDataReader->readInt8();
}

void							CRWEntry_2dEffect_EnterExit::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->write(m_fEnterMarkerRotationAngle);
	pDataWriter->write(m_vecMarkerApporximationRadius);
	pDataWriter->write(m_vecExitMarkerPosition);
	pDataWriter->write(m_fExitMarkerAngleRotation);
	pDataWriter->write(m_iInteriorCount);
	pDataWriter->write(m_iFlags);
	pDataWriter->write(m_strInteriorName, 8);
	pDataWriter->write(m_iSkyColour);
}