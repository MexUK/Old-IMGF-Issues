#ifndef CIMGFormat_H
#define CIMGFormat_H

#include "Types.h"
#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "eIMGVersion.h"
#include "Engine/RW/eRWVersion.h"
#include "CIMGPeekData.h"
#include "eCompressionAlgorithm.h"
#include "IMG/eIMGVersionFastman92CompressionAlgorithm.h"
#include "COL/eCOLVersion.h"
#include "CIMGEntry.h"
#include "Platform/ePlatform.h"
#include "eCompressionAlgorithm.h"
#include <string>
#include <vector>
#include <unordered_map>

class CDataReader;

class CIMGFormat : public CFormat, public CVectorPool<CIMGEntry*>
{
public:
	CIMGFormat(void);

	void											unload(void) {};

	void											unserializeRWVersions(void);
	void											unserializeResourceTypes(void);

	void											setIMGVersion(eIMGVersion eIMGVersion) { m_eIMGVersion = eIMGVersion; }
	eIMGVersion										getIMGVersion(void) { return m_eIMGVersion; }

	void											setEncrypted(bool bEncrypted) { m_bEncrypted = bEncrypted; }
	bool											isEncrypted(void) { return m_bEncrypted; }

	void											setGameType(uint8 ucGameType) { m_ucGameType = ucGameType; }
	uint8											getGameType(void) { return m_ucGameType; }

	void											setPlatform(ePlatform ePlatformValue) { m_ePlatform = ePlatformValue; }
	ePlatform										getPlatform(void) { return m_ePlatform; }

	std::vector<std::string>						getEntryExtensions(void);
	uint32											getEntryExtensionCount(std::string strEntryExtension);
	std::vector<std::string>						getEntryVersions(std::vector<eCOLVersion>& vecCOLVersions, std::vector<eRWVersion>& vecRWVersions);

	static eCompressionAlgorithm					getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(eIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId);

	CIMGEntry*										addEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	CIMGEntry*										addEntryViaData(std::string& strEntryName, std::string& strEntryData);
	void											addEntry(CIMGEntry *pIMGEntry);
	void											addEntries(std::vector<std::string>& vecEntryFilePaths);
	void											addAllEntriesInFolder(std::string& strFolderPath);
	void											removeEntry(CIMGEntry *pIMGEntry);
	CIMGEntry*										replaceEntryViaFile(std::string& strEntryName, std::string& strEntryFilePath, std::string strNewEntryName = "");
	CIMGEntry*										replaceEntryViaData(std::string& strEntryName, std::string& strEntryData, std::string strNewEntryName = "");
	uint32											replaceEntries(std::vector<std::string>& vecPaths, std::vector<std::string>& vecReplacedEntryNames, std::vector<CIMGEntry*>& vecReplacedEntries);
	CIMGEntry*										addOrReplaceEntryViaFile(std::string& strEntryFilePath, std::string strEntryName = "");
	CIMGEntry*										addOrReplaceEntryViaData(std::string& strEntryName, std::string& strEntryData);
	std::unordered_map<CIMGEntry*, std::string>		getAllEntriesData(void); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	std::unordered_map<CIMGEntry*, std::string>		getEntriesData(std::vector<CIMGEntry*>& vecEntries); // not recommended to be used, may use too much memory, better to iterate pIMGFile->getEntries()
	uint32											getNextEntryOffset(void); // in bytes
	std::vector<CIMGEntry*>							getEntriesByExtension(std::string strExtension);
	CIMGEntry*										getEntryByName(std::string& strEntryName); // case-insensitive
	CIMGEntry*										getEntryByNameWithoutExtension(std::string& strEntryNameWithoutExtension); // case-insensitive
	std::vector<std::string>						getEntryNames(void);
	CIMGEntry*										getEntryByHighestOffset(void);
	uint32											getEntryCountForName(std::string& strEntryName); // case-insensitive
	uint32											getEntryCountForCompressionType(eCompressionAlgorithm eCompressionAlgorithmValue);

	static uint32									getEntryPaddedSize(uint32 uiDataLength);
	void											unsetNewAndReplacedFlagForAllEntries(void);

	uint32											merge(std::string& strSecondIMGPath, std::vector<std::string>& vecImportedEntryNames);
	void											split(std::vector<CIMGEntry*>& vecIMGEntries, std::string& strOutPath, eIMGVersion eIMGVersion);
	CIMGFormat*										clone(std::string& strClonedIMGPath);

	void											exportSingle(CIMGEntry *pIMGEntry, std::string& strFolderPath);
	void											exportMultiple(std::vector<CIMGEntry*>& vecIMGEntries, std::string strFolderPath);
	void											exportAll(std::string& strFolderPath);

	bool											doesContainEntryWithUnknownRWVersion(void); // for IMG versions 1 and 2 - todo - should it be changed to without RW to match function below - check func bodys
	std::vector<CIMGEntry*>							getUnknownVersionEntries(void); // for IMG versions 1 and 2

private:
	void											unserialize(void);
	void											serialize(void);

	void											unserializeHeaderComponents(void);
	void											unserializeBodyComponents(void);

	void											serializeHeaderAndBodyComponents(void);

	CIMGPeekData									peekIMGData(void);
	
	void											unserializeVersion1(void);
	void											unserializeVersion2(void);
	void											unserializeVersion3_Encrypted(void);
	void											unserializeVersion3_Unencrypted(void);
	void											unserializeVersionFastman92(void);

	void											serializeVersion1(void);
	void											serializeVersion2(void);
	void											serializeVersion3_Encrypted(void);
	void											serializeVersion3_Unencrypted(void);
	void											serializeVersionFastman92(void);

	uint32											getVersion3NamesLength(void);

private:
	void											addEntryExtensionCount(std::string strEntryExtension);
	void											removeEntryExtensionCount(std::string strEntryExtension);
	void											loadEntryExtensionCounts(void);

private:
	eIMGVersion										m_eIMGVersion;
	ePlatform										m_ePlatform;
	std::unordered_map<std::string, uint32>			m_umapExtensionCounts;
	uint8											m_ucGameType;
	uint8											m_bEncrypted : 1;
};

#endif