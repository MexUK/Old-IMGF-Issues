#ifndef CIDEManager_H
#define CIDEManager_H

#include "CFormatManager.h"
#include "CSingleton.h"
#include <string>
#include <vector>

class CIDEFormat;

class CIDEManager : public CFormatManager<CIDEFormat>, public CSingleton<CIDEManager>
{
public:
	CIDEManager(void);
	
	void								init(void);
	void								uninit(void);

	static std::vector<std::string>		getIDEEntryNamesWithoutExtension(std::vector<std::string> vecIDEPaths, bool bModelNames = true, bool bTXDNames = true);
};

#endif