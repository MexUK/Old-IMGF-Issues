#include "CDATAnimgrpEntry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CDATAnimgrpEntry::CDATAnimgrpEntry(void) :
	m_pDATAnimgrpSection(nullptr)
{
}

void					CDATAnimgrpEntry::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_strAnimationName = pDataReader->readTokenString();
}

void					CDATAnimgrpEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->writeToken(m_strAnimationName);
}