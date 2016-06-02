#pragma warning(disable : 4005)

#include "CLSTProcessingManager.h"
#include "CKGM.h"
#include "Globals.h"
#include "LST/CLSTFormat.h"
#include "LST/CLSTSection.h"
#include "LST/CLSTEntry.h"
#include "String/CStringUtility.h"
#include "File/CFileUtility.h"
#include "Path/CPathUtility.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "GUI/Editors/CIMGEditor.h"
#include "GUI/Editors/Tab/CIMGEditorTab.h"
#include "COL/CCOLManager.h"
#include "COL/CCOLFormat.h"
#include "COL/CCOLEntry.h"
#include "GUI/CGUIUtility.h"
#include "Sort/CSortManager.h"
#include "IMG/CIMGEntry.h"
#include "Localization/CLocalizationManager.h"

using namespace std;

void		CLSTProcessingManager::init(void)
{
}
void		CLSTProcessingManager::uninit(void)
{
}

void		CLSTProcessingManager::process(CLSTFormat *pLSTFile)
{
	string strGTARootFolderPath;
	if (pLSTFile->doesEntryExistByName("gtaroot"))
	{
		if (pLSTFile->getEntryByName("gtaroot")->getEntryCount() > 0)
		{
			strGTARootFolderPath = pLSTFile->getEntryByName("gtaroot")->getEntryByIndex(0)->getLine();
		}
	}
	if (strGTARootFolderPath == "")
	{
		strGTARootFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_3"), getKGM()->getLastUsedDirectory("LST_GAME"));
		if (strGTARootFolderPath == "")
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_60"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_60"), MB_OK);
			return;
		}
		strGTARootFolderPath = CPathUtility::addSlashToEnd(strGTARootFolderPath);
		getKGM()->setLastUsedDirectory("LST_GAME", strGTARootFolderPath);
	}
	strGTARootFolderPath = CPathUtility::addSlashToEnd(strGTARootFolderPath);

	//string strWorkingPath = strGTARootFolderPath;
	//string strWorkingPath = CPathUtility::addSlashToEnd(pLSTFile->getSectionByName("Start")->getEntryByName("updatearchive")->getValue(0));

	if (pLSTFile->doesEntryExistByName("cdimages"))
	{
		if (!pLSTFile->getEntryByName("cdimages")->doesEntryExistByName("gamepath"))
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "gamepath", "cdimages"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
			return;
		}

		string strIMGPath = strGTARootFolderPath + pLSTFile->getEntryByName("cdimages")->getEntryByName("gamepath")->getValuesLine();
		if (!CFileUtility::doesFileExist(strIMGPath))
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_27", strIMGPath.c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
			return;
		}
		getKGM()->getTaskManager()->getDispatch()->onRequestOpen2(strIMGPath);

		string strSourceFolderPath = "";
		if (!pLSTFile->getEntryByName("cdimages")->doesEntryExistByName("source"))
		{
			strSourceFolderPath = CPathUtility::removeSlashFromFront(CPathUtility::addSlashToEnd(pLSTFile->getEntryByName("cdimages")->getEntryByName("source")->getValuesLine()));
		}

		for (auto pLSTFileEntry : pLSTFile->getEntryByName("cdimages")->getEntries())
		{
			string strCommandUpper = CStringUtility::toUpperCase(pLSTFileEntry->getName());
			if (strCommandUpper == "ADD")
			{
				if (strSourceFolderPath == "")
				{
					CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "source", "cdimages"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
					return;
				}

				string strEntryFileName = CPathUtility::getFileName(pLSTFileEntry->getValuesLine());
				string strEntryPath = strGTARootFolderPath + strSourceFolderPath + strEntryFileName;
				if (CFileUtility::doesFileExist(strEntryPath))
				{
					getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strEntryPath);
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_30", strEntryFileName.c_str()));
				}
				else
				{
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_31", strEntryFileName.c_str()));
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_32", strEntryPath.c_str()), true);
				}
			}
			else if (strCommandUpper == "DEL")
			{
				string strEntryFileName = CPathUtility::getFileName(pLSTFileEntry->getValuesLine());
				CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getEntryByName(strEntryFileName);
				if (pIMGEntry == nullptr)
				{
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_33", strEntryFileName.c_str()));
				}
				else
				{
					getKGM()->getEntryListTab()->removeEntry(pIMGEntry);
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_34", strEntryFileName.c_str()));
				}
			}
			else if (strCommandUpper == "RESORT")
			{
				getKGM()->getSortManager()->sort(getKGM()->getEntryListTab()->getIMGFile());
				getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_35"));
			}
			else if (strCommandUpper == "REBUILD")
			{
				getKGM()->getEntryListTab()->rebuild();
				getKGM()->getIMGEditor()->refreshActiveTab();
				getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_36"));
			}
		}

	}

	if (pLSTFile->doesEntryExistByName("collisions"))
	{
		if (!pLSTFile->getEntryByName("collisions")->doesEntryExistByName("gamepath"))
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "gamepath", "collisions"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
			return;
		}

		string strCOLPath = strGTARootFolderPath + pLSTFile->getEntryByName("collisions")->getEntryByName("gamepath")->getValuesLine();
		if (!CFileUtility::doesFileExist(strCOLPath))
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_29", strCOLPath.c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
			return;
		}
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strCOLPath);
		if(!pCOLFile->doesHaveError())
		{
			string strSourceFolderPath = "";
			if (!pLSTFile->getEntryByName("collisions")->doesEntryExistByName("source"))
			{
				strSourceFolderPath = CPathUtility::removeSlashFromFront(CPathUtility::addSlashToEnd(pLSTFile->getEntryByName("collisions")->getEntryByName("source")->getValuesLine()));
			}

			for (auto pLSTFileEntry : pLSTFile->getEntryByName("collisions")->getEntries())
			{
				string strCommandUpper = CStringUtility::toUpperCase(pLSTFileEntry->getName());
				if (strCommandUpper == "RENAME")
				{
					string strCOLOldFileName = CPathUtility::removeFileExtension(CPathUtility::getFileName(pLSTFileEntry->getValue(0)));
					string strCOLNewFileName = CPathUtility::removeFileExtension(CPathUtility::getFileName(pLSTFileEntry->getValue(1)));
					CCOLEntry *pCOLEntry = pCOLFile->getEntryByName(strCOLOldFileName);
					if (pCOLEntry == nullptr)
					{
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_37", strCOLOldFileName.c_str()));
					}
					else
					{
						pCOLEntry->setModelName(strCOLNewFileName);
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_38", strCOLOldFileName.c_str(), strCOLNewFileName.c_str()));
					}
				}
				else if (strCommandUpper == "ADD")
				{
					if (strSourceFolderPath == "")
					{
						CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "source", "collisions"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
						return;
					}

					string strEntryFileName = CPathUtility::getFileName(pLSTFileEntry->getValuesLine());
					string strEntryPath = strGTARootFolderPath + strSourceFolderPath + strEntryFileName;

					if (CFileUtility::doesFileExist(strEntryPath))
					{
						getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strEntryPath);
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_39", strEntryFileName.c_str()));
					}
					else
					{
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_40", strEntryFileName.c_str()));
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_41", strEntryPath.c_str()), true);
					}
				}
				else if (strCommandUpper == "DEL")
				{
					string strEntryFileName = CPathUtility::getFileName(pLSTFileEntry->getValuesLine());
					CCOLEntry *pCOLEntry = pCOLFile->getEntryByName(strEntryFileName);
					if (pCOLEntry == nullptr)
					{
						getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_42", strEntryFileName.c_str()));
					}
					else
					{
						string strEntryActualFileName = pCOLEntry->getModelName();
						pCOLFile->removeEntry(pCOLEntry);
						getKGM()->getEntryListTab()->log("Removed COL entry: " + strEntryActualFileName);
					}
				}
				else if (strCommandUpper == "RESORT")
				{
					pCOLFile->sort();
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_43"));
				}
			}
			pCOLFile->serializeViaFile();
			pCOLFile->unload();
		}
		pCOLFile->unload();
		delete pCOLFile;
	}

	string
		strSourceFolderPath = "",
		strGamePathFolderPath = "";
	if (pLSTFile->doesEntryExistByName("other"))
	{
		for (auto pLSTFileEntry : pLSTFile->getEntryByName("other")->getEntries())
		{
			string strCommandUpper = CStringUtility::toUpperCase(pLSTFileEntry->getName());
			if (strCommandUpper == "SOURCE")
			{
				strSourceFolderPath = CPathUtility::removeSlashFromFront(CPathUtility::addSlashToEnd(pLSTFileEntry->getValuesLine()));
			}
			else if (strCommandUpper == "GAMEPATH")
			{
				strGamePathFolderPath = CPathUtility::removeSlashFromFront(CPathUtility::addSlashToEnd(pLSTFileEntry->getValuesLine()));
			}
			else if (strCommandUpper == "COPY")
			{
				if (strSourceFolderPath == "")
				{
					CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "source", "other"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
					return;
				}

				if (strGamePathFolderPath == "")
				{
					CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "gamepath", "other"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
					return;
				}

				string strFileName = CPathUtility::getFileName(CPathUtility::removeSlashFromEnd(pLSTFileEntry->getValuesLine()));
				if (strFileName == "*")
				{
					string strCopySourceFolder = strGTARootFolderPath + strSourceFolderPath + CPathUtility::removeSlashFromFront(CPathUtility::getDirectory(pLSTFileEntry->getValuesLine()));
					for (auto strFileName : CFileUtility::getFileNames(strCopySourceFolder))
					{
						string strCopySourcePath = strGTARootFolderPath + strSourceFolderPath + strFileName;
						string strCopyDestinationPath = strGTARootFolderPath + strGamePathFolderPath + strFileName;
						CFileUtility::copyFile(strCopySourcePath, strCopyDestinationPath);
					}
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_LSTCopy1"));
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTCopy2", strCopySourceFolder.c_str(), (strGTARootFolderPath + strGamePathFolderPath).c_str()), true);
				}
				else
				{
					string strFileName = CPathUtility::removeSlashFromFront(pLSTFileEntry->getValuesLine());
					string strCopySourcePath = strGTARootFolderPath + strSourceFolderPath + strFileName;
					string strCopyDestinationPath = strGTARootFolderPath + strGamePathFolderPath + strFileName;
					CFileUtility::copyFile(strCopySourcePath, strCopyDestinationPath);
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTCopy3", strFileName.c_str()));
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTCopy4", strFileName.c_str(), strCopySourcePath.c_str(), strCopyDestinationPath.c_str()), true);
				}
			}
			else if (strCommandUpper == "DEL" || strCommandUpper == "DELETE")
			{
				if (strSourceFolderPath == "")
				{
					CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "source", "other"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
					return;
				}

				if (strGamePathFolderPath == "")
				{
					CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("LSTEntryMissing", "gamepath", "other"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_LSTFileError"), MB_OK);
					return;
				}

				string strFileName = CPathUtility::getFileName(CPathUtility::removeSlashFromEnd(pLSTFileEntry->getValuesLine()));
				if (strFileName == "*")
				{
					string strFolderPath = strGTARootFolderPath + strGamePathFolderPath + CPathUtility::removeSlashFromFront(CPathUtility::getDirectory(pLSTFileEntry->getValuesLine()));
					for (auto strFileName : CFileUtility::getFileNames(strFolderPath))
					{
						string strFilePath = strFolderPath + strFileName;
						CFileUtility::removeFile(strFilePath);
					}
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_LSTDelete1"));
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTDelete2", strFolderPath.c_str()), true);
				}
				else
				{
					string strFileName = CPathUtility::removeSlashFromFront(pLSTFileEntry->getValuesLine());
					string strFilePath = strGTARootFolderPath + strGamePathFolderPath + strFileName;
					CFileUtility::removeFile(strFilePath);

					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTDelete3", strFileName.c_str()));
					getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_LSTDelete4", strFilePath.c_str()), true);
				}
			}
		}
	}
	
	pLSTFile->unload();
	delete pLSTFile;
}