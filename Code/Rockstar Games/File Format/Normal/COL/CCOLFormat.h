#ifndef CCOLFormat_H
#define CCOLFormat_H

#include "CFormat.h"
#include "Pool/CVectorPool.h"
#include "eCOLVersion.h"
#include "CCOLEntry.h"
#include <string>
#include <vector>

class CCOLVersion;

class CCOLFormat : public CFormat, public CVectorPool<CCOLEntry*>
{
public:
	CCOLFormat(void) : CFormat(true) {};

	void							unload(void);

	void							setCOLVersion(CCOLVersion *pCOLVersion);

	std::vector<std::string>		getModelNames(void);
	CCOLEntry*						getEntryByName(std::string strName);
	std::vector<CCOLEntry*>			getEntriesByModelName(std::string strModelName);

	void							sort(void);
	static bool						sortFunction(CCOLEntry *pCOLEntry1, CCOLEntry *pCOLEntry2);

private:
	void							unserialize(void);
	void							serialize(void);
};

#endif