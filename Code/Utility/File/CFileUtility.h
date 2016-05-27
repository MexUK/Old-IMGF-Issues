#ifndef CFileUtility_H
#define CFileUtility_H

#include "Types.h"
#include <string>
#include <vector>

class CFileUtility
{
public:
	static std::string					getFileContent(std::string& strPath, bool bBinaryMode = true);
	static std::string*					getFileContentPointer(std::string& strPath, bool bBinaryMode = true);
	static std::string					getFileSubContent(std::string& strPath, uint32 uiSeek, uint32 uiByteCount, bool bBinaryMode = true);
	static uint32						getFileSize(std::string& strPath);
	static void							storeFile(std::string& strPath, std::string& strData, bool bAppend = true, bool bBinaryMode = true);
	static void							storeFileByStringPointer(std::string& strPath, std::string* pData, bool bAppend = true, bool bBinaryMode = true);
	static void							renameFile(std::string& strPath, std::string& strNewPath);
	static void							removeFile(std::string& strPath);
	static std::string					getFileNameFromNameWithoutExtension(std::string& strFolderPath, std::string& strFileNameWithoutExtension);
	static std::string					getNextIncrementingFileName(std::string& strFilePath);
	static bool							doesFileExist(std::string& strPath);
	static std::vector<std::string>		getFileNames(std::string& strFolderPath);
	static std::vector<std::string>		getFileNamesByExtension(std::string& strFolderPath, std::string strFileExtension);
	static std::vector<std::string>		getFilePaths(std::string& strFolderPath, bool bDeep = false, bool bFoldersOnly = false, std::string strExtensionFilter = "");
	static void							storeFileSubContent(std::string& strPath, std::string& strData, uint32 uiSeek);
	static void							createFoldersForPath(std::string& strPath);
	static bool							doesFolderExist(std::string& strPath);
	static void							createFolder(std::string& strPath);
	static void							copyFile(std::string& strSourcePath, std::string& strDestPath);
	static bool							moveFile(std::string& strSourcePath, std::string& strDestPath);
	static bool							isFolderEmpty(std::string& strFolderPath);
	static void							emptyFolder(std::string& strFolderPath);
	static void							overwriteLockedFileViaContent(std::string& strLockedFilePath, std::string& strNewFileName, std::string& strNewFileData);
	static std::string					findFile(std::string& strFileName, std::string& strFolderPath); // returns full path to file or blank string if not found
	static std::string					findImageFile(std::string& strFileNameWithoutExtension, std::string& strFolderPath); // returns full path to file or blank string if not found
	static uint32						getFileCreationDate(std::string& strFilePath);
};

#endif