#include "CDBEntry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

void				CDBEntry::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiEntryNameLength = pDataReader->readUint32();
	m_strEntryName = string((char*)pDataReader->readCString(uiEntryNameLength));
	m_uiEntrySize = pDataReader->readUint32();
	m_uiEntryDataCRC = pDataReader->readUint32();
	m_uiEntryCreationDate = pDataReader->readUint32();
	m_bHasIssue = (pDataReader->readUint8() & 1) == 1;
}

void				CDBEntry::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	pDataWriter->write((uint32)m_strEntryName.length());
	pDataWriter->write(m_strEntryName);
	pDataWriter->write(m_uiEntrySize);
	pDataWriter->write(m_uiEntryDataCRC);
	pDataWriter->write(m_uiEntryCreationDate);
	pDataWriter->write((uint8)(m_bHasIssue ? 1 : 0));
}