#include "CDATScriptPathEntry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CDATScriptPathEntry::CDATScriptPathEntry(void)
{
	m_vecPosition.m_x = 0.0f;
	m_vecPosition.m_y = 0.0f;
	m_vecPosition.m_y = 0.0f;
}

void					CDATScriptPathEntry::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_vecPosition = pDataReader->readTokenVector3D();
}

void					CDATScriptPathEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->writeToken(m_vecPosition);
}