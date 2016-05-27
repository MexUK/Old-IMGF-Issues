#ifndef CCOLManager_H
#define CCOLManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include "eCOLVersion.h"
#include "CCOLVersion.h"
#include "Game/ePlatformedGame.h"
#include "CCOLEntry.h"
#include <string>
#include <unordered_map>

class CCOLFormat;
class CCOLVersionManager;

class CCOLManager : public CFormatManager<CCOLFormat>, public CSingleton<CCOLManager>
{
public:
	CCOLManager(void);
	~CCOLManager(void);

	void					init(void);
	void					uninit(void);

	void					initGameMaterials(void);

	CCOLVersionManager*		getVersionManager(void) { return m_pVersionManager; }

	static CCOLVersion*		getCOLVersionFromFourCC(std::string strFourCC);
	static std::string		getFourCCFromCOLVersion(eCOLVersion eCOLVersionValue);

	static std::string		getCOLVersionText(eCOLVersion eCOLVersionValue);
	static std::string		getCOLVersionText(CCOLVersion* pCOLVersion) { return getCOLVersionText(pCOLVersion->getVersionId()); }

	static uint32			getEntryHeaderSizeForPacking(eCOLVersion eCOLVersionValue);

private:
	CCOLVersionManager*		m_pVersionManager;
	
	//std::unordered_map<ePlatformedGame, std::unordered_map<std::string, uint32>> umapGameMaterialIds; // todo
};

#endif