#include "CIMGEntry.h"
#include "File/CFileUtility.h"
#include "CIMGFormat.h"
#include "String/CStringUtility.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Engine/RAGE/CRageManager.h"
#include "Path/CPathUtility.h"
#include "Engine/RAGE/CRageResourceTypeManager.h"
#include "Testing/CDebugger.h"
#include "CIMGManager.h"
#include "Compression/CCompressionUtility.h"
#include "Raw/IMG/CIMGEntry_Version1Or2.h"
#include "Raw/IMG/CIMGEntry_Version3.h"
#include "IMG/CIMGEntry_Fastman92.h"
#include "Localization/CLocalizationManager.h"
#include "COL/CCOLManager.h"

using namespace std;

CIMGEntry::CIMGEntry(void) :
	m_pIMGFile(nullptr),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_eCompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),

	m_pRageResourceType(nullptr),
	
	m_pRWVersion(nullptr),
	m_pCOLVersion(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0)
{
}

CIMGEntry::CIMGEntry(CIMGFormat *pIMGFile) :
	m_pIMGFile(pIMGFile),
	m_uiEntryOffset(0),
	m_uiEntrySize(0),
	m_bNewEntry(false),
	m_bReplacedEntry(false),
	m_bProtectedEntry(false),
	m_uiFileCreationDate(0),
	m_eCompressionAlgorithm(COMPRESSION_NONE),
	m_uiCompressionLevel(0),
	m_bIsEncrypted(false),

	m_pRageResourceType(nullptr),

	m_pRWVersion(nullptr),
	m_pCOLVersion(nullptr),
	m_uiUncompressedSize(0),
	m_uiFlags(0)
{
}

void					CIMGEntry::unserializeVersion1Or2(RG::CIMGEntry_Version1Or2 *pRGIMGEntry)
{
	m_uiEntryOffset = CMathUtility::convertSectorsToBytes(pRGIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = CMathUtility::convertSectorsToBytes(pRGIMGEntry->m_uiSizeInSectors);
	m_strEntryName = CStringUtility::rtrimFromLeft(string((char*)pRGIMGEntry->m_szName));
}

void					CIMGEntry::unserializeVersion3(RG::CIMGEntry_Version3 *pRGIMGEntry)
{
	setRageResourceTypeByIdentifier(pRGIMGEntry->m_uiRageResourceTypeIdentifier);
	m_uiEntryOffset = CMathUtility::convertSectorsToBytes(pRGIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = CMathUtility::convertSectorsToBytes(pRGIMGEntry->m_uiSizeInSectors) - (pRGIMGEntry->m_uiFlags & 2047);
	m_uiFlags = pRGIMGEntry->m_uiFlags;
}

void					CIMGEntry::unserializeVersionFastman92(CIMGEntry_Fastman92 *pRawIMGEntry)
{
	m_uiEntryOffset = CMathUtility::convertSectorsToBytes(pRawIMGEntry->m_uiOffsetInSectors);
	m_uiEntrySize = CMathUtility::convertSectorsToBytes(pRawIMGEntry->m_uiSizeInSectors);
	m_uiUncompressedSize = CMathUtility::convertSectorsToBytes(pRawIMGEntry->m_uiUncompressedSizeInSectors);
	m_eCompressionAlgorithm = CIMGFormat::getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId((eIMGVersionFastman92CompressionAlgorithm)pRawIMGEntry->m_uiCompressionAlgorithmId);
	m_strEntryName = CStringUtility::rtrimFromLeft(string((char*)pRawIMGEntry->m_strName));
}

void					CIMGEntry::setEntrySize(uint32 uiEntrySize)
{
	m_uiEntrySize = uiEntrySize;
	if (getIMGFile()->getIMGVersion() == IMG_3)
	{
		m_uiFlags = (m_uiFlags & 63488) | (uint16)getVersion3IMGSizeDeduction(uiEntrySize);
	}
}

uint32			CIMGEntry::getPaddedEntrySize(void)
{
	return (uint32) (ceil(((float32) getEntrySize()) / 2048.0f) * 2048.0f);
}

void					CIMGEntry::setEntryData(string strEntryData, bool bIsNew)
{
	//setEntryOffset(getIMGFile()->getNextEntryOffset()); // todo - this line is needed but getNextEntryOffset isnt defined yet
	uint32 uiFUncompressedSize = (uint32)strEntryData.length();
	if(!bIsNew)
	{
		setReplacedEntry(true);
	}

	strEntryData = CStringUtility::zeroPad(strEntryData, CIMGEntry::getEntryDataPadLength(strEntryData.length()));
	if (getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
	{
		setUncompressedSize(uiFUncompressedSize);

		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:
				strEntryData = CCompressionUtility::compressZLib(strEntryData, getCompressionLevel());
				break;
			case COMPRESSION_LZ4:
				strEntryData = CCompressionUtility::compressLZ4(strEntryData, getCompressionLevel());
				break;
			}
		}
		else
		{
			// not compressed
		}
	}
	/*
	// todo - add getPlatform() to CIMGFormat and uncomment this code
	else if (getIMGFile()->getPlatform() == PLATFORM_XBOX)
	{
		// the entry data is compressed with LZO 1X 999 compression
		bool bBigEndian = false;
		string strOutputData = "";
		const uint32 uiBlockSize = 131072;

		for (uint32 i = 0, uiBlockCount = ceil((float32)strEntryData.length() / (float32)uiBlockSize); i < uiBlockCount; i++)
		{
			string strCompressedData;
			if (i == (uiBlockCount - 1))
			{
				strCompressedData = CCompressionUtility::compressLZO1X999(strEntryData.substr(i * uiBlockSize));
			}
			else
			{
				strCompressedData = CCompressionUtility::compressLZO1X999(strEntryData.substr(i * uiBlockSize, uiBlockSize));
			}

			uint32 uiUnknown1 = 4;
			uint32 uiUncompressedBlockSize = strEntryData.length();
			uint32 uiCompressedBlockSize = strCompressedData.length();

			strOutputData += CStringUtility::packUint32(uiUnknown1, bBigEndian);
			strOutputData += CStringUtility::packUint32(uiUncompressedBlockSize, bBigEndian);
			strOutputData += CStringUtility::packUint32(uiCompressedBlockSize, bBigEndian);

			strOutputData += strCompressedData;
		}

		uint32 uiMagicNumber = 0x67A3A1CE;
		uint32 uiChecksum = 0;
		uint32 uiCompressionDataSize = strOutputData.length();

		string strOutputData2 = "";
		strOutputData2 += CStringUtility::packUint32(uiMagicNumber, bBigEndian);
		strOutputData2 += CStringUtility::packUint32(uiChecksum, bBigEndian);
		strOutputData2 += CStringUtility::packUint32(uiCompressionDataSize, bBigEndian);
		
		strEntryData = strOutputData2 + strOutputData;
	}
	*/
	setEntrySize((uint32)strEntryData.length());
	CFileUtility::storeFileSubContent(getIMGFile()->getFilePath(), strEntryData, getEntryOffset());
}

string					CIMGEntry::getEntryData(void)
{
	string strEntryData = CFileUtility::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset(), getEntrySize(), true);
	bool bBigEndian = false;

	if (getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
	{
		if (isCompressed())
		{
			// compressed
			switch (getCompressionAlgorithmId())
			{
			case COMPRESSION_ZLIB:	return CCompressionUtility::decompressZLib(strEntryData, getUncompressedSize());
			case COMPRESSION_LZ4:	return CCompressionUtility::decompressLZ4(strEntryData, getUncompressedSize());
			default:					return "";
			}
		}
		else
		{
			// not compressed
			return strEntryData;
		}
	}
	else if (CStringUtility::unpackUint32(strEntryData.substr(0, 4), bBigEndian) == 0x67A3A1CE)
	{
		// the entry data is compressed with LZO 1X 999 compression
		uint32 uiSeek = 0;

		uint32 uiMagicNumber = CStringUtility::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
		uint32 uiChecksum = CStringUtility::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
		uint32 uiCompressionDataSize = CStringUtility::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);
		uiSeek += 12;

		string strUncompressedData = "";
		uint32 uiByteCountRead = 0;
		while (uiByteCountRead < uiCompressionDataSize)
		{
			uint32 uiUnknown1 = CStringUtility::unpackUint32(strEntryData.substr(uiSeek, 4), bBigEndian);
			uint32 uiUncompressedBlockSize = CStringUtility::unpackUint32(strEntryData.substr(uiSeek + 4, 4), bBigEndian);
			uint32 uiCompressedBlockSize = CStringUtility::unpackUint32(strEntryData.substr(uiSeek + 8, 4), bBigEndian);

			string strCompressedBlock = strEntryData.substr(uiSeek + 12, uiCompressedBlockSize);
			strUncompressedData += CCompressionUtility::decompressLZO1X(strCompressedBlock, uiUncompressedBlockSize);

			uiSeek += 12 + uiCompressedBlockSize;
			uiByteCountRead += 12 + uiCompressedBlockSize;
		}

		return strUncompressedData;
	}
	else
	{
		return strEntryData;
	}
}

string					CIMGEntry::getEntrySubData(uint32 uiStart, uint32 uiLength)
{
	if (getIMGFile()->getIMGVersion() == IMG_FASTMAN92 && isCompressed())
	{
		// compressed
		const uint8 ucZLibBlockSize = 16;
		uint32 uiStart2 = floor((float32)uiStart / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiLength2 = ceil((float32)uiLength / (float32) ucZLibBlockSize) * ucZLibBlockSize;
		uint32 uiStartOffset = uiStart - uiStart2;
		/*
		CDebugger::log("uiStart: " + CStringUtility::toString(uiStart));
		CDebugger::log("uiLength: " + CStringUtility::toString(uiLength));
		CDebugger::log("uiStart2: " + CStringUtility::toString(uiStart2));
		CDebugger::log("uiLength2: " + CStringUtility::toString(uiLength2));
		CDebugger::log("uiStartOffset: " + CStringUtility::toString(uiStartOffset));
		*/
		string strEntrySubData = CFileUtility::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart2, uiLength2, true);
		//strEntrySubData = CIMGManager::decompressZLib(strEntrySubData, uiLength2);
		switch (getCompressionAlgorithmId())
		{
		case COMPRESSION_ZLIB:	return CCompressionUtility::decompressZLib(strEntrySubData, getUncompressedSize());
		case COMPRESSION_LZ4:	return CCompressionUtility::decompressLZ4(strEntrySubData, getUncompressedSize());
		}
		return strEntrySubData.substr(uiStartOffset, uiLength);
	}
	else
	{
		// not compressed
		return CFileUtility::getFileSubContent(getIMGFile()->getFilePath(), getEntryOffset() + uiStart, uiLength, true);
	}
}

void					CIMGEntry::saveEntry(string strFilePath)
{
	CFileUtility::storeFile(strFilePath, getEntryData(), false, true);
}

void					CIMGEntry::saveEntryByMemory(string strFilePath, string& strEntryData)
{
	CFileUtility::storeFile(strFilePath, strEntryData, false, true);
}

uint32			CIMGEntry::getEntryDataPadLength(uint32 uiUnpaddedDataLength)
{
	return (uint32)(ceil((float32)uiUnpaddedDataLength / (float32)2048.0f) * (float32)2048.0f);
}

void					CIMGEntry::setRWVersionByVersionCC(uint32 uiRWVersionCC)
{
	setRWVersion(CRWManager::getInstance()->getVersionManager()->getEntryByVersionCC(uiRWVersionCC));
}

void					CIMGEntry::setRageResourceTypeByIdentifier(uint32 uiResourceType)
{
	setRageResourceType(CRageManager::getInstance()->getResourceTypeManager()->getResourceTypeByIdentifier(uiResourceType));
}

uint32			CIMGEntry::getVersion3IMGSizeDeduction(uint32 uiDataLength)
{
	return 2048 - (uiDataLength % 2048);
}

bool					CIMGEntry::isModelFile(void)
{
	return CPathUtility::isModelExtension(CPathUtility::getFileExtension(getEntryName()));
}

bool					CIMGEntry::isTextureFile(void)
{
	return CPathUtility::isTextureExtension(CPathUtility::getFileExtension(getEntryName()));
}

bool					CIMGEntry::isCollisionFile(void)
{
	return CPathUtility::isCollisionExtension(CPathUtility::getFileExtension(getEntryName()));
}

bool					CIMGEntry::isDFFFile(void)
{
	return CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName())) == "DFF";
}

bool					CIMGEntry::isCOLFile(void)
{
	return CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName())) == "COL";
}

bool					CIMGEntry::isTXDFile(void)
{
	return CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName())) == "TXD";
}

bool					CIMGEntry::isWTDFile(void)
{
	return CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName())) == "WTD";
}

/*
string					CIMGEntry::getEntryDecompressedData(void)
{

}
string					CIMGEntry::getEntryCompressedData(void)
{

}
*/

void					CIMGEntry::applyCompression(eCompressionAlgorithm eCompressionAlgorithmValue, uint32 uiCompressionLevel)
{
	string strEntryData = getEntryData();
	setCompressionAlgorithmId(eCompressionAlgorithmValue);
	setCompressionLevel(uiCompressionLevel);
	setEntryData(strEntryData);
}

bool					CIMGEntry::doesHaveUnknownVersion(void)
{
	if (isEncrypted() || getIMGFile()->isEncrypted())
	{
		return false;
	}

	string strEntryExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName()));
	if (CPathUtility::isModelExtension(strEntryExtension) || strEntryExtension == "TXD")
	{
		if (getRWVersion() == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (strEntryExtension == "COL")
	{
		if (getCOLVersion() == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (strEntryExtension == "IPL")
	{
		return false;
	}
	else if (strEntryExtension == "IFP")
	{
		return false;
	}
	else if (strEntryExtension == "DAT")
	{
		return false;
	}
	return true;
}

CIMGEntry*				CIMGEntry::clone(CIMGFormat *pIMGFile)
{
	CIMGEntry *pClonedIMGEntry = new CIMGEntry(pIMGFile);

	pClonedIMGEntry->setEntryOffsetInSectors(getEntryOffsetInSectors());
	pClonedIMGEntry->setEntrySize(getEntrySize());
	pClonedIMGEntry->setEntryName(getEntryName());
	
	if (isTXDFile() || isDFFFile())
	{
		pClonedIMGEntry->setRWVersion(getRWVersion());
	}
	else if (isCOLFile())
	{
		pClonedIMGEntry->setCOLVersion(getCOLVersion());
	}

	if (pIMGFile->getIMGVersion() == IMG_3)
	{
		pClonedIMGEntry->setRageResourceType(getRageResourceType());
		pClonedIMGEntry->setFlags(getFlags());
	}
	else if (pIMGFile->getIMGVersion() == IMG_FASTMAN92)
	{
		pClonedIMGEntry->setUncompressedSize(getUncompressedSize());
		pClonedIMGEntry->setFlags(getFlags());
	}

	pClonedIMGEntry->setNewEntry(isNewEntry());
	pClonedIMGEntry->setReplacedEntry(isReplacedEntry());
	pClonedIMGEntry->setProtectedEntry(isProtectedEntry());
	pClonedIMGEntry->setFileCreationDate(getFileCreationDate());
	pClonedIMGEntry->setCompressionAlgorithmId(getCompressionAlgorithmId());

	return pClonedIMGEntry;
}

bool				CIMGEntry::canBeRead(void)
{
	if (getIMGFile()->isEncrypted())
	{
		return false;
	}

	if (isEncrypted())
	{
		return false;
	}

	if (isCompressed() && getCompressionAlgorithmId() == COMPRESSION_UNKNOWN)
	{
		return false;
	}

	return true;
}

std::string				CIMGEntry::getVersionText(void)
{
	string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(getEntryName()));

	if (getIMGFile()->getIMGVersion() == IMG_3)
	{
		if (getRageResourceType() == nullptr)
		{
			return CLocalizationManager::getInstance()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown");
		}
		return getRageResourceType()->getResourceName();
	}
	else
	{
		if (strExtensionUpper == "TXD" || CPathUtility::isModelExtension(strExtensionUpper))
		{
			if (getRWVersion() == nullptr)
			{
				return CLocalizationManager::getInstance()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown");
			}
			return getRWVersion()->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(getRWVersion()->getLocalizationKey()) + ")";
		}
		else if (strExtensionUpper == "COL")
		{
			return CCOLManager::getCOLVersionText(getCOLVersion());
		}
	}

	return strExtensionUpper;
}