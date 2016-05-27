#ifndef CCOLVersionManager_H
#define CCOLVersionManager_H

#include "CManager.h"
#include "eCOLVersion.h"
#include "Pool/CVectorPool.h"

class CCOLVersion;

class CCOLVersionManager : public CManager, public CVectorPool<CCOLVersion*>
{
public:
	void			init(void);
	void			uninit(void);

	CCOLVersion*	getEntryByVersionId(eCOLVersion eCOLVersionValue);

private:
	void			initCOLVersions(void);
	void			uninitCOLVersions(void);
};

#endif