#include "CPathUtility.h"
#include "../String/CStringUtility.h"
#include "../Vector/CVectorUtility.h"

using namespace std;

string			CPathUtility::getFileName(string& strPath)
{
	strPath = CStringUtility::replace(strPath, "\\", "/");
	size_t uiPosition = strPath.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return strPath;
	}
	return strPath.substr(uiPosition + 1);
}

string			CPathUtility::getFolderName(string& strPath)
{
	return CPathUtility::getFileName(CPathUtility::removeSlashFromEnd(strPath));
}

string			CPathUtility::getDirectory(string& strPath)
{
	strPath = CStringUtility::replace(strPath, "\\", "/");
	size_t uiPosition = strPath.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return "";
	}
	return strPath.substr(0, uiPosition + 1);
}

string			CPathUtility::removeFileName(string& strPath)
{
	strPath = CStringUtility::replace(strPath, "\\", "/");
	size_t uiPosition = strPath.find_last_of("/");
	if (uiPosition == string::npos)
	{
		return strPath;
	}
	return strPath.substr(0, uiPosition + 1);
}

string			CPathUtility::getFileExtension(string& strPath)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return "";
	}
	return strPath.substr(uiPosition + 1);
}

string			CPathUtility::replaceFileExtension(string strPath, string strExtension)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return strPath + "." + strExtension;
	}
	return strPath.substr(0, uiPosition + 1) + strExtension;
}

string			CPathUtility::removeFileExtension(string& strPath)
{
	size_t uiPosition = strPath.find_last_of(".");
	if (uiPosition == string::npos)
	{
		return strPath;
	}
	return strPath.substr(0, uiPosition);
}

string			CPathUtility::addSlashToEnd(string& strPath)
{
	if (strPath.c_str()[strPath.length() - 1] != '\\' && strPath.c_str()[strPath.length() - 1] != '/')
	{
		strPath += "\\";
	}
	return strPath;
}

string			CPathUtility::removeSlashFromEnd(string& strPath)
{
	if (strPath.c_str()[strPath.length() - 1] == '\\' || strPath.c_str()[strPath.length() - 1] == '/')
	{
		return strPath.substr(0, strPath.length() - 1);
	}
	return strPath;
}
string			CPathUtility::addSlashToFront(string& strPath)
{
	if (strPath.c_str()[0] != '\\' && strPath.c_str()[0] != '/')
	{
		strPath = "\\" + strPath;
	}
	return strPath;
}
string			CPathUtility::removeSlashFromFront(string& strPath)
{
	if (strPath.c_str()[0] == '\\' || strPath.c_str()[0] == '/')
	{
		return strPath.substr(1);
	}
	return strPath;
}

string			CPathUtility::getNextFileName(string& strFilePath, uint32 uiFilenameIndex, string strNextSuffix)
{
	if (uiFilenameIndex == 0)
	{
		return strFilePath;
	}
	return removeFileExtension(strFilePath) + strNextSuffix + CStringUtility::toString(uiFilenameIndex + 1) + "." + getFileExtension(strFilePath);
}

vector<string>	CPathUtility::getUniqueFilePaths(vector<string>& vecFilePaths, vector<string>& vecFileNames)
{
	vecFileNames = CVectorUtility::toUpperCase(vecFileNames);
	vector<string> vecUniqueFilePaths;
	for (auto strFilePath : vecFilePaths)
	{
		if (!CVectorUtility::isIn(vecFileNames, CStringUtility::toUpperCase(getFileName(strFilePath))))
		{
			vecUniqueFilePaths.push_back(strFilePath);
		}
	}
	return vecUniqueFilePaths;
}

bool			CPathUtility::isModelExtension(string& strExtension)
{
	string strExtension2 = CStringUtility::toUpperCase(strExtension);
	return strExtension2 == "DFF" || strExtension2 == "BSP"/* || strExtension2 == "WDR"*/;
}

bool			CPathUtility::isTextureExtension(string& strExtension)
{
	string strExtension2 = CStringUtility::toUpperCase(strExtension);
	return strExtension2 == "TXD" || strExtension2 == "WTD";
}

bool			CPathUtility::isCollisionExtension(string& strExtension)
{
	string strExtension2 = CStringUtility::toUpperCase(strExtension);
	return strExtension2 == "COL" || strExtension2 == "WBN";
}