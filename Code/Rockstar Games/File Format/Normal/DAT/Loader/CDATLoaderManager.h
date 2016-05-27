#ifndef CDATLoaderManager_H
#define CDATLoaderManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include "eDATLoaderEntryType.h"
#include "Game/ePlatformedGame.h"
#include <string>

class CDATLoaderFormat;

class CDATLoaderManager : public CFormatManager<CDATLoaderFormat>, public CSingleton<CDATLoaderManager>
{
public:
	CDATLoaderManager(void);

	void						init(void);
	void						uninit(void);

	static eDATLoaderEntryType	getDATEntryTypeFromString(std::string strType);
	static std::string			getDefaultGameDATSubPath(ePlatformedGame ePlatformedGameValue);
};

#endif