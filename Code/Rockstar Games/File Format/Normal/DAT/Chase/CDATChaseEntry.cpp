#include "CDATChaseEntry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CDATChaseEntry::CDATChaseEntry(void) :
	m_pFormat(nullptr),
	m_iSteeringAngle(0),
	m_iGasPedalPower(0),
	m_iBrakePedalPower(0),
	m_iHandbrakeUsed(0)
{
	m_vecVelocity.m_x = 0;
	m_vecVelocity.m_y = 0;
	m_vecVelocity.m_z = 0;
	m_vecRight.m_x = 0;
	m_vecRight.m_y = 0;
	m_vecRight.m_z = 0;
	m_vecTop.m_x = 0;
	m_vecTop.m_y = 0;
	m_vecTop.m_z = 0;
	m_vecPosition.m_x = 0.0f;
	m_vecPosition.m_y = 0.0f;
	m_vecPosition.m_z = 0.0f;
}

void					CDATChaseEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	/*
	todo
	pDataWriter->write(pDATChaseEntry_Raw->m_vecVelocity);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecRight);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecTop);
	pDataWriter->write(pDATChaseEntry_Raw->m_iSteeringAngle);
	pDataWriter->write(pDATChaseEntry_Raw->m_iGasPedalPower);
	pDataWriter->write(pDATChaseEntry_Raw->m_iBrakePedalPower);
	pDataWriter->write(pDATChaseEntry_Raw->m_iHandbrakeUsed);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecPosition);
	*/
}