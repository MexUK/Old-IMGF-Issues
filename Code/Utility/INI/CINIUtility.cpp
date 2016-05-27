#include "CINIUtility.h"
#include "iniFile.h"

bool			CINIUtility::setItem(std::string strPath, std::string strSection, std::string strKey, std::string strValue)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	bool bResult = iniFile.SetValue(strSection, strKey, strValue);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::setItem(std::string strPath, std::string strSection, uint32 uiKey, std::string strValue)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	uint32 uiSection = iniFile.FindKey(strSection);
	bool bResult = iniFile.SetValue(uiSection, uiKey, strValue);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::setItem(std::string strPath, uint32 uiSection, std::string strKey, std::string strValue)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	uint32 uiKey = iniFile.FindValue(uiSection, strKey);
	bool bResult = iniFile.SetValue(uiSection, uiKey, strValue);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::setItem(std::string strPath, uint32 uiSection, uint32 uiKey, std::string strValue)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	bool bResult = iniFile.SetValue(uiSection, uiKey, strValue);
	iniFile.WriteFile();
	return bResult;
}
std::string		CINIUtility::getItem(std::string strPath, std::string strSection, std::string strKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	return iniFile.GetValue(strSection, strKey);
}
std::string		CINIUtility::getItem(std::string strPath, std::string strSection, uint32 uiKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	uint32 uiSection = iniFile.FindKey(strSection);
	return iniFile.GetValue(uiSection, uiKey);
}
std::string		CINIUtility::getItem(std::string strPath, uint32 uiSection, std::string strKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	uint32 uiKey = iniFile.FindValue(uiSection, strKey);
	return iniFile.GetValue(uiSection, uiKey);
}
std::string		CINIUtility::getItem(std::string strPath, uint32 uiSection, uint32 uiKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	return iniFile.GetValue(uiSection, uiKey);
}
bool			CINIUtility::removeItem(std::string strPath, std::string strSection, std::string strKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	bool bResult = iniFile.DeleteValue(strSection, strKey);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::removeItem(std::string strPath, std::string strSection, uint32 uiKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::string strKey = iniFile.GetValueName(strSection, uiKey);
	bool bResult = iniFile.DeleteValue(strSection, strKey);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::removeItem(std::string strPath, uint32 uiSection, std::string strKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::string strSection = iniFile.GetKeyName(uiSection);
	bool bResult = iniFile.DeleteValue(strSection, strKey);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::removeItem(std::string strPath, uint32 uiSection, uint32 uiKey)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::string strSection = iniFile.GetKeyName(uiSection);
	std::string strKey = iniFile.GetValueName(uiSection, uiKey);
	bool bResult = iniFile.DeleteValue(strSection, strKey);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::doesItemExist(std::string strPath, std::string strSection, std::string strKey)
{
	return getItem(strPath, strSection, strKey) != "";
}
bool			CINIUtility::doesItemExist(std::string strPath, std::string strSection, uint32 uiKey)
{
	return getItem(strPath, strSection, uiKey) != "";
}
bool			CINIUtility::doesItemExist(std::string strPath, uint32 uiSection, std::string strKey)
{
	return getItem(strPath, uiSection, strKey) != "";
}
bool			CINIUtility::doesItemExist(std::string strPath, uint32 uiSection, uint32 uiKey)
{
	return getItem(strPath, uiSection, uiKey) != "";
}
std::vector<std::string>	CINIUtility::getItems(std::string strPath, std::string strSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::vector<std::string> vecItems;
	for (uint32 i = 0, j = iniFile.GetNumValues(strSection); i < j; i++)
	{
		vecItems.push_back(iniFile.GetValueName(strSection, i));
	}
	return vecItems;
}
std::vector<std::string>	CINIUtility::getItems(std::string strPath, uint32 uiSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::vector<std::string> vecItems;
	for (uint32 i = 0, j = iniFile.GetNumValues(uiSection); i < j; i++)
	{
		vecItems.push_back(iniFile.GetValueName(uiSection, i));
	}
	return vecItems;
}
void			CINIUtility::addSection(std::string strPath, std::string strSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	iniFile.AddKeyName(strSection);
	iniFile.WriteFile();
}
bool			CINIUtility::removeSection(std::string strPath, std::string strSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	bool bResult = iniFile.DeleteKey(strSection);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::removeSection(std::string strPath, uint32 uiSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::string strSection = iniFile.GetKeyName(uiSection);
	bool bResult = iniFile.DeleteKey(strSection);
	iniFile.WriteFile();
	return bResult;
}
bool			CINIUtility::doesSectionExist(std::string strPath, std::string strSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	return iniFile.FindKey(strSection) != CIniFile::noID;
}
bool			CINIUtility::doesSectionExist(std::string strPath, uint32 uiSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	return iniFile.GetKeyName(uiSection) != "";
}
std::string		CINIUtility::getSection(std::string strPath, uint32 uiSection)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	return iniFile.GetKeyName(uiSection);
}
std::vector<std::string>	CINIUtility::getSections(std::string strPath)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	std::vector<std::string> vecSections;
	for (uint32 i = 0, j = iniFile.GetNumKeys(); i < j; i++)
	{
		vecSections.push_back(iniFile.GetKeyName(i));
	}
	return vecSections;
}
void			CINIUtility::clearFile(std::string strPath)
{
	CIniFile iniFile(strPath);
	iniFile.ReadFile();
	iniFile.Clear();
	iniFile.WriteFile();
}