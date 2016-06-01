#include "CRecentlyOpenManager.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "Vector/CVectorUtility.h"
#include "Registry/CRegistryUtility.h"
#include "Globals.h"
#include "CKGM.h"
#include "Testing/CDebugger.h"
#include "IMG/CIMGEntry.h"
#include "Localization/CLocalizationManager.h"
#include "GUI/Window/CWindow.h"

using namespace std;

void					CRecentlyOpenManager::init(void)
{
	loadRecentlyOpenEntries();
}
void					CRecentlyOpenManager::uninit(void)
{
	unloadRecentlyOpenEntries();
}

void					CRecentlyOpenManager::loadRecentlyOpenEntries(void)
{
	/*
	todo

	removeAllEntries();

	for (auto it : getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer())
	{
		DeleteMenu(getKGM()->m_hSubMenu_File_OpenRecent, it.first, 0);
	}
	getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer().clear();
	DeleteMenu(getKGM()->m_hSubMenu_File_OpenRecent, 1880, 0);
	DeleteMenu(getKGM()->m_hSubMenu_File_OpenRecent, 1881, 0);

	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	for (int32 i = uiRecentlyOpenedCount; i >= 1; i--)
	{
		string strIMGPath = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i));

		AppendMenu(getKGM()->m_hSubMenu_File_OpenRecent, MF_STRING, 1800 + i, CStringUtility::convertStdStringToStdWString(CStringUtility::toString((uiRecentlyOpenedCount - i) + 1) + ") " + strIMGPath).c_str());

		getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer()[1800 + i] = strIMGPath;

		CRecentlyOpenEntry *pRecentlyOpenEntry = new CRecentlyOpenEntry;
		pRecentlyOpenEntry->m_strPath = strIMGPath;
		addEntry(pRecentlyOpenEntry);
	}

	if (uiRecentlyOpenedCount == 0)
	{
		AppendMenu(getKGM()->m_hSubMenu_File_OpenRecent, MF_STRING | MF_DISABLED, 1881, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_RecentlyOpenFiles_NoFiles").c_str());
	}

	AppendMenu(getKGM()->m_hSubMenu_File_OpenRecent, MF_STRING, 1880, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_RecentlyOpenFiles_Clear").c_str());
	*/
}
void					CRecentlyOpenManager::unloadRecentlyOpenEntries(void)
{
	removeAllEntries();
}

CRecentlyOpenEntry*		CRecentlyOpenManager::addRecentlyOpenEntry(std::string strPath)
{
	if (doesRecentlyOpenEntryExist(strPath))
	{
		moveRecentlyOpenEntryToTop(strPath);
		loadRecentlyOpenEntries();
		CRecentlyOpenEntry *pRecentlyOpenEntry = getRecentlyOpenEntryByPath(strPath);
		return pRecentlyOpenEntry;
	}

	strPath = CStringUtility::replace(strPath, "/", "\\");

	CRecentlyOpenEntry *pRecentlyOpenEntry = new CRecentlyOpenEntry;
	pRecentlyOpenEntry->m_strPath = strPath;
	addEntry(pRecentlyOpenEntry);

	uint32 uiRecentlyOpenedMaxCount = 15;
	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");

	if (uiRecentlyOpenedCount == uiRecentlyOpenedMaxCount)
	{
		for (uint32 i = 2; i <= uiRecentlyOpenedMaxCount; i++)
		{
			string strIMGPath2 = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i));
			CRegistryUtility::setSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i - 1), strIMGPath2);
		}
		CRegistryUtility::setSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(uiRecentlyOpenedMaxCount), strPath);
	}
	else
	{
		CRegistryUtility::setSoftwareValueInt("KGM\\RecentlyOpened", "Count", uiRecentlyOpenedCount + 1);
		CRegistryUtility::setSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(uiRecentlyOpenedCount + 1), strPath);
	}

	return pRecentlyOpenEntry;
}

void					CRecentlyOpenManager::removeRecentlyOpenedEntries(void)
{
	/*
	todo
	for (auto it : getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer())
	{
		DeleteMenu(getKGM()->m_hSubMenu_File_OpenRecent, it.first, 0);
	}
	getKGM()->getRecentlyOpenManager()->getRecentlyOpenedFilesContainer().clear();

	removeAllEntries();
	getKGM()->getActiveWindow()->clearOpenLastFilename();

	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	for (uint32 i = 1; i <= uiRecentlyOpenedCount; i++)
	{
		CRegistryUtility::removeSoftwareValue("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i));
	}
	CRegistryUtility::setSoftwareValueInt("KGM\\RecentlyOpened", "Count", 0);

	loadRecentlyOpenEntries();
	*/
}

void					CRecentlyOpenManager::removeRecentlyOpenEntry(CRecentlyOpenEntry *pRecentlyOpenEntry)
{
	uint32 uiRecentlyOpenedIndex = getRecentlyOpenedFileIndex(pRecentlyOpenEntry->getPath());
	CRegistryUtility::removeSoftwareValue("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(uiRecentlyOpenedIndex));

	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	for (uint32 i = uiRecentlyOpenedIndex; i <= uiRecentlyOpenedCount; i++)
	{
		string strIMGPath2 = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i + 1));
		CRegistryUtility::setSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i), strIMGPath2);
	}
	CRegistryUtility::setSoftwareValueInt("KGM\\RecentlyOpened", "Count", uiRecentlyOpenedCount - 1);

	removeEntry(pRecentlyOpenEntry);
}

uint32			CRecentlyOpenManager::getRecentlyOpenedFileIndex(string strIMGPath)
{
	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	for (uint32 i = 1; i <= uiRecentlyOpenedCount; i++)
	{
		string strIMGPath2 = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(i));
		if (strIMGPath2 == strIMGPath)
		{
			return i;
		}
	}
	return -1;
}

CRecentlyOpenEntry*		CRecentlyOpenManager::getRecentlyOpenEntryByPath(string strPath)
{
	strPath = CStringUtility::toUpperCase(strPath);
	strPath = CStringUtility::replace(strPath, "\\", "/");
	for (auto pRecentlyOpenEntry : getEntries())
	{
		if (CStringUtility::replace(CStringUtility::toUpperCase(pRecentlyOpenEntry->m_strPath), "\\", "/") == strPath)
		{
			return pRecentlyOpenEntry;
		}
	}
	return nullptr;
}

bool					CRecentlyOpenManager::doesRecentlyOpenEntryExist(std::string strPath)
{
	return getRecentlyOpenEntryByPath(strPath) != nullptr;
}

string					CRecentlyOpenManager::getLastOpenEntry(void)
{
	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	if (uiRecentlyOpenedCount == 0)
	{
		return "";
	}

	string strIMGPath = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(uiRecentlyOpenedCount));
	return strIMGPath;
}

void					CRecentlyOpenManager::moveRecentlyOpenEntryToTop(string strIMGPath)
{
	removeRecentlyOpenEntry(getRecentlyOpenEntryByPath(strIMGPath));
	addRecentlyOpenEntry(strIMGPath);
}