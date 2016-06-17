#include "CDATAnimgrpSection.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

CDATAnimgrpSection::CDATAnimgrpSection(void) :
	m_pDATAnimgrpFormat(nullptr)
{
}

void					CDATAnimgrpSection::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	
	m_strWalkStyleName = pDataReader->readTokenString();
	m_strIFPFileName = pDataReader->readTokenString();
	m_strAnimationType = pDataReader->readTokenString();
	uint32 uiEntryCount = pDataReader->readTokenUint32();
}

void					CDATAnimgrpSection::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	
	pDataWriter->writeToken(m_strWalkStyleName);
	pDataWriter->writeToken(m_strIFPFileName);
	pDataWriter->writeToken(m_strAnimationType);
	pDataWriter->writeToken(getEntryCount());
}