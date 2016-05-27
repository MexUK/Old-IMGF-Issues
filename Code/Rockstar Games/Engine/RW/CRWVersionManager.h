#ifndef CRWVersionManager_H
#define CRWVersionManager_H

#include "CManager.h"
#include "CRWVersion.h"
#include "Pool/CVectorPool.h"
#include <vector>
#include <unordered_map>

class CRWVersionManager : public CManager, public CVectorPool<CRWVersion*>
{
public:
	void											init(void);
	void											uninit(void);

	CRWVersion*										getEntryByVersionCC(uint32 uiVersionCC);
	CRWVersion*										getEntryByVersionId(eRWVersion eRWVersion);
	CRWVersion*										getRWVersionFromGame(ePlatformedGame ePlatformedGame);
	std::unordered_map<uint32, std::vector<std::string>>		getVersionNames(void);

private:
	void											initRWVersions(void);
};

#endif