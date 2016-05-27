#ifndef CRWManager_H
#define CRWManager_H

#include "CSingleton.h"
#include "CManager.h"
#include "CRWVersion.h"
#include <string>

class CRWVersionManager;

class CRWManager : public CManager, public CSingleton<CRWManager>
{
public:
	CRWManager(void);
	~CRWManager(void);

	void											init(void);
	void											uninit(void);

	CRWVersionManager*								getVersionManager(void) { return m_pVersionManager; }

	static std::string								getRWVersionText(CRWVersion* pRWVersion) { return getRWVersionText(pRWVersion == nullptr ? RW_VERSION_UNKNOWN : pRWVersion->getVersionId()); }
	static std::string								getRWVersionText(eRWVersion eRWVersionValue);

	std::string										getGameName(ePlatformedGame ePlatformedGame);

	void											setRWVersionCCForSerialization(uint32 uiRWVersionCC) { m_uiRWVersionCCForSerialization = uiRWVersionCC; }
	uint32											getRWVersionCCForSerialization(void) { return m_uiRWVersionCCForSerialization; }

private:
	CRWVersionManager*								m_pVersionManager;
	uint32											m_uiRWVersionCCForSerialization;
};

#endif