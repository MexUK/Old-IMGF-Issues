#include <Windows.h>
#include "CFileUtility.h"
#include "../Path/CPathUtility.h"
#include "../String/CStringUtility.h"
#include "../Vector/CVectorUtility.h"
#include <fstream>

using namespace std;

string		CFileUtility::getFileContent(string& strPath, bool bBinaryMode)
{
	ifstream t(strPath.c_str(), bBinaryMode ? ios::in | ios::binary : ios::in);
	if (!t.is_open())
	{
		return "";
	}
	t.seekg(0, ios::end);
	streamoff size = t.tellg();
	string buffer((unsigned int)size, ' ');
	t.seekg(0);
	t.read(&buffer[0], size);
	t.close();
	return buffer;
}
string*			CFileUtility::getFileContentPointer(string& strPath, bool bBinaryMode)
{
	string *pData = new string;

	ifstream t(strPath.c_str(), bBinaryMode ? (ios::in | ios::binary) : ios::in);
	if (!t.is_open())
	{
		*pData = "";
		return pData;
	}
	t.seekg(0, ios::end);
	streamoff size = t.tellg();
	pData->resize((unsigned int)size, '\0');
	t.seekg(0, ios::beg);
	t.read(&(*pData)[0], size);
	t.close();
	return pData;
}
string			CFileUtility::getFileSubContent(string& strPath, uint32 uiSeek, uint32 uiByteCount, bool bBinaryMode)
{
	ifstream t(strPath.c_str(), bBinaryMode ? ios::in | ios::binary : ios::in);
	if (!t.is_open())
	{
		return "";
	}
	t.seekg(uiSeek, ios::beg);
	string buffer(uiByteCount, ' ');
	t.read(&buffer[0], uiByteCount);
	t.close();
	return buffer;
}

uint32	CFileUtility::getFileSize(string& strPath)
{
	FILE *pFile;
	fopen_s(&pFile, strPath.c_str(), "rb");
	if (pFile == NULL)
	{
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	uint32 iSeek = ftell(pFile);
	fclose(pFile);
	return iSeek;
}

void			CFileUtility::storeFile(string& strPath, string& strData, bool bAppend, bool bBinaryMode)
{
	createFoldersForPath(strPath);
	FILE *pFile;
	fopen_s(&pFile, strPath.c_str(), (string((bAppend ? "a" : "w")) + string((bBinaryMode ? "b" : ""))).c_str());
	fwrite(strData.c_str(), 1, strData.length(), pFile);
	fclose(pFile);
}

void			CFileUtility::storeFileByStringPointer(string& strPath, string* pData, bool bAppend, bool bBinaryMode)
{
	FILE *pFile;
	fopen_s(&pFile, strPath.c_str(), (string((bAppend ? "a" : "w")) + string((bBinaryMode ? "b" : ""))).c_str());
	fwrite(pData->c_str(), 1, pData->length(), pFile);
	fclose(pFile);
}

void			CFileUtility::renameFile(string& strPath, string& strNewPath)
{
	rename(strPath.c_str(), strNewPath.c_str());
}

void			CFileUtility::removeFile(string& strPath)
{
	//remove(strPath.c_str());
	//DeleteFile(strPath.c_str());
	DeleteFileW(CStringUtility::convertStdStringToStdWString(strPath).c_str());
}

string			CFileUtility::getFileNameFromNameWithoutExtension(string& strFolderPath, string& strFileNameWithoutExtension)
{
	CPathUtility::addSlashToEnd(strFolderPath);
	strFileNameWithoutExtension = CStringUtility::toUpperCase(strFileNameWithoutExtension);
	strFolderPath += "*";

	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(CStringUtility::convertStdStringToStdWString(strFolderPath).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return "";
	}

	do
	{
		if (CStringUtility::toUpperCase(CPathUtility::removeFileExtension(CStringUtility::convertStdWStringToStdString(ffd.cFileName))) == strFileNameWithoutExtension)
		{
			FindClose(hFind);
			return CStringUtility::convertStdWStringToStdString(ffd.cFileName);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return "";
}

string			CFileUtility::getNextIncrementingFileName(string& strFilePath)
{
	string strNextFilePath = strFilePath;
	uint32 uiSuffix = 1;
	while (doesFileExist(strNextFilePath))
	{
		strNextFilePath = CPathUtility::removeFileExtension(strFilePath) + CStringUtility::toString(uiSuffix) + "." + CPathUtility::getFileExtension(strFilePath);
		uiSuffix++;
	}
	return strNextFilePath;
}

bool			CFileUtility::doesFileExist(string& strPath)
{
	/*
	this commented code seems to fail sometimes, even when the "rb" is just "r"
	FILE *fp;
	fopen_s(&fp, strPath.c_str(), "rb");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	return false;
	*/
	ifstream f(strPath.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

vector<string>	CFileUtility::getFileNames(string& strFolderPath)
{
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);

	vector<string> vecFileNames;
	strFolderPath += "*";
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile(CStringUtility::convertStdStringToStdWString(strFolderPath).c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFileNames;
	}

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			// file
			vecFileNames.push_back(CStringUtility::convertStdWStringToStdString(ffd.cFileName));
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFileNames;
}

vector<string>	CFileUtility::getFileNamesByExtension(string& strFolderPath, string strFileExtension)
{
	vector<string> vecFileNames = getFileNames(strFolderPath);
	vector<string> vecFileNamesWithMatchingExtension;
	strFileExtension = CStringUtility::toUpperCase(strFileExtension);
	for (auto strFileName : vecFileNames)
	{
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFileName)) == strFileExtension)
		{
			vecFileNamesWithMatchingExtension.push_back(strFileName);
		}
	}
	return vecFileNamesWithMatchingExtension;
}

vector<string>	CFileUtility::getFilePaths(string& strFolderPath, bool bDeep, bool bFoldersOnly, string strExtensionFilter)
{
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
	strExtensionFilter = CStringUtility::toUpperCase(strExtensionFilter);

	vector<string> vecFilePaths;
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = FindFirstFile((CStringUtility::convertStdStringToStdWString(strFolderPath) + L"*").c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return vecFilePaths;
	}

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			// file
			if (!bFoldersOnly)
			{
				if (strExtensionFilter == "" || strExtensionFilter == CStringUtility::toUpperCase(CPathUtility::getFileExtension(CStringUtility::convertStdWStringToStdString(ffd.cFileName))))
				{
					vecFilePaths.push_back(strFolderPath + CStringUtility::convertStdWStringToStdString(ffd.cFileName));
				}
			}
		}
		else
		{
			// folder
			if (wstring(ffd.cFileName) != L"." && wstring(ffd.cFileName) != L"..")
			{
				if (bDeep)
				{
					vecFilePaths = CVectorUtility::combineVectors(vecFilePaths, getFilePaths(strFolderPath + CStringUtility::convertStdWStringToStdString(ffd.cFileName) + "/", true, bFoldersOnly, strExtensionFilter));
				}

				if (bFoldersOnly)
				{
					vecFilePaths.push_back(strFolderPath + CStringUtility::convertStdWStringToStdString(ffd.cFileName) + "/");
				}
			}
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
	return vecFilePaths;
}

void			CFileUtility::storeFileSubContent(string& strPath, string& strData, uint32 uiSeek)
{
	ofstream file;
	file.open(strPath.c_str(), ofstream::out | ofstream::binary | ofstream::in);
	if (!file.is_open())
	{
		// create the file so that the file can be opened with the 'in' flag
		file.open(strPath.c_str(), ofstream::out | ofstream::binary);
		file.close();

		file.open(strPath.c_str(), ofstream::out | ofstream::binary | ofstream::in);
	}
	if (!file.is_open())
	{
		return;
	}
	file.seekp(uiSeek);
	file.write(strData.c_str(), strData.length());
	file.close();
}

void						CFileUtility::createFoldersForPath(string& strPath)
{
	std::vector<std::string> vecFolders = CStringUtility::split(CStringUtility::replace(strPath, "\\", "/"), "/");
	vecFolders.erase(vecFolders.end() - 1);
	std::string strPath2 = "";
	for (auto strFolderName : vecFolders)
	{
		strPath2 += strFolderName + "/";

		if (!doesFolderExist(strPath2))
		{
			createFolder(strPath2);
		}
	}
}

bool						CFileUtility::doesFolderExist(string& strPath)
{
	uint32 uiAttributes = GetFileAttributes(CStringUtility::convertStdStringToStdWString(strPath).c_str());
	return uiAttributes != INVALID_FILE_ATTRIBUTES && (uiAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

void						CFileUtility::createFolder(string& strPath)
{
	CreateDirectory(CStringUtility::convertStdStringToStdWString(strPath).c_str(), NULL);
}

void						CFileUtility::copyFile(string& strSourcePath, string& strDestPath)
{
	const uint32 uiBufferSize = 8192;
	char szBuffer[uiBufferSize];
	size_t size;

	FILE
		*pFileSource = nullptr,
		*pFileDest = nullptr;
	fopen_s(&pFileSource, strSourcePath.c_str(), "rb");
	fopen_s(&pFileDest, strDestPath.c_str(), "wb");

	while (size = fread(szBuffer, 1, uiBufferSize, pFileSource)) {
		fwrite(szBuffer, 1, size, pFileDest);
	}

	fclose(pFileSource);
	fclose(pFileDest);
}

bool						CFileUtility::moveFile(string& strSourcePath, string& strDestPath)
{
	bool bFileExistsInDest = doesFileExist(strDestPath);
	string strMovedFilePath;
	
	// move existing file in destination to other location
	if (bFileExistsInDest)
	{
		strMovedFilePath = getNextIncrementingFileName(strDestPath);
		if (rename(strDestPath.c_str(), strMovedFilePath.c_str()) != 0)
		{
			// move failed, just attempt to move requested file
			return rename(strSourcePath.c_str(), strDestPath.c_str()) == 0;
		}
	}

	// move requested file
	if (rename(strSourcePath.c_str(), strDestPath.c_str()) != 0)
	{
		if (bFileExistsInDest)
		{
			// move failed, attempt to restore file from other location
			rename(strMovedFilePath.c_str(), strDestPath.c_str());
		}
		return false;
	}

	// remove file in other location
	if (bFileExistsInDest)
	{
		removeFile(strMovedFilePath);
	}

	return true;
}

bool						CFileUtility::isFolderEmpty(string& strFolderPath)
{
#ifdef WIN32
	bool bEntryFound = false;
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = FindFirstFile((CStringUtility::convertStdStringToStdWString(strFolderPath) + L"*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	do
	{
		if (wstring(FindFileData.cFileName) != L"." && wstring(FindFileData.cFileName) != L"..")
		{
			bEntryFound = true;
			break;
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	return !bEntryFound;
#else
	bool bEntryFound = false;
	DIR *pFolder = opendir(strFolderPath.c_str());
	do
	{
		dirent *pEntry = readdir(pFolder);
		if (dirent != nullptr)
		{
			if (string(dirent.d_name) != "." && string(dirent.d_name) != "..")
			{
				bEntryFound = true;
				break;
			}
		}
	} while (dirent != nullptr);
	closedir(pFolder);
	return !bEntryFound;
#endif
}

void						CFileUtility::emptyFolder(string& strFolderPath)
{
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = FindFirstFile((CStringUtility::convertStdStringToStdWString(strFolderPath) + L"*").c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (wstring(FindFileData.cFileName) != L"." && wstring(FindFileData.cFileName) != L"..")
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				emptyFolder(strFolderPath + CStringUtility::convertStdWStringToStdString(FindFileData.cFileName) + "/");
			}
			else
			{
				remove((strFolderPath + CStringUtility::convertStdWStringToStdString(FindFileData.cFileName)).c_str());
			}
		}
	}
	while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	RemoveDirectory(CStringUtility::convertStdStringToStdWString(strFolderPath).c_str());
}

void						CFileUtility::overwriteLockedFileViaContent(string& strLockedFilePath, string& strNewFileName, string& strNewFileData)
{
	string strLockedFileDirectory = CPathUtility::getDirectory(strLockedFilePath);
	rename(strLockedFilePath.c_str(), (strLockedFileDirectory + "Temp File").c_str());
	storeFile(strLockedFileDirectory + strNewFileName, strNewFileData, false, true);
}

string						CFileUtility::findFile(string& strFileName, string& strFolderPath)
{
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
	
	// check if file exists in root folder
	if (doesFileExist(strFolderPath + strFileName))
	{
		return strFolderPath + strFileName;
	}

	// search deep folders for file
	vector<string> vecFolderPaths = getFilePaths(strFolderPath, true, true);
	for (string strFolderPath2 : vecFolderPaths)
	{
		if (doesFileExist(strFolderPath2 + strFileName))
		{
			return strFolderPath2 + strFileName;
		}
	}

	// file not found
	return "";
}

string							CFileUtility::findImageFile(string& strFileNameWithoutExtension, string& strFolderPath)
{
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);

	string strFilePath;
	vector<string> vecImageExtensions = {
		"bmp",
		"png",
		"dds",
		"png",
		"gif",
		"jpg",
		"jpeg",
		"tga",
		"pgm",
		"ppm",
		"hdr"
	}; // must match list in CImageManager::loadImageFromFile

	// check if file exists in root folder
	for (string strImageExtension : vecImageExtensions)
	{
		strFilePath = strFolderPath + strFileNameWithoutExtension + "." + strImageExtension;
		if (doesFileExist(strFilePath))
		{
			return strFilePath;
		}
	}

	// search deep folders for file
	vector<string> vecFolderPaths = getFilePaths(strFolderPath, true, true);
	for (string strFolderPath2 : vecFolderPaths)
	{
		for (string strImageExtension : vecImageExtensions)
		{
			strFilePath = strFolderPath2 + strFileNameWithoutExtension + "." + strImageExtension;
			if (doesFileExist(strFilePath))
			{
				return strFilePath;
			}
		}
	}

	// file not found
	return "";
}

uint32					CFileUtility::getFileCreationDate(string& strFilePath)
{
	FILETIME fileTimeCreation, fileTimeLastAccess, fileTimeLastModified;
	FILE *pFile = nullptr;
	fopen_s(&pFile, strFilePath.c_str(), "rb");
	if (!pFile)
	{
		return 0;
	}

	int iResult = GetFileTime(pFile, &fileTimeCreation, &fileTimeLastAccess, &fileTimeLastModified);
	fclose(pFile);
	if (iResult == 0)
	{
		return 0;
	}

	GetSystemTimeAsFileTime(&fileTimeCreation);
	__int64* val = (__int64*)&fileTimeCreation;
	uint32 uiFileCreationDate = (uint32)(static_cast<uint32>(*val) / 10000000.0 - 11644473600.0);   // epoch is Jan. 1, 1601: 134774 days to Jan. 1, 1970
	return uiFileCreationDate;
}