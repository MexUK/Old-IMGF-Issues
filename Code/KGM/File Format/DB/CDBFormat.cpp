#include "CDBFormat.h"
#include "CDBEntry.h"
#include "String/CStringUtility.h"
#include "IMG/CIMGFormat.h"
#include "IMG/CIMGEntry.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

void								CDBFormat::unload(void)
{
	removeAllEntries();
}

void								CDBFormat::unserialize(void)
{
	unload();
	CDataReader *pDataReader = CDataReader::getInstance();

	// DB file header
	m_uiDBVersion = pDataReader->readUint32();
	uint32 uiEntryCount = pDataReader->readUint32();

	// copy RG structs into wrapper structs - so that we can use std::string for strings in our structs rather than char arrays
	vector<CDBEntry*>& rvecDBEntries = getEntries();
	rvecDBEntries.resize(uiEntryCount);
	CDBEntry *pDBEntries = new CDBEntry[uiEntryCount];

	// DB file entries
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		CDBEntry *pDBEntry = pDBEntries++;
		rvecDBEntries[i] = pDBEntry;
		pDBEntry->unserialize();
	}
}

void								CDBFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// DB file header
	pDataWriter->write(getDBVersion());
	pDataWriter->write(getEntryCount());

	// DB file entries
	for (auto pDBEntry : getEntries())
	{
		pDBEntry->serialize();
	}
}

bool								CDBFormat::isIMGEntryFound(CIMGFormat *pIMGFile, CIMGEntry *pIMGEntry)
{
	CDBEntry *pDBEntry = getEntryByNameExactCase(pIMGEntry->getEntryName());
	if (pDBEntry == nullptr)
	{
		return false;
	}

	if (pDBEntry->getEntryDataCRC() != CStringUtility::getCRC(pIMGEntry->getEntryData()))
	{
		return false;
	}

	return true;
}

CDBEntry*							CDBFormat::getEntryByNameExactCase(string strEntryName)
{
	for (auto pDBEntry : getEntries())
	{
		if (pDBEntry->getEntryName() == strEntryName)
		{
			return pDBEntry;
		}
	}
	return nullptr;
}