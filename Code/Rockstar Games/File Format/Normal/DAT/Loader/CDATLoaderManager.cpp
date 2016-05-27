#include "CDATLoaderManager.h"
#include "String/CStringUtility.h"
#include "Vector/CVectorUtility.h"
#include "Testing/CDebugger.h"

using namespace std;

CDATLoaderManager::CDATLoaderManager(void)
{
}

void				CDATLoaderManager::init(void)
{
}
void				CDATLoaderManager::uninit(void)
{
}

eDATLoaderEntryType CDATLoaderManager::getDATEntryTypeFromString(string strType)
{
	strType = CStringUtility::toUpperCase(strType);
	if (strType == "IMG")
		return DAT_LOADER_IMG;
	if (strType == "CDIMAGE")
		return DAT_LOADER_CDIMAGE;
	if (strType == "IMGLIST")
		return DAT_LOADER_IMGLIST;
	if (strType == "WATER")
		return DAT_LOADER_WATER;
	if (strType == "IDE")
		return DAT_LOADER_IDE;
	if (strType == "HIERFILE")
		return DAT_LOADER_HIERFILE;
	if (strType == "COLFILE")
		return DAT_LOADER_COLFILE;
	if (strType == "IPL")
		return DAT_LOADER_IPL;
	if (strType == "MAPZONE")
		return DAT_LOADER_MAPZONE;
	if (strType == "TEXDICTION")
		return DAT_LOADER_TEXDICTION;
	if (strType == "MODELFILE")
		return DAT_LOADER_MODELFILE;
	if (strType == "SPLASH")
		return DAT_LOADER_SPLASH;
	if (strType == "EXIT")
		return DAT_LOADER_EXIT;
	return DAT_LOADER_UNKNOWN;
}

string			CDATLoaderManager::getDefaultGameDATSubPath(ePlatformedGame ePlatformedGameValue)
{
	switch (ePlatformedGameValue)
	{
	case PLATFORMED_GAME_PC_GTA_III:	return "data/gta3.dat";
	case PLATFORMED_GAME_PC_GTA_VC:		return "data/gta_vc.dat";
	case PLATFORMED_GAME_PC_GTA_SA:		return "data/gta.dat";
	case PLATFORMED_GAME_PC_GTA_IV:		return "common/data/gta.dat";
	case PLATFORMED_GAME_PC_SOL:		return "sol/gta_sol.dat";
	}
	return "";
}