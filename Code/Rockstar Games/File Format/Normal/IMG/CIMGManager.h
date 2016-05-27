#ifndef CIMGManager_H
#define CIMGManager_H

#include "Types.h"
#include "CFormatManager.h"
#include "CSingleton.h"
#include "CIMGFormat.h"
#include "Engine/RAGE/CRageResourceType.h"
#include "Game/ePlatformedGame.h"
#include <string>
#include <vector>

class CRageResource;

class CIMGManager : public CFormatManager<CIMGFormat>, public CSingleton<CIMGManager>
{
public:
	CIMGManager(void);
	
	void								init(void);
	void								uninit(void);

	static std::string					getIMGVersionName(eIMGVersion eVersion, bool bIsEncrypted = false);
	static std::string					getIMGVersionGames(eIMGVersion eVersion);
	static std::string					getIMGVersionNameWithGames(eIMGVersion eVersion, bool bIsEncrypted = false);
	static uint32						getIMGEntryCount(std::string& strIMGFilePath, eIMGVersion eIMGVersion = IMG_UNKNOWN);

	static eIMGVersion					detectIMGVersion(std::string& strIMGFilePath);
	static bool							detectIMGEncryptionState(std::string& strIMGFilePath);

	static std::string					encryptVersion3IMGString(std::string strData);
	static std::string					decryptVersion3IMGString(std::string strData);

	static std::vector<std::string>		getDefaultGameIMGSubPaths(ePlatformedGame ePlatformedGameValue);

	static std::string					getCompressionTypeText(eCompressionAlgorithm eCompressionAlgorithmValue);
	static std::string					getEncryptionText(bool bIsEncrypted);
};

#endif