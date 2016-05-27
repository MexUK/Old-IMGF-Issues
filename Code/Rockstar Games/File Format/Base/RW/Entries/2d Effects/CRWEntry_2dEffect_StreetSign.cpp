#include "CRWEntry_2dEffect_StreetSign.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "String/CStringUtility.h"

CRWEntry_2dEffect_StreetSign::CRWEntry_2dEffect_StreetSign(void) :
	C2dEffect(_2DFX_STREET_SIGN),
	m_vecSize({ 0.0f, 0.0f }),
	m_vecRotation({ 0.0f, 0.0f, 0.0f }),
	m_uiFlags(0)
{
};

void							CRWEntry_2dEffect_StreetSign::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_vecSize = pDataReader->readVector2D();
	m_vecRotation = pDataReader->readVector3D();
	m_uiFlags = pDataReader->readUint32();
	for (uint32 i = 0; i < 4; i++)
	{
		m_strText[i] = CStringUtility::rtrimFromLeft(pDataReader->readString(16));
	}
}

void							CRWEntry_2dEffect_StreetSign::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write(m_vecSize);
	pDataWriter->write(m_vecRotation);
	pDataWriter->write(m_uiFlags);
	for (uint32 i = 0; i < 4; i++)
	{
		pDataWriter->write(CStringUtility::zeroPad(m_strText[i].substr(0, 16), 16)); // todo - use pDataWriter->write(std::string str, uint32 len)
	}
}