#include "CIMGFormat.h"
#include "Types.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CIMGManager.h"
#include "String/CStringUtility.h"
#include "Namespace.h"
#include "Raw/IMG/CIMGFormat_Version2_Header1.h"
#include "Raw/IMG/CIMGFormat_Version3_Header1.h"
#include "Raw/IMG/CIMGEntry_Version1Or2.h"
#include "Raw/IMG/CIMGEntry_Version3.h"
#include "eExceptionCode.h"
#include "Path/CPathUtility.h"
#include "COL/CCOLManager.h"
#include "Localization/CLocalizationManager.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersion.h"
#include "Memory/CMemoryUtility.h"
#include "IMG/CIMGFormat_VersionFastman92_Header1.h"
#include "IMG/CIMGFormat_VersionFastman92_Header2.h"
#include "IMG/CIMGEntry_Fastman92.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Engine/RAGE/CRageManager.h"
#include "Engine/RAGE/CRageResourceTypeManager.h"
#include "File/CFileUtility.h"
#include "CTiming.h"

using namespace std;

CIMGFormat::CIMGFormat(void) :
	CFormat(true, LITTLE_ENDIAN),
	m_eIMGVersion(IMG_UNKNOWN),
	m_ePlatform(PLATFORM_PC),
	m_bEncrypted(false),
	m_ucGameType(0)
{
};

// unserialization & serialization
void		CIMGFormat::unserialize(void)
{
	unserializeHeaderComponents();
	unserializeBodyComponents();
}
void		CIMGFormat::serialize(void)
{
	serializeHeaderAndBodyComponents();
}

// unserialization
void		CIMGFormat::unserializeHeaderComponents(void)
{
	CIMGPeekData imgPeekData = peekIMGData();
	setIMGVersion(imgPeekData.getIMGVersion());
	setEncrypted(imgPeekData.isEncrypted());
	switch (getIMGVersion())
	{
	case IMG_1:
		//CTiming::getInstance()->start("unserializeVersion1");
		unserializeVersion1();
		//CTiming::getInstance()->stop();
		break;
	case IMG_2:
		unserializeVersion2();
		break;
	case IMG_3:
		if (isEncrypted())
		{
			unserializeVersion3_Encrypted();
		}
		else
		{
			unserializeVersion3_Unencrypted();
		}
		break;
	case IMG_FASTMAN92:
		unserializeVersionFastman92();
		break;
	}
	loadEntryExtensionCounts();
}

void		CIMGFormat::unserializeBodyComponents(void)
{
	switch (getIMGVersion())
	{
	case IMG_1:
	case IMG_2:
	case IMG_FASTMAN92:
		//CTiming::getInstance()->start("unserializeRWVersions");
		unserializeRWVersions();
		//CTiming::getInstance()->stop();
		break;
	case IMG_3:
		unserializeResourceTypes();
		break;
	}
}

CIMGPeekData	CIMGFormat::peekIMGData(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	pDataReader->setPeek(true);
	string strHeader16B = pDataReader->readString(16);
	pDataReader->setPeek(false);
	pDataReader->setSeek(0);

	CIMGPeekData imgPeekData;
	switch (*(uint32*)(strHeader16B.c_str()))
	{
	case '2REV':
		imgPeekData.setIMGVersion(IMG_2);
		imgPeekData.setEncrypted(false);
		break;
	case 'FREV':
		imgPeekData.setIMGVersion(IMG_FASTMAN92);
		imgPeekData.setEncrypted(false);
		break;
	case 0xA94E2A52:
		imgPeekData.setIMGVersion(IMG_3);
		imgPeekData.setEncrypted(false);
		break;
	default:
		if (CStringUtility::unpackUint32(CIMGManager::decryptVersion3IMGString(strHeader16B).substr(0, 4), false) == 0xA94E2A52)
		{
			imgPeekData.setIMGVersion(IMG_3);
			imgPeekData.setEncrypted(true);
		}
		else
		{
			imgPeekData.setIMGVersion(IMG_1);
			imgPeekData.setEncrypted(false);
		}
	}

	return imgPeekData;
}

// header unserialization
void		CIMGFormat::unserializeVersion1(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // DIR file

	// verify file size & fetch entry count
	uint64 uiFileSize = pDataReader->getDataLength();
	if ((uiFileSize % 32) != 0)
	{
		throw EXCEPTION_INVALID_DATA_SIZE_MULTIPLE;
	}
	uint64 uiEntryCount = uiFileSize / sizeof(RG::CIMGEntry_Version1Or2);

	// load data from file into RG structs
	RG::CIMGEntry_Version1Or2
		*pRGIMGEntries = pDataReader->readStructMultiple<RG::CIMGEntry_Version1Or2>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy RG structs into wrapper structs - so that we can use std::string for strings in our structs rather than char arrays
	vector<CIMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	CIMGEntry *pIMGEntries = new CIMGEntry[uiEntryCount];

	for (uint64 i = 0; i < uiEntryCount; i++)
	{
		CIMGEntry *pIMGEntry = pIMGEntries++;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion1Or2(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}

	// clean up
	delete[] pRGIMGEntries;
}

void		CIMGFormat::unserializeVersion2(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // IMG file

	// read header 1
	RG::CIMGFormat_Version2_Header1 *pHeader1 = pDataReader->readStruct<RG::CIMGFormat_Version2_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// load data from file into RG structs
	RG::CIMGEntry_Version1Or2
		*pRGIMGEntries = pDataReader->readStructMultiple<RG::CIMGEntry_Version1Or2>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy RG structs into wrapper structs
	vector<CIMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	CIMGEntry *pIMGEntries = new CIMGEntry[uiEntryCount];

	for (uint64 i = 0; i < uiEntryCount; i++)
	{
		CIMGEntry *pIMGEntry = pIMGEntries++;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion1Or2(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}

	// clean up
	delete pHeader1;
	delete[] pRGIMGEntries;
}

void		CIMGFormat::unserializeVersion3_Encrypted(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // IMG file
	
	// decrypt IMG header
	string strIMGHeaderEncrypted = pDataReader->readString(32); // padded with 12 bytes at the end
	string strIMGHeaderUnencrypted = CIMGManager::decryptVersion3IMGString(strIMGHeaderEncrypted); // padded with 12 bytes at the end too
	eDataStreamType ePreviousStreamType = pDataReader->getStreamType();
	
	pDataReader->setStreamType(DATA_STREAM_MEMORY);
	pDataReader->setData(strIMGHeaderUnencrypted);
	
	// read header 1
	RG::CIMGFormat_Version3_Header1 *pHeader1 = pDataReader->readStruct<RG::CIMGFormat_Version3_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;
	
	// decrypt IMG table
	uint32 uiRemainder = (pHeader1->m_uiTableSize % 16);
	uint32 iTablesLength = pHeader1->m_uiTableSize - uiRemainder;
	
	pDataReader->setStreamType(ePreviousStreamType);
	pDataReader->setSeek(20);
	
	string strIMGTableEncrypted = pDataReader->readString(iTablesLength);
	string strIMGBodyPartial = pDataReader->readString(uiRemainder);
	string strIMGTableUnencrypted = CIMGManager::decryptVersion3IMGString(strIMGTableEncrypted) + strIMGBodyPartial;
	
	pDataReader->setStreamType(DATA_STREAM_MEMORY);
	pDataReader->setData(strIMGTableUnencrypted);

	RG::CIMGEntry_Version3
		*pRGIMGEntries = pDataReader->readStructMultiple<RG::CIMGEntry_Version3>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;
	
	// copy IMG tables RG structs into wrapper structs
	vector<CIMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	CIMGEntry *pIMGEntries = new CIMGEntry[uiEntryCount];
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		CIMGEntry *pIMGEntry = pIMGEntries++;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}
	
	// read IMG entry names
	pDataReader->setStreamType(ePreviousStreamType);
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		rvecIMGEntries[i]->setEntryName(pDataReader->readStringUntilZero());
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}
	
	// restore
	pDataReader->setStreamType(ePreviousStreamType);
}

void		CIMGFormat::unserializeVersion3_Unencrypted(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // IMG file

	// read header 1
	RG::CIMGFormat_Version3_Header1 *pHeader1 = pDataReader->readStruct<RG::CIMGFormat_Version3_Header1>();
	uint32 uiEntryCount = pHeader1->m_uiEntryCount;

	// load data from file into RG structs
	RG::CIMGEntry_Version3
		*pRGIMGEntries = pDataReader->readStructMultiple<RG::CIMGEntry_Version3>(uiEntryCount),
		*pRGIMGActiveEntry = pRGIMGEntries;

	// copy IMG tables RG structs into wrapper structs
	vector<CIMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	CIMGEntry *pIMGEntries = new CIMGEntry[uiEntryCount];
	
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		CIMGEntry *pIMGEntry = pIMGEntries++;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersion3(pRGIMGActiveEntry++);
		pIMGEntry->setEntryExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}

	// read IMG entry names
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		rvecIMGEntries[i]->setEntryName(pDataReader->readStringUntilZero());
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}

	// clean up
	delete pHeader1;
	delete[] pRGIMGEntries;
}

void		CIMGFormat::unserializeVersionFastman92(void)
{
	CDataReader *pDataReader = CDataReader::getInstance(); // IMG file
	
	// read header 1
	CIMGFormat_VersionFastman92_Header1 *pHeader1 = pDataReader->readStruct<CIMGFormat_VersionFastman92_Header1>();
	setGameType(pHeader1->m_uiGameId);

	// verify header 1 data
	if (pHeader1->m_uiFastman92IMGVersion != 1)
	{
		delete pHeader1;
		throw EXCEPTION_UNSUPPORTED_FORMAT_VERSION;
	}
	if (pHeader1->m_uiEncryptionAlgorithmId != 0)
	{
		delete pHeader1;
		setEncrypted(true);
		throw EXCEPTION_UNKNOWN_ENCRYPTION_ALGORITHM_USED;
	}
	if (pHeader1->m_uiGameId != 0)
	{
		delete pHeader1;
		throw EXCEPTION_UNSUPPORTED_GAME_USED;
	}

	// read header 2
	CIMGFormat_VersionFastman92_Header2 *pHeader2 = pDataReader->readStruct<CIMGFormat_VersionFastman92_Header2>();
	uint32 uiEntryCount = pHeader2->m_uiEntryCount;

	// verify header 2 data
	if (pHeader2->m_uiCheck != 1)
	{
		delete pHeader1;
		delete pHeader2;
		throw EXCEPTION_FILE_INVALID;
	}

	// load data from file into raw structs
	CIMGEntry_Fastman92
		*pRawIMGEntries = pDataReader->readStructMultiple<CIMGEntry_Fastman92>(uiEntryCount),
		*pRawIMGActiveEntry = pRawIMGEntries;

	// copy raw structs into wrapper structs
	vector<CIMGEntry*>& rvecIMGEntries = getEntries();
	rvecIMGEntries.clear();
	rvecIMGEntries.resize(uiEntryCount);
	CIMGEntry *pIMGEntries = new CIMGEntry[uiEntryCount];

	for (uint64 i = 0, j = uiEntryCount; i < j; i++)
	{
		CIMGEntry *pIMGEntry = pIMGEntries++;
		rvecIMGEntries[i] = pIMGEntry;
		pIMGEntry->setIMGFile(this);
		pIMGEntry->unserializeVersionFastman92(pRawIMGActiveEntry++);
		pIMGEntry->setEntryExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}

	// clean up
	delete pHeader1;
	delete pHeader2;
	delete[] pRawIMGEntries;
}

// body unserialization
void		CIMGFormat::unserializeRWVersions(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	if (getIMGVersion() == IMG_1 && pDataReader->getStreamType() == DATA_STREAM_FILE) // todo - IMG_1 and data stream memory
	{
		pDataReader->close(); // close handle to DIR file

		string strIMGFilePath = CPathUtility::replaceFileExtension(pDataReader->getFilePath(), "IMG");
		pDataReader->setFilePath(strIMGFilePath);
		pDataReader->open(true); // open handle to IMG file (handle is closed in CFormat::unserializeVia*() methods
	}

	for (auto pIMGEntry : getEntries())
	{
		string strUncompressedEntryData;
		bool bEntryIsCompressed = pIMGEntry->isCompressed();
		if (bEntryIsCompressed)
		{
			strUncompressedEntryData = pIMGEntry->getEntryData();
		}
		string& strEntryExtensionUpper = pIMGEntry->getEntryExtension();
		if (CPathUtility::isModelExtension(strEntryExtensionUpper) || strEntryExtensionUpper == "TXD")
		{
			if (bEntryIsCompressed)
			{
				if (strUncompressedEntryData.length() >= 12)
				{
					pIMGEntry->setRWVersionByVersionCC(CStringUtility::unpackUint32(strUncompressedEntryData.substr(8, 4), false));
				}
			}
			else
			{
				pDataReader->setSeek(pIMGEntry->getEntryOffset() + 8);
				pIMGEntry->setRWVersionByVersionCC(pDataReader->readUint32());
			}
		}
		else if (strEntryExtensionUpper == "COL")
		{
			if (bEntryIsCompressed)
			{
				if (strUncompressedEntryData.length() >= 4)
				{
					pIMGEntry->setCOLVersion(CCOLManager::getCOLVersionFromFourCC(strUncompressedEntryData.substr(0, 4)));
				}
			}
			else
			{
				pDataReader->setSeek(pIMGEntry->getEntryOffset());
				pIMGEntry->setCOLVersion(CCOLManager::getCOLVersionFromFourCC(pDataReader->readString(4)));
			}
		}

		CEventManager::getInstance()->triggerEvent(EVENT_onParseIMGEntry, this);
	}
}
void		CIMGFormat::unserializeResourceTypes(void)
{
	// todo
}

// header & body serialization
void		CIMGFormat::serializeHeaderAndBodyComponents(void)
{
	switch (getIMGVersion())
	{
	case IMG_1:
		serializeVersion1();
		break;
	case IMG_2:
		serializeVersion2();
		break;
	case IMG_3:
		if (isEncrypted())
		{
			serializeVersion3_Encrypted();
		}
		else
		{
			serializeVersion3_Unencrypted();
		}
		break;
	case IMG_FASTMAN92:
		serializeVersionFastman92();
		break;
	}
}
void					CIMGFormat::serializeVersion1(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// open IMG file to read from (IMG file to write to is already open in CDataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
	}

	// write IMG data
	uint32 uiSeek = 0;
	for (auto pIMGEntry : getEntries())
	{
		uint32 uiEntryByteCountPadded = CIMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize());

		pDataReader->setSeek(pIMGEntry->getEntryOffset());
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), uiEntryByteCountPadded);

		pIMGEntry->setEntryOffsetInSectors(uiSeek);
		uiSeek += uiEntryByteCountPadded;

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		// finalize IMG data reading/writing
		pDataReader->close();
		pDataWriter->close();

		// open DIR files to read/write from/to
		string
			strIMGFilePath = getFilePath(),
			strDIRFilePath = CPathUtility::replaceFileExtension(strIMGFilePath, "DIR");

		pDataReader->setFilePath(strDIRFilePath);
		pDataReader->open(true);

		pDataWriter->setFilePath(strDIRFilePath);
		pDataWriter->open(true);
	}

	// write DIR data
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint32(pIMGEntry->getEntrySizeInSectors());
		pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 24);

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	// finalize DIR data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->close();
		pDataWriter->close(); // optionally called here, CDataWriter::close() will be called by CFormat too.
	}
}

void					CIMGFormat::serializeVersion2(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiBodyStart = (uiEntryCount * 32) + 8,
		uiSeek = CMathUtility::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in CDataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	pDataWriter->writeString("VER2");
	pDataWriter->writeUint32(getEntryCount());

	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(vecNewEntryPositions[i++]);
		pDataWriter->writeUint16(ceil((float)(pIMGEntry->getEntrySize() / (float)2048.0f)));
		pDataWriter->writeUint16(0);
		pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 24);

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	if ((uiBodyStart % 2048) != 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// write IMG data - IMG body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffsetInSectors() * 2048);
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), CIMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->close();
		pDataWriter->close(); // optionally called here, CDataWriter::close() will be called by CFormat too.
	}
}

void					CIMGFormat::serializeVersionFastman92(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiBodyStart = (uiEntryCount * 32) + 8,
		uiSeek = CMathUtility::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in CDataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	string strIMGVersion = "VERF";
	uint32 uiArchiveVersion = 1;
	uint32 uiArchiveFlags = uiArchiveVersion;

	pDataWriter->writeStringRef(strIMGVersion);
	pDataWriter->writeUint32(uiArchiveFlags);
	pDataWriter->writeString("fastman92", 12);
	if (uiArchiveVersion == 1)
	{
		// write IMG data - IMG header
		uint32 uiCheck = 1;
		uint32 uiEntryCount = getEntryCount();
		string strReserved1 = CStringUtility::zeroPad(8);

		pDataWriter->writeUint32(uiCheck);
		pDataWriter->writeUint32(uiEntryCount);
		pDataWriter->writeStringRef(strReserved1);

		// write IMG data - IMG directory
		i = 0;
		for (auto pIMGEntry : getEntries())
		{
			uint32 uiEntryFlags = 0;
			if (pIMGEntry->isCompressed())
			{
				uint32 uiCompressionValue = 0;
				switch (pIMGEntry->getCompressionAlgorithmId())
				{
				case COMPRESSION_ZLIB:
					uiCompressionValue = 1;
					break;
				case COMPRESSION_LZ4:
					uiCompressionValue = 2;
					break;
				}
				uiEntryFlags |= uiCompressionValue;
			}
			uint16 usUncompressedSizeInSectors = ceil((float)pIMGEntry->getUncompressedSize() / (float)2048.0f);
			uint16 usPackedSizeInSectors = pIMGEntry->getEntrySizeInSectors();
			uint16 usPaddedBytesCountInAlignedOriginalSize = (usUncompressedSizeInSectors * 2048) % 2048;
			uint16 usPaddedBytesCountInAlignedPackedSize = (usPackedSizeInSectors * 2048) % 2048;

			pDataWriter->writeUint32(vecNewEntryPositions[i++]);
			pDataWriter->writeUint16(usUncompressedSizeInSectors);
			pDataWriter->writeUint16(usPaddedBytesCountInAlignedOriginalSize);
			pDataWriter->writeUint16(usPackedSizeInSectors);
			pDataWriter->writeUint16(usPaddedBytesCountInAlignedPackedSize);
			pDataWriter->writeUint32(uiEntryFlags);
			pDataWriter->writeStringRef(pIMGEntry->getEntryName(), 40);
			pDataWriter->writeString(8);

			CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
		}

		if ((uiBodyStart % 2048) != 0)
		{
			uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
			pDataWriter->writeString(uiPadByteCount);
		}

		// write IMG data - IMG body
		i = 0;
		for (auto pIMGEntry : getEntries())
		{
			pDataReader->setSeek(pIMGEntry->getEntryOffsetInSectors() * 2048);
			pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()), CIMGFormat::getEntryPaddedSize(pIMGEntry->getEntrySize()));

			pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

			CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
		}
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->close();
		pDataWriter->close(); // optionally called here, CDataWriter::close() will be called by CFormat too.
	}
}

void					CIMGFormat::serializeVersion3_Encrypted(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiNamesLength = getVersion3NamesLength(),
		uiBodyStart = 20 + (16 * getEntryCount()) + uiNamesLength,
		uiSeek = CMathUtility::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in CDataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
	}

	// write IMG data - IMG header
	eDataStreamType ePreviousStreamType = pDataWriter->getStreamType();
	pDataWriter->setStreamType(DATA_STREAM_MEMORY);

	pDataWriter->writeUint32(0xA94E2A52);
	pDataWriter->writeUint32(3);
	pDataWriter->writeUint32(getEntryCount());
	pDataWriter->writeUint32((ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048.0));
	pDataWriter->writeUint16(16);
	pDataWriter->writeUint16(0);

	// IMG file - table
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(0);
		pDataWriter->writeUint32(pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier());
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint16(ceil(((float)pIMGEntry->getEntrySize()) / (float)2048.0f));

		uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
		pDataWriter->writeUint16((pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder)))));

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	string strEntryName;
	for (auto pIMGEntry : getEntries())
	{
		strEntryName = pIMGEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter->writeStringRef(strEntryName);
	}

	if ((uiBodyStart % 2048) != 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// encrypt header and table
	string strData = pDataWriter->getData();

	string strHeader = CIMGManager::encryptVersion3IMGString(strData.substr(0, 32));
	string strTables = CIMGManager::encryptVersion3IMGString(CStringUtility::zeroPad(strData.substr(20), (strData.length() - 20) + (2048 - ((strData.length() - 20) % 2048))));

	pDataWriter->setStreamType(ePreviousStreamType);
	pDataWriter->setSeek(0);
	pDataWriter->writeStringRef(strHeader);
	pDataWriter->setSeek(20);
	pDataWriter->writeStringRef(strTables);

	// IMG file - body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffsetInSectors() * 2048);
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->close();
		pDataWriter->close(); // optionally called here, CDataWriter::close() will be called by CFormat too.
	}
}

void					CIMGFormat::serializeVersion3_Unencrypted(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	// fetch new seek positions for all IMG entries
	uint32
		uiEntryCount = getEntryCount(),
		uiNamesLength = getVersion3NamesLength(),
		uiBodyStart = 20 + (16 * getEntryCount()) + uiNamesLength,
		uiSeek = CMathUtility::convertBytesToSectors(uiBodyStart),
		i = 0;
	vector<uint32> vecNewEntryPositions;
	vecNewEntryPositions.resize(uiEntryCount);
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		vecNewEntryPositions[i] = uiSeek;
		uiSeek += pIMGEntry->getEntrySizeInSectors();
		i++;
	}

	// open IMG file to read from (IMG file to write to is already open in CDataWriter)
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
	}

	// IMG file - header
	pDataWriter->writeUint32(0xA94E2A52);
	pDataWriter->writeUint32(3);
	pDataWriter->writeUint32(getEntryCount());
	pDataWriter->writeUint32((ceil(((float)((16 * getEntryCount()) + uiNamesLength)) / 2048.0) * 2048));
	pDataWriter->writeUint16(16);
	pDataWriter->writeUint16(0);

	// IMG file - table
	for (auto pIMGEntry : getEntries())
	{
		pDataWriter->writeUint32(0);
		pDataWriter->writeUint32((pIMGEntry->getRageResourceType() == nullptr ? 0 : pIMGEntry->getRageResourceType()->getIdentifier()));
		pDataWriter->writeUint32(pIMGEntry->getEntryOffsetInSectors());
		pDataWriter->writeUint16(ceil(((float)pIMGEntry->getEntrySize()) / (float)2048.0f));

		uint32 uiRemainder = pIMGEntry->getEntrySize() % 2048;
		pDataWriter->writeUint16((pIMGEntry->getFlags() | ((uint16)(uiRemainder == 0 ? 0 : (2048 - uiRemainder)))));

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	string strEntryName;
	for (auto pIMGEntry : getEntries())
	{
		strEntryName = pIMGEntry->getEntryName();
		strEntryName.append("\0", 1);
		pDataWriter->writeStringRef(strEntryName);
	}

	if ((uiBodyStart % 2048) != 0)
	{
		uint32 uiPadByteCount = 2048 - (uiBodyStart % 2048);
		pDataWriter->writeString(uiPadByteCount);
	}

	// IMG file - body
	i = 0;
	for (auto pIMGEntry : getEntries())
	{
		pDataReader->setSeek(pIMGEntry->getEntryOffsetInSectors() * 2048);
		pDataWriter->writeString(pDataReader->readString(pIMGEntry->getEntrySize()));

		pIMGEntry->setEntryOffsetInSectors(vecNewEntryPositions[i++]);

		CEventManager::getInstance()->triggerEvent(EVENT_onStoreIMGEntry, this);
	}

	// finalize IMG data reading/writing
	if (pDataReader->getStreamType() == DATA_STREAM_FILE && pDataWriter->getStreamType() == DATA_STREAM_FILE) // todo make it work for DATA_STREAM_MEMORY too
	{
		pDataReader->close();
		pDataWriter->close(); // optionally called here, CDataWriter::close() will be called by CFormat too.
	}
}

// extension counts
void					CIMGFormat::addEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = CStringUtility::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		m_umapExtensionCounts[strEntryExtension]++;
	}
	else
	{
		m_umapExtensionCounts[strEntryExtension] = 1;
	}
}
void					CIMGFormat::removeEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = CStringUtility::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts[strEntryExtension] > 1)
	{
		m_umapExtensionCounts[strEntryExtension]--;
	}
	else
	{
		m_umapExtensionCounts.erase(strEntryExtension);
	}
}

uint32					CIMGFormat::getEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = CStringUtility::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		return m_umapExtensionCounts[strEntryExtension];
	}
	else
	{
		return 0;
	}
}

void					CIMGFormat::loadEntryExtensionCounts(void)
{
	m_umapExtensionCounts.clear();
	for (auto pIMGEntry : getEntries())
	{
		addEntryExtensionCount(pIMGEntry->getEntryExtension());
	}
}

vector<string>			CIMGFormat::getEntryExtensions(void)
{
	vector<string> vecEntryExtensions;
	for (auto it : m_umapExtensionCounts)
	{
		vecEntryExtensions.push_back(it.first);
	}
	return vecEntryExtensions;
}

vector<string>			CIMGFormat::getEntryVersions(vector<eCOLVersion>& vecCOLVersions, vector<eRWVersion>& vecRWVersions)
{
	unordered_map<std::string, eCOLVersion> umapCOLEntryExtensions;
	unordered_map<std::string, eRWVersion> umapRWEntryExtensions;
	for (CIMGEntry* pIMGEntry : getEntries())
	{
		string
			strEntryExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())),
			strEntryVersion;
		if (strEntryExtension == "COL")
		{
			eCOLVersion eCOLVersionValue = pIMGEntry->getCOLVersion() == nullptr ? COL_UNKNOWN : pIMGEntry->getCOLVersion()->getVersionId();
			strEntryVersion = eCOLVersionValue == COL_UNKNOWN ? CLocalizationManager::getInstance()->getTranslatedText("UnknownVersion") : CCOLManager::getCOLVersionText(eCOLVersionValue);
			umapCOLEntryExtensions[strEntryVersion] = eCOLVersionValue;
		}
		else if (strEntryExtension == "DFF" || strEntryExtension == "TXD")
		{
			eRWVersion eRWVersionValue = pIMGEntry->getRWVersion() == nullptr ? RW_VERSION_UNKNOWN : pIMGEntry->getRWVersion()->getVersionId();
			strEntryVersion = pIMGEntry->getRWVersion() == nullptr ? CLocalizationManager::getInstance()->getTranslatedText("UnknownVersion") : CRWManager::getRWVersionText(pIMGEntry->getRWVersion());
			umapRWEntryExtensions[strEntryVersion] = eRWVersionValue;
		}
		else
		{
			strEntryVersion = CLocalizationManager::getInstance()->getTranslatedText("UnknownVersion");
		}
	}
	vector<string> vecEntryVersions;
	for (auto it : umapCOLEntryExtensions)
	{
		vecEntryVersions.push_back(it.first);
		vecCOLVersions.push_back(it.second);
	}
	for (auto it : umapRWEntryExtensions)
	{
		vecEntryVersions.push_back(it.first);
		vecRWVersions.push_back(it.second);
	}
	return vecEntryVersions;
}

eCompressionAlgorithm								CIMGFormat::getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(eIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId)
{
	switch (eFastman92CompressionAlgorithmId)
	{
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNKNOWN:		return COMPRESSION_UNKNOWN;
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNCOMPRESSED:	return COMPRESSION_NONE;
	case IMGFASTMAN92COMPRESSIONALGORITHM_ZLIB:			return COMPRESSION_ZLIB;
	case IMGFASTMAN92COMPRESSIONALGORITHM_LZ4:			return COMPRESSION_LZ4;
	}
	return COMPRESSION_UNKNOWN;
}

uint32			CIMGFormat::getVersion3NamesLength(void)
{
	uint32 uiLength = 0;
	for (auto pIMGEntry : getEntries())
	{
		uiLength += pIMGEntry->getEntryName().length();
	}
	uiLength += getEntryCount();
	return uiLength;
}

CIMGEntry*							CIMGFormat::addEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = CPathUtility::getFileName(strEntryFilePath);
	}

	string strEntryData = CFileUtility::getFileContent(strEntryFilePath);
	CIMGEntry *pIMGEntry = addEntryViaData(strEntryName, strEntryData);
	pIMGEntry->setFileCreationDate(CFileUtility::getFileCreationDate(strEntryFilePath));
	return pIMGEntry;
}

CIMGEntry*							CIMGFormat::addEntryViaData(string& strEntryName, string& strEntryData)
{
	CIMGEntry *pIMGEntry = new CIMGEntry(this);

	pIMGEntry->setNewEntry(true);
	pIMGEntry->setEntryName(strEntryName);
	pIMGEntry->setEntrySize(strEntryData.length()); // this is also set inside CIMGEntry::setEntryData()

	if (getIMGVersion() == IMG_3)
	{
		pIMGEntry->setRageResourceType(CRageManager::getInstance()->getResourceTypeManager()->getResourceTypeByFileExtension(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
		pIMGEntry->setFlags((uint16)CIMGEntry::getVersion3IMGSizeDeduction(pIMGEntry->getEntrySize()));
	}
	else
	{
		string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(strEntryName));
		if (strExtensionUpper == "TXD" || CPathUtility::isModelExtension(strExtensionUpper))
		{
			if (strEntryData.length() >= 12)
			{
				pIMGEntry->setRWVersionByVersionCC(CStringUtility::unpackUint32(strEntryData.substr(8, 4), false));
			}
		}
		else if (strExtensionUpper == "COL")
		{
			if (strEntryData.length() >= 4)
			{
				pIMGEntry->setCOLVersion(CCOLManager::getCOLVersionFromFourCC(strEntryData.substr(0, 4)));
			}
		}
	}

	pIMGEntry->setEntryData(strEntryData, true);
	addEntry(pIMGEntry);
	return pIMGEntry;
}

void					CIMGFormat::addEntry(CIMGEntry *pIMGEntry)
{
	CVectorPool::addEntry(pIMGEntry);

	CEventManager::getInstance()->triggerEvent(EVENT_onAddIMGEntry, pIMGEntry);

	string strEntryExtension = CPathUtility::getFileExtension(pIMGEntry->getEntryName());
	addEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 1)
	{
		CEventManager::getInstance()->triggerEvent(EVENT_onAddIMGEntryExtension, pIMGEntry);
	}
}

void								CIMGFormat::addEntries(vector<string>& vecEntryFilePaths)
{
	for (auto strEntryFilePath : vecEntryFilePaths)
	{
		addEntryViaFile(strEntryFilePath);
	}
}

void								CIMGFormat::addAllEntriesInFolder(string& strFolderPath)
{
	vector<string> vecFileNames = CFileUtility::getFileNames(strFolderPath);
	for (auto strFileName : vecFileNames)
	{
		addEntryViaFile(strFileName);
	}
}

void								CIMGFormat::removeEntry(CIMGEntry *pIMGEntry)
{
	string strEntryExtension = CPathUtility::getFileExtension(pIMGEntry->getEntryName());
	removeEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 0)
	{
		CEventManager::getInstance()->triggerEvent(EVENT_onRemoveIMGEntryExtension, pIMGEntry);
	}

	CEventManager::getInstance()->triggerEvent(EVENT_onRemoveIMGEntry, pIMGEntry);

	CVectorPool::removeEntry(pIMGEntry);
}

CIMGEntry*							CIMGFormat::replaceEntryViaFile(string& strEntryName, string& strEntryFilePath, string strNewEntryName)
{
	string strEntryData = CFileUtility::getFileContent(strEntryFilePath);
	CIMGEntry *pIMGEntry = replaceEntryViaData(strEntryName, strEntryData, strNewEntryName);
	pIMGEntry->setFileCreationDate(CFileUtility::getFileCreationDate(strEntryFilePath));
	return pIMGEntry;
}
CIMGEntry*							CIMGFormat::replaceEntryViaData(string& strEntryName, string& strEntryData, string strNewEntryName)
{
	CIMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (!pIMGEntry)
	{
		return nullptr;
	}

	if (strNewEntryName != "")
	{
		pIMGEntry->setEntryName(strNewEntryName);
	}

	pIMGEntry->setEntryData(strEntryData);
	return pIMGEntry;
}

uint32						CIMGFormat::replaceEntries(vector<string>& vecPaths, vector<string>& vecReplacedEntryNames, vector<CIMGEntry*>& vecReplacedEntries)
{
	uint32 uiReplaceCount = 0;

	for (auto strPath : vecPaths)
	{
		string strNewEntryName = CPathUtility::getFileName(strPath);
		CIMGEntry *pIMGEntry = getEntryByName(strNewEntryName);
		if (!pIMGEntry)
		{
			CEventManager::getInstance()->triggerEvent(EVENT_onTaskProgress, this);
			continue;
		}

		// body
		string strFileContent = CFileUtility::getFileContent(strPath);
		uint32 uiFileSize = strFileContent.length();
		pIMGEntry->setEntryData(strFileContent);

		// header
		pIMGEntry->setEntryName(strNewEntryName);
		pIMGEntry->setEntrySize(uiFileSize);
		if (getIMGVersion() == IMG_3)
		{
			pIMGEntry->setRageResourceType(CRageManager::getInstance()->getResourceTypeManager()->getResourceTypeByFileExtension(CPathUtility::getFileExtension(strNewEntryName)));
		}
		else
		{
			pIMGEntry->setRWVersionByVersionCC(strFileContent.length() >= 12 ? CStringUtility::unpackUint32(strFileContent.substr(8, 4), false) : 0);
		}

		uiReplaceCount++;
		vecReplacedEntryNames.push_back(strNewEntryName);
		vecReplacedEntries.push_back(pIMGEntry);

		CEventManager::getInstance()->triggerEvent(EVENT_onTaskProgress, this);
	}

	return uiReplaceCount;
}

CIMGEntry*							CIMGFormat::addOrReplaceEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	CIMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaFile(strEntryName, strEntryFilePath);
	}
	else
	{
		pIMGEntry = addEntryViaFile(strEntryFilePath, strEntryName);
	}
	return pIMGEntry;
}
CIMGEntry*							CIMGFormat::addOrReplaceEntryViaData(string& strEntryName, string& strEntryData)
{
	CIMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaData(strEntryName, strEntryData);
	}
	else
	{
		pIMGEntry = addEntryViaData(strEntryName, strEntryData);
	}
	return pIMGEntry;
}

unordered_map<CIMGEntry*, string>	CIMGFormat::getAllEntriesData(void)
{
	return getEntriesData(getEntries());
}

unordered_map<CIMGEntry*, string>	CIMGFormat::getEntriesData(vector<CIMGEntry*>& vecEntries)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	unordered_map<CIMGEntry*, string> umapEntriesData;

	try
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
		for (auto pIMGEntry : vecEntries)
		{
			pDataReader->setSeek(pIMGEntry->getEntryOffset());
			umapEntriesData.insert(make_pair(pIMGEntry, pDataReader->readString(pIMGEntry->getEntrySize())));
		}
		pDataReader->close();
	}
	catch (eExceptionCode eCode)
	{
		pDataReader->reset();
	}

	return umapEntriesData;
}

uint32			CIMGFormat::getNextEntryOffset(void)
{
	if (getEntryCount() == 0)
	{
		switch (getIMGVersion())
		{
		case IMG_1:
			return 0;
		case IMG_2:
		case IMG_3:
			return 2048;
		}
		return 0;
	}

	CIMGEntry *pIMGEntry = getEntryByHighestOffset();
	return pIMGEntry->getEntryOffset() + pIMGEntry->getPaddedEntrySize();
}

vector<CIMGEntry*>		CIMGFormat::getEntriesByExtension(string strExtension)
{
	strExtension = CStringUtility::toUpperCase(strExtension);
	vector<CIMGEntry*> vecIMGEntries;
	for (auto pIMGEntry : getEntries())
	{
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) == strExtension)
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}
	return vecIMGEntries;
}

CIMGEntry*				CIMGFormat::getEntryByName(string& strEntryName)
{
	strEntryName = CStringUtility::toUpperCase(strEntryName);
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryName == CStringUtility::toUpperCase(pIMGEntry->getEntryName()))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

CIMGEntry*				CIMGFormat::getEntryByNameWithoutExtension(string& strEntryNameWithoutExtension)
{
	strEntryNameWithoutExtension = CStringUtility::toUpperCase(strEntryNameWithoutExtension);
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryNameWithoutExtension == CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName())))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

CIMGEntry*				CIMGFormat::getEntryByHighestOffset(void)
{
	uint32 uiHighestOffset = 0;
	CIMGEntry *pHighestOffsetIMGEntry = nullptr;
	for (auto pIMGEntry : getEntries())
	{
		if (pIMGEntry->getEntryOffsetInSectors() >= uiHighestOffset)
		{
			uiHighestOffset = pIMGEntry->getEntryOffsetInSectors();
			pHighestOffsetIMGEntry = pIMGEntry;
		}
	}
	return pHighestOffsetIMGEntry;
}

uint32			CIMGFormat::getEntryCountForName(string& strEntryName)
{
	strEntryName = CStringUtility::toUpperCase(strEntryName);
	uint32 uiNameCount = 0;
	for (auto pIMGEntry : getEntries())
	{
		if (strEntryName == CStringUtility::toUpperCase(pIMGEntry->getEntryName()))
		{
			uiNameCount++;
		}
	}
	return uiNameCount;
}

void					CIMGFormat::unsetNewAndReplacedFlagForAllEntries(void)
{
	for (auto pIMGEntry : getEntries())
	{
		pIMGEntry->setNewEntry(false);
		pIMGEntry->setReplacedEntry(false);
	}
}

uint32			CIMGFormat::getEntryPaddedSize(uint32 uiDataLength)
{
	return (uint32)(ceil((float)uiDataLength / (float)2048.0f) * (float)2048.0f);
}

uint32			CIMGFormat::merge(string& strSecondIMGPath, vector<string>& vecImportedEntryNames)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	uint32 uiImportedEntryCount = 0;
	try
	{
		// parse second IMG file for entry information
		CIMGFormat *pIMGFileIn = CIMGManager::getInstance()->parseViaFile(strSecondIMGPath);
		if (pIMGFileIn->doesHaveError())
		{
			pIMGFileIn->unload();
			delete pIMGFileIn;
			return 0;
		}

		// open second IMG file to read entry body data from
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);

		// import entries from second IMG into first IMG
		bool bVersion3IMG = getIMGVersion() == IMG_3;
		for (auto pInEntry : pIMGFileIn->getEntries())
		{
			// entry header data
			CIMGEntry *pOutEntry = new CIMGEntry(this);
			pOutEntry->setEntryName(pInEntry->getEntryName());
			pOutEntry->setEntrySize(pInEntry->getEntrySize());
			if (bVersion3IMG)
			{
				pOutEntry->setRageResourceType(pInEntry->getRageResourceType());
				pOutEntry->setFlags(pInEntry->getFlags());
			}
			else
			{
				pOutEntry->setRWVersion(pInEntry->getRWVersion());
			}
			addEntry(pOutEntry);

			// entry body data
			pDataReader->setSeek(pInEntry->getEntryOffset());
			string strEntryData = pDataReader->readString(pInEntry->getEntrySize());
			pOutEntry->setEntryData(strEntryData, true);

			vecImportedEntryNames.push_back(pOutEntry->getEntryName());

			CEventManager::getInstance()->triggerEvent(EVENT_onTaskProgress, this);
		}

		// finalize
		pDataReader->close();

		uint32 uiImportedEntryCount = pIMGFileIn->getEntryCount();
		pIMGFileIn->unload();
		delete pIMGFileIn;
	}
	catch (eExceptionCode eCode)
	{
		pDataReader->reset();
	}

	return uiImportedEntryCount;
}

void					CIMGFormat::split(vector<CIMGEntry*>& vecIMGEntries, string& strOutPath, eIMGVersion eIMGVersion)
{
	CIMGFormat *pIMGFile = new CIMGFormat;
	pIMGFile->setIMGVersion(eIMGVersion);
	pIMGFile->setFilePath(getFilePath());

	bool bVersion3IMG = eIMGVersion == IMG_3;
	for (auto pIMGEntry : vecIMGEntries)
	{
		CIMGEntry *pIMGEntry2 = new CIMGEntry(pIMGFile);
		pIMGEntry2->setEntryName(pIMGEntry->getEntryName());
		pIMGEntry2->setEntryOffsetInSectors(pIMGEntry->getEntryOffsetInSectors());
		pIMGEntry2->setEntrySize(pIMGEntry->getEntrySize());
		pIMGFile->addEntry(pIMGEntry2);

		if (bVersion3IMG)
		{
			pIMGEntry2->setRageResourceType(pIMGEntry->getRageResourceType());
			pIMGEntry2->setFlags(pIMGEntry->getFlags());
		}
		else
		{
			pIMGEntry2->setRWVersion(pIMGEntry->getRWVersion());
		}

		CEventManager::getInstance()->triggerEvent(EVENT_onTaskProgress, this);
	}

	pIMGFile->serializeViaFile(strOutPath);

	pIMGFile->unload();
	delete pIMGFile;
}

void					CIMGFormat::exportSingle(CIMGEntry *pIMGEntry, string& strFolderPath)
{
	if (!pIMGEntry->canBeRead())
	{
		return;
	}

	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);

	CFileUtility::storeFile(strFolderPath + pIMGEntry->getEntryName(), pIMGEntry->getEntryData(), false, true);
}

void					CIMGFormat::exportMultiple(vector<CIMGEntry*>& vecIMGEntries, string strFolderPath)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);

	try
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);

		for (auto pIMGEntry : vecIMGEntries)
		{
			if (!pIMGEntry->canBeRead())
			{
				continue;
			}

			pDataReader->setSeek(pIMGEntry->getEntryOffset());
			string strFileContent = pDataReader->readString(pIMGEntry->getEntrySize());
			CFileUtility::storeFile(strFolderPath + pIMGEntry->getEntryName(), strFileContent, false, true);

			CEventManager::getInstance()->triggerEvent(EVENT_onTaskProgress, this);
		}

		pDataReader->close();
	}
	catch (eExceptionCode eCode)
	{
		pDataReader->reset();
	}
}

void					CIMGFormat::exportAll(string& strFolderPath)
{
	CDataReader *pDataReader = CDataReader::getInstance();
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);

	try
	{
		pDataReader->setFilePath(getFilePath());
		pDataReader->open(true);
		for (auto pIMGEntry : getEntries())
		{
			if (!pIMGEntry->canBeRead())
			{
				continue;
			}

			pDataReader->setSeek(pIMGEntry->getEntryOffset());
			pIMGEntry->saveEntryByMemory(strFolderPath + pIMGEntry->getEntryName(), pDataReader->readString(pIMGEntry->getEntrySize()));
		}
		pDataReader->close();
	}
	catch (eExceptionCode eCode)
	{
		pDataReader->reset();
	}
}

CIMGFormat*				CIMGFormat::clone(string& strClonedIMGPath)
{
	serializeViaFile(strClonedIMGPath);
	return CIMGManager::getInstance()->parseViaFile(strClonedIMGPath);
}

vector<CIMGEntry*>		CIMGFormat::getUnknownVersionEntries(void)
{
	vector<CIMGEntry*> vecIMGEntries;
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->doesHaveUnknownVersion())
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}
	return vecIMGEntries;
}

uint32			CIMGFormat::getEntryCountForCompressionType(eCompressionAlgorithm eCompressionAlgorithmValue)
{
	uint32 uiCount = 0;
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->getCompressionAlgorithmId() == eCompressionAlgorithmValue)
		{
			uiCount++;
		}
	}
	return uiCount;
}

vector<string>			CIMGFormat::getEntryNames(void)
{
	vector<string> vecEntryNames;
	vecEntryNames.resize(getEntryCount());
	uint32 i = 0;
	for (CIMGEntry *pIMGEntry : getEntries())
	{
		vecEntryNames[i] = pIMGEntry->getEntryName();
		++i;
	}
	return vecEntryNames;
}