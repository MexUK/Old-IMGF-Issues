#include "CInstallationMeta.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "Registry/CRegistryUtility.h"
#include <Shlobj.h>
#include <sstream>

using namespace std;

string				CInstallationMeta::getProgramFilesPath(void)
{
	return CPathUtility::addSlashToEnd(CRegistryUtility::getSoftwareValueString("KGM\\InternalSettings", "InstallationPath"));
}

string				CInstallationMeta::getLocalAppPath(void)
{
	wchar_t *pLocalAppDataFolderPath = 0;
	SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pLocalAppDataFolderPath);
	wstringstream ss;
	ss << pLocalAppDataFolderPath;
	string strLocalAppFolderPath = CPathUtility::addSlashToEnd(CStringUtility::convertStdWStringToStdString(ss.str()));
	CoTaskMemFree(static_cast<void*>(pLocalAppDataFolderPath));
	return CPathUtility::addSlashToEnd(strLocalAppFolderPath) + "KGM/";
}