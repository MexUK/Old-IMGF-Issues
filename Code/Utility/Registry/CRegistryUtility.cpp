#include <Windows.h>
#include "CRegistryUtility.h"
#include "../String/CStringUtility.h"

using namespace std;

void			CRegistryUtility::setRegistryValueString(string strKey1, string strKey2, string strValue)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_FILE_NOT_FOUND)
	{
		RegCreateKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	RegSetValueEx(hKey, CStringUtility::convertStdStringToStdWString(strKey2).c_str(), 0, REG_SZ, (BYTE*)CStringUtility::convertStdStringToStdWString(strValue).c_str(), strValue.length()*sizeof(wchar_t));
	RegCloseKey(hKey);
}

string			CRegistryUtility::getRegistryValueString(string strKey1, string strKey2)
{
	wchar_t szBuffer[16383];
	DWORD uiBufferSize = 16383;

	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), &hKey) != ERROR_SUCCESS)
	{
		return "";
	}
	if (RegQueryValueEx(hKey, CStringUtility::convertStdStringToStdWString(strKey2).c_str(), NULL, NULL, (LPBYTE)szBuffer, &uiBufferSize) != ERROR_SUCCESS)
	{
		return "";
	}
	RegCloseKey(hKey);

	szBuffer[uiBufferSize] = '\0';

	string strData = CStringUtility::convertStdWStringToStdString(szBuffer);
	return strData;
}

void			CRegistryUtility::setRegistryValueInt(string strKey1, string strKey2, int iValue)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_FILE_NOT_FOUND)
	{
		RegCreateKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}
	RegSetValueEx(hKey, CStringUtility::convertStdStringToStdWString(strKey2).c_str(), 0, REG_DWORD, (BYTE*)&iValue, 4);
	RegCloseKey(hKey);
}

int				CRegistryUtility::getRegistryValueInt(string strKey1, string strKey2)
{
	DWORD uiValue = 0;
	DWORD uiBufferSize = 4;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		return 0;
	}
	if (RegQueryValueEx(hKey, CStringUtility::convertStdStringToStdWString(strKey2).c_str(), NULL, NULL, (LPBYTE)&uiValue, &uiBufferSize) != ERROR_SUCCESS)
	{
		return 0;
	}
	RegCloseKey(hKey);

	return uiValue;
}

void			CRegistryUtility::removeRegistryValue(string strKey1, string strKey2)
{
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, CStringUtility::convertStdStringToStdWString(strKey1).c_str(), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS)
	{
		return;
	}

	RegDeleteValue(hKey, CStringUtility::convertStdStringToStdWString(strKey2).c_str());
	RegCloseKey(hKey);
}

void			CRegistryUtility::assoicateFileExtension(string strFileExtension, string strExePath)
{
	strFileExtension = "." + strFileExtension;

	string strDecription = "GTA IMG File";
	string strAction = "Open GTA IMG file";
	string strRegistryPath = strFileExtension + "\\shell\\" + strAction + "\\command\\";

	HKEY hkey;

	// 1: Create subkey for extension -> HKEY_CLASSES_ROOT\.<extension>
	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, CStringUtility::convertStdStringToStdWString(strFileExtension).c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
	{
		return;
	}
	RegSetValueEx(hkey, _T(""), 0, REG_SZ, (BYTE*)strDecription.c_str(), strDecription.length()); // default vlaue is description of file extension
	RegCloseKey(hkey);



	// 2: Create Subkeys for action ( "Open with my program" )
	// HKEY_CLASSES_ROOT\.<extension>\Shell\\open with my program\\command
	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, CStringUtility::convertStdStringToStdWString(strRegistryPath).c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hkey, 0) != ERROR_SUCCESS)
	{
		return;
	}
	RegSetValueEx(hkey, _T(""), 0, REG_SZ, (BYTE*)strExePath.c_str(), strExePath.length());
	RegCloseKey(hkey);
}

void				CRegistryUtility::setSoftwareValueString(string strKey1, string strKey2, string strValue)
{
	return setRegistryValueString("SOFTWARE\\" + strKey1, strKey2, strValue);
}
string				CRegistryUtility::getSoftwareValueString(string strKey1, string strKey2)
{
	return getRegistryValueString("SOFTWARE\\" + strKey1, strKey2);
}
void				CRegistryUtility::setSoftwareValueInt(string strKey1, string strKey2, int iValue)
{
	return setRegistryValueInt("SOFTWARE\\" + strKey1, strKey2, iValue);
}
int					CRegistryUtility::getSoftwareValueInt(string strKey1, string strKey2)
{
	return getRegistryValueInt("SOFTWARE\\" + strKey1, strKey2);
}
void				CRegistryUtility::removeSoftwareValue(string strKey1, string strKey2)
{
	return removeRegistryValue("SOFTWARE\\" + strKey1, strKey2);
}