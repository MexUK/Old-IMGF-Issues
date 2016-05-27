#pragma warning(disable : 4005)

#include "CSettingsManager.h"
#include "CKGM.h"
#include "Globals.h"
#include "CGUIManager.h"
#include "Registry/CRegistryUtility.h"
#include "IMG/CIMGEntry.h"
#include "INI/CINIUtility.h"
#include "String/CStringUtility.h"

using namespace std;

void			CSettingsManager::init(void)
{
	setUsesINIStorage(true);
	setINIFilePath(getKGM()->getInstallationMeta().getLocalAppPath() + "Settings.ini");

	/*
	setUsesINIStorage(false);
	setRegistryKey("KGM\\Settings");
	*/
}
void			CSettingsManager::uninit(void)
{
	//unloadSettings();
}

void			CSettingsManager::loadSettings(void)
{
	vector<string>
		vecSettingNames_Bool,
		vecSettingNames_Int,
		vecSettingNames_String
	;

	vecSettingNames_Bool = {
		"ColouredButtons",
		"ColouredTabs",
		"RebuildConfirmationOnClose",
		"RebuildOnConvert",
		"AutomaticLoggingBasic",
		"AutomaticLoggingExtended",
		"AskBeforeOverwritingFiles",
		"OverwriteOlderFiles",
		"OverwriteNewerFiles",
		"OverwriteProtectedFiles",
		"AutoSave",
		"RemoveOldVersionOnUpdate",
		"AutoCompressionImportReplace"
	};

	vecSettingNames_Int = {
		"Language"
	};

	vecSettingNames_String = {
		"QuickExportPath",
		"AutomaticLoggingPath"
	};

	for (string& strSettingName : vecSettingNames_Bool)
	{
		setSettingBool(strSettingName, getSettingBool_PermanentStroage(strSettingName), true);
	}

	for (string& strSettingName : vecSettingNames_Int)
	{
		setSettingInt(strSettingName, getSettingInt_PermanentStroage(strSettingName), true);
	}

	for (string& strSettingName : vecSettingNames_String)
	{
		setSettingString(strSettingName, getSettingString_PermanentStroage(strSettingName), true);
	}

	if (getSettingInt("Language") == LANGUAGE_UNKNOWN)
	{
		setSettingInt("Language", LANGUAGE_ENGLISH, true);
	}

	if (getSettingBool("ColouredTabs"))
	{
		// todo
		//((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->ModifyStyle(0, TCS_OWNERDRAWFIXED, 0);
	}
	else
	{
		//((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->ModifyStyle(TCS_OWNERDRAWFIXED, 0, 0);
	}

	// todo
	//getKGM()->getDialog()->Invalidate();
	//getKGM()->getDialog()->UpdateWindow();
}
void			CSettingsManager::unloadSettings(void)
{
	m_umapSettings_Bool.clear();
	m_umapSettings_Int.clear();
	m_umapSettings_String.clear();
}
void			CSettingsManager::reloadSettings(void)
{
	unloadSettings();
	loadSettings();
}

void			CSettingsManager::setSettingBool(string strSettingName, bool bValue, bool bMemoryOnly)
{
	m_umapSettings_Bool[strSettingName] = bValue;
	if (!bMemoryOnly)
	{
		setSettingBool_PermanentStroage(strSettingName, bValue);
	}
}
void			CSettingsManager::setSettingInt(string strSettingName, int iValue, bool bMemoryOnly)
{
	m_umapSettings_Int[strSettingName] = iValue;
	if (!bMemoryOnly)
	{
		setSettingInt_PermanentStroage(strSettingName, iValue);
	}
}
void			CSettingsManager::setSettingString(string strSettingName, string strValue, bool bMemoryOnly)
{
	m_umapSettings_String[strSettingName] = strValue;
	if (!bMemoryOnly)
	{
		setSettingString_PermanentStroage(strSettingName, strValue);
	}
}

bool			CSettingsManager::getSettingBool(string strSettingName)
{
	if (m_umapSettings_Bool.find(strSettingName) == m_umapSettings_Bool.end())
	{
		return false;
	}
	else
	{
		return m_umapSettings_Bool[strSettingName];
	}
}
int				CSettingsManager::getSettingInt(string strSettingName)
{
	if (m_umapSettings_Int.find(strSettingName) == m_umapSettings_Int.end())
	{
		return 0;
	}
	else
	{
		return m_umapSettings_Int[strSettingName];
	}
}
string			CSettingsManager::getSettingString(string strSettingName)
{
	if (m_umapSettings_String.find(strSettingName) == m_umapSettings_String.end())
	{
		return "";
	}
	else
	{
		return m_umapSettings_String[strSettingName];
	}
}

// permanent storage
void			CSettingsManager::setSettingBool_PermanentStroage(string strSettingName, bool bValue)
{
	if (doesUseINIStorage())
	{
		CINIUtility::setItem(getINIFilePath(), "Settings", strSettingName, bValue ? "1" : "0");
	}
	else
	{
		CRegistryUtility::setSoftwareValueInt(getRegistryKey(), strSettingName, bValue ? 1 : 0);
	}
}
void			CSettingsManager::setSettingInt_PermanentStroage(string strSettingName, int iValue)
{
	if (doesUseINIStorage())
	{
		CINIUtility::setItem(getINIFilePath(), "Settings", strSettingName, CStringUtility::toString(iValue));
	}
	else
	{
		CRegistryUtility::setSoftwareValueInt("KGM\\Settings", strSettingName, iValue);
	}
}
void			CSettingsManager::setSettingString_PermanentStroage(string strSettingName, string strValue)
{
	if (doesUseINIStorage())
	{
		CINIUtility::setItem(getINIFilePath(), "Settings", strSettingName, strValue);
	}
	else
	{
		CRegistryUtility::setSoftwareValueString("KGM\\Settings", strSettingName, strValue);
	}
}

bool			CSettingsManager::getSettingBool_PermanentStroage(string strSettingName)
{
	if (doesUseINIStorage())
	{
		return CINIUtility::getItem(getINIFilePath(), "Settings", strSettingName) == "1";
	}
	else
	{
		return CRegistryUtility::getSoftwareValueInt("KGM\\Settings", strSettingName) == 1;
	}
}
int				CSettingsManager::getSettingInt_PermanentStroage(string strSettingName)
{
	if (doesUseINIStorage())
	{
		return CStringUtility::toNumber(CINIUtility::getItem(getINIFilePath(), "Settings", strSettingName));
	}
	else
	{
		return CRegistryUtility::getSoftwareValueInt("KGM\\Settings", strSettingName);
	}
}
string			CSettingsManager::getSettingString_PermanentStroage(string strSettingName)
{
	if (doesUseINIStorage())
	{
		return CINIUtility::getItem(getINIFilePath(), "Settings", strSettingName);
	}
	else
	{
		return CRegistryUtility::getSoftwareValueString("KGM\\Settings", strSettingName);
	}
}