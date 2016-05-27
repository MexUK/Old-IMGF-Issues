#pragma warning(disable : 4005)

#include "CEntryListWindowTab.h"
#include "CKGM.h"
#include "Globals.h"
#include "String/CStringUtility.h"
#include "Vector/CVectorUtility.h"
#include "Path/CPathUtility.h"
#include "File/CFileUtility.h"
#include "IMG/CIMGManager.h"
#include "IMG/CIMGFormat.h"
#include "IMG/CIMGEntry.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersion.h"
#include "Engine/RAGE/CRageManager.h"
#include "Sort/CSortManager.h"
#include "Sort/CSortPriority.h"
#include "Sort/CSortType.h"
#include "Sort/eSortType.h"
#include "Testing/CDebugger.h"
#include "DB/CDBFormat.h"
#include "COL/CCOLManager.h"
#include "CSearchEntry.h"
#include "Engine/RW/CRWVersionManager.h"
#include "COL/CCOLVersionManager.h"
#include "Engine/RAGE/CRageResourceTypeManager.h"
#include "Localization/CLocalizationManager.h"
#include "Sort/CSortPriorities.h"
#include "Platform/CPlatformManager.h"
#include "CSettingsManager.h"
#include "CGUIManager.h"
#include "GUI/CGUIUtility.h"
#include "DB/CDBManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Window/Main Window/CMainWindow.h"
#include "Window/Window Types/CEntryListWindow.h"
#include "Window/Window Controls/Controls/CWindowControl_List.h"
#include "Task/CTaskManager.h"
#include <algorithm>

using namespace std;

void					CEntryListWindowTab::unload(void)
{
	delete m_pDBFile;

	m_pIMGFile->unload();
	delete m_pIMGFile;
}

bool					CEntryListWindowTab::onTabFormatReady(void)
{
	if (!checkForErrors())
	{
		return false;
	}

	initTab();
	return true;
}

bool					CEntryListWindowTab::checkForErrors(void)
{
	CIMGFormat *pIMGFormat = getIMGFile();

	if (pIMGFormat->getIMGVersion() == IMG_FASTMAN92)
	{
		// check if IMG is fastman92 format and is encrypted
		if (pIMGFormat->isEncrypted())
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_21"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_21"), MB_OK);
			return false;
		}

		// check if IMG is fastman92 format and has an unsupported game type
		if (pIMGFormat->getGameType() != 0)
		{
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_68", pIMGFormat->getGameType()), CLocalizationManager::getInstance()->getTranslatedText("UnableToOpenIMG"), MB_OK);
			return false;
		}
	}

	// check for unserialize error [includes file open/close errors]
	if (pIMGFormat->doesHaveError())
	{
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_23"), CLocalizationManager::getInstance()->getTranslatedText("UnableToOpenIMG"), MB_OK);
		return false;
	}

	// no errors occurred
	return true;
}

void					CEntryListWindowTab::initTab(void)
{
	// add visual tab
	// todo
	//((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->InsertItem(getIndex(), CStringUtility::convertStdStringToStdWString(CPathUtility::getFileName(getIMGFile()->getFilePath())).c_str());
	//((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->SetCurSel(getIndex());

	// store tab data
	// todo setListViewHwnd(GetDlgItem(getKGM()->getDialog()->GetSafeHwnd(), 37));

	// add to recently open
	getKGM()->getRecentlyOpenManager()->addRecentlyOpenEntry(getIMGFile()->getFilePath());
	getKGM()->getRecentlyOpenManager()->loadRecentlyOpenEntries();

	// update filename for open last
	getKGM()->getActiveWindow()->setOpenLastFilename(CPathUtility::getFileName(getIMGFile()->getFilePath()));

	// load corresponding DB file & protected entry states
	m_pDBFile = nullptr;
	string strDBFilePath = CPathUtility::replaceFileExtension(getIMGFile()->getFilePath(), "db");
	if (CFileUtility::doesFileExist(strDBFilePath))
	{
		m_pDBFile = CDBManager::getInstance()->parseViaFile(strDBFilePath);

		if (m_pDBFile->doesHaveError())
		{
			m_pDBFile = nullptr;
		}
	}

	if (m_pDBFile == nullptr)
	{
		// either the db file doesn't exist or the db file is corrupt
		//pWindowTab->m_pDBFile = CDBManager::getInstance()->createDBFileFromIMGFile(pWindowTab->getIMGFile());
		m_pDBFile = CDBManager::getInstance()->createBlankDBFile();
	}
	loadProtectedEntryStates();

	// load filters - todo where is the other filter loading from?
	//loadFilter_Type();

	// check for unknown RW versions
	checkForUnknownRWVersionEntries();
}

void					CEntryListWindowTab::checkForUnknownRWVersionEntries(void)
{
	if (getIMGFile()->getIMGVersion() == IMG_3)
	{
		return;
	}

	vector<CIMGEntry*> vecUnknownRWVersionEntries = getIMGFile()->getUnknownVersionEntries();
	if (vecUnknownRWVersionEntries.size() > 0)
	{
		vector<string> vecIMGEntryNames;
		for (auto pIMGEntry : vecUnknownRWVersionEntries)
		{
			vecIMGEntryNames.push_back(pIMGEntry->getEntryName());
		}

		getKGM()->getTaskManager()->onTaskPause();
		getKGM()->getGUIManager()->showListViewDialog(
			CLocalizationManager::getInstance()->getTranslatedText("UnknownVersions"),
			CLocalizationManager::getInstance()->getTranslatedFormattedText("UnknownVersionsCheck", CPathUtility::getFileName(getIMGFile()->getFilePath()).c_str(), vecUnknownRWVersionEntries.size()),
			CLocalizationManager::getInstance()->getTranslatedText("Window_OrphanEntries_EntryName"),
			vecIMGEntryNames,
			CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_3_InitialFilename",
			CPathUtility::replaceFileExtension(CPathUtility::getFileName(getIMGFile()->getFilePath()), "TXT").c_str()),
			"UNKNOWNRWVERSIONS"
			);
		getKGM()->getTaskManager()->onTaskUnpause();
	}
}

void					CEntryListWindowTab::log(string strText, bool bExtendedModeOnly)
{
	//string strLogEntryWithTimestamp = "[" + CStringUtility::getTimestampText() + "] " + strText;
	string strLogEntryWithTimestampAndIMG = "[" + CStringUtility::getTimestampText() + "] [" + CPathUtility::getFileName(m_pIMGFile->getFilePath()) + "] " + strText;

	if (bExtendedModeOnly)
	{
		m_vecLogLinesExtended.push_back(strLogEntryWithTimestampAndIMG);

		// automatic logging to file
		if (getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath") != "")
		{
			// extended file
			if (getKGM()->getSettingsManager()->getSettingBool("AutomaticLoggingExtended"))
			{
				string strExtendedLogPath = CPathUtility::addSlashToEnd(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath"));
				strExtendedLogPath += CStringUtility::getDateTextForFolder() + "/" + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Extended");
				CFileUtility::storeFile(strExtendedLogPath, strLogEntryWithTimestampAndIMG + "\n", true, false);
			}
		}
	}
	else
	{
		m_vecLogLinesGUI.push_back(strText);
		m_vecLogLinesBasic.push_back(strLogEntryWithTimestampAndIMG);
		m_vecLogLinesExtended.push_back(strLogEntryWithTimestampAndIMG);

		// automatic logging to file
		if (getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath") != "")
		{
			// basic file
			if (getKGM()->getSettingsManager()->getSettingBool("AutomaticLoggingBasic"))
			{
				string strExtendedLogPath = CPathUtility::addSlashToEnd(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath"));
				strExtendedLogPath += CStringUtility::getDateTextForFolder() + "/" + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Basic");
				CFileUtility::storeFile(strExtendedLogPath, strLogEntryWithTimestampAndIMG + "\n", true, false);
			}
			
			// extended file
			if (getKGM()->getSettingsManager()->getSettingBool("AutomaticLoggingExtended"))
			{
				string strExtendedLogPath = CPathUtility::addSlashToEnd(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath"));
				strExtendedLogPath += CStringUtility::getDateTextForFolder() + "/" + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Extended");
				CFileUtility::storeFile(strExtendedLogPath, strLogEntryWithTimestampAndIMG + "\n", true, false);
			}
		}
	}

	if (getKGM()->getActiveTab() == this)
	{
		/*
		todo
		CEdit *pEdit = ((CEdit*)getKGM()->getDialog()->GetDlgItem(14));
		pEdit->SetWindowTextW(CStringUtility::convertStdStringToStdWString(CStringUtility::join(m_vecLogLinesGUI, "\r\n")).c_str());
		pEdit->LineScroll(pEdit->GetLineCount());
		*/
	}
}

void					CEntryListWindowTab::clearLogs(void)
{
	getLogLinesGUI().clear();
	getLogLinesBasic().clear();
	getLogLinesExtended().clear();

	/*
	todo
	CEdit *pEdit = ((CEdit*)getKGM()->getDialog()->GetDlgItem(14));
	pEdit->SetWindowTextW(L"");
	pEdit->LineScroll(0);
	*/
}

void					CEntryListWindowTab::checkToApplyCompression(CIMGEntry *pIMGEntry)
{
	if (getKGM()->getSettingsManager()->getSettingBool("AutoCompressionImportReplace"))
	{
		if (getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
		{
			//eCompressionAlgorithm eCompressionAlgorithmValue = (eCompressionAlgorithm)getKGM()->getSettingsManager()->getSettingInt("Fastman92IMGAutoCompressionType");
			if (getIMGFile()->getEntryCount() > 1) // > 1 instead of > 0, because the entry has already been added to the pool.
			{
				eCompressionAlgorithm eCompressionAlgorithmValue;

				// check to use compression type from first entry in IMG
				eCompressionAlgorithmValue = getIMGFile()->getEntryByIndex(0)->getCompressionAlgorithmId();
				
				// if compression type is not compressed (none) or unknown, default to ZLIB compression
				if (eCompressionAlgorithmValue == IMGCOMPRESSION_NONE || eCompressionAlgorithmValue == IMGCOMPRESSION_UNKNOWN)
				{
					eCompressionAlgorithmValue = IMGCOMPRESSION_ZLIB;
				}

				// apply the compression to the IMG entry
				pIMGEntry->applyCompression(eCompressionAlgorithmValue);
			}
		}
	}
}
void					CEntryListWindowTab::addEntryViaFile(string strEntryFilePath, string strEntryName)
{
	CIMGEntry *pIMGEntry = getIMGFile()->addEntryViaFile(strEntryFilePath, strEntryName);
	checkToApplyCompression(pIMGEntry);
	addEntryToMainListView(pIMGEntry);
	updateEntryCountText();
	updateIMGText();
}
void					CEntryListWindowTab::addEntryViaData(string strEntryName, string strEntryData)
{
	CIMGEntry *pIMGEntry = getIMGFile()->addEntryViaData(strEntryName, strEntryData);
	checkToApplyCompression(pIMGEntry);
	addEntryToMainListView(pIMGEntry);
	updateEntryCountText();
	updateIMGText();
}
void					CEntryListWindowTab::replaceEntryViaFile(string strEntryName, string strEntryFilePath, string strNewEntryName)
{
	CIMGEntry *pIMGEntry = getIMGFile()->replaceEntryViaFile(strEntryName, strEntryFilePath, strNewEntryName);
	checkToApplyCompression(pIMGEntry);
	updateEntryInMainListView(pIMGEntry);
	updateIMGText();
}
void					CEntryListWindowTab::replaceEntryViaData(string strEntryName, string& strEntryData, string strNewEntryName)
{
	CIMGEntry *pIMGEntry = getIMGFile()->replaceEntryViaData(strEntryName, strEntryData, strNewEntryName);
	checkToApplyCompression(pIMGEntry);
	updateEntryInMainListView(pIMGEntry);
	updateIMGText();
}
void					CEntryListWindowTab::addOrReplaceEntryViaFile(string strEntryFilePath, string strEntryName)
{
	uint32 uiIMGEntryCount = getIMGFile()->getEntryCount();
	CIMGEntry *pIMGEntry = getIMGFile()->addOrReplaceEntryViaFile(strEntryFilePath, strEntryName);
	checkToApplyCompression(pIMGEntry);
	if (uiIMGEntryCount == getIMGFile()->getEntryCount())
	{
		// entry was replaced
		updateEntryInMainListView(pIMGEntry);
	}
	else
	{
		// entry was added
		addEntryToMainListView(pIMGEntry);
		updateEntryCountText();
	}
	updateIMGText();
}
void					CEntryListWindowTab::addOrReplaceEntryViaData(string strEntryName, string strEntryData)
{
	uint32 uiIMGEntryCount = getIMGFile()->getEntryCount();
	CIMGEntry *pIMGEntry = getIMGFile()->addOrReplaceEntryViaData(strEntryName, strEntryData);
	checkToApplyCompression(pIMGEntry);
	if (uiIMGEntryCount == getIMGFile()->getEntryCount())
	{
		// entry was replaced
		updateEntryInMainListView(pIMGEntry);
	}
	else
	{
		// entry was added
		addEntryToMainListView(pIMGEntry);
		updateEntryCountText();
	}
	updateIMGText();
}
void					CEntryListWindowTab::addOrReplaceEntryViaFileAndSettings(string strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = CPathUtility::getFileName(strEntryFilePath);
	}

	CIMGEntry *pIMGEntry = getIMGFile()->getEntryByName(strEntryName);
	if (pIMGEntry == nullptr)
	{
		// entry name not found in IMG
		// import the entry
		return addEntryViaFile(strEntryFilePath, strEntryName);
	}

	// entry name is found in IMG
	if (getKGM()->getSettingsManager()->getSettingBool("AskBeforeOverwritingFiles"))
	{
		// Setting is enabled: Ask before overwriting files

		// show popup - ask to replace or import
		uint32 uiResultOption;
		if (isOverwriteEntryOptionSet())
		{
			uiResultOption = getOverwriteEntryOption();
		}
		else
		{
			COverwriteEntryDialogData *pOverwriteEntryDialogData = getKGM()->getGUIManager()->showOverwriteEntryDialog();
			if (pOverwriteEntryDialogData->m_bCancel)
			{
				// cancel
				delete pOverwriteEntryDialogData;
				return;
			}

			setOverwriteEntryOption(pOverwriteEntryDialogData->m_uiResultOption);
			uiResultOption = pOverwriteEntryDialogData->m_uiResultOption;

			delete pOverwriteEntryDialogData;
		}

		if (uiResultOption == 0) // import
		{
			return addEntryViaFile(strEntryFilePath, strEntryName);
		}
		else if (uiResultOption == 1) // replace
		{
			return replaceEntryViaFile(strEntryName, strEntryFilePath);
		}
	}

	if (pIMGEntry->isProtectedEntry())
	{
		// entry is protected

		if (getKGM()->getSettingsManager()->getSettingBool("OverwriteProtectedFiles"))
		{
			// Setting is enabled: Overwrite protected files
			return replaceEntryViaFile(strEntryName, strEntryFilePath);
		}
		else
		{
			// Setting is disabled: Overwrite protected files
			// do nothing, don't replace the file
			return;
		}
	}

	uint32
		uiExistingEntryFileCreationDate = pIMGEntry->getFileCreationDate(),
		uiNewEntryFileCreationDate = CFileUtility::getFileCreationDate(strEntryFilePath);

	if (uiExistingEntryFileCreationDate == 0 || uiNewEntryFileCreationDate == 0)
	{
		// either the file creation date is not stored for the existing entry or unable to fetch it for the new entry
		return replaceEntryViaFile(strEntryName, strEntryFilePath);
	}

	if (uiNewEntryFileCreationDate == uiExistingEntryFileCreationDate)
	{
		// file creation date is the same for both existing entry and new entry
		return replaceEntryViaFile(strEntryName, strEntryFilePath);
	}

	if (uiExistingEntryFileCreationDate < uiNewEntryFileCreationDate)
	{
		// the existing entry is older than the new entry

		if (getKGM()->getSettingsManager()->getSettingBool("OverwriteOlderFiles"))
		{
			// Setting is enabled: Overwrite older files
			return replaceEntryViaFile(strEntryName, strEntryFilePath);
		}
		else
		{
			// Setting is disabled: Overwrite older files
			// do nothing, don't replace the file
			return;
		}
	}
	else if (uiExistingEntryFileCreationDate > uiNewEntryFileCreationDate)
	{
		// the existing entry is newer than the new entry

		if (getKGM()->getSettingsManager()->getSettingBool("OverwriteNewerFiles"))
		{
			// Setting is enabled: Overwrite newer files
			return replaceEntryViaFile(strEntryName, strEntryFilePath);
		}
		else
		{
			// Setting is disabled: Overwrite newer files
			// do nothing, don't replace the file
			return;
		}
	}

	// replace by default. e.g. if no settings are enabled.
	return replaceEntryViaFile(strEntryName, strEntryFilePath);
}
void					CEntryListWindowTab::addOrReplaceEntryViaDataAndSettings(string strEntryName, string strEntryData)
{
	CIMGEntry *pIMGEntry = getIMGFile()->getEntryByName(strEntryName);
	if (pIMGEntry == nullptr)
	{
		// entry name not found in IMG
		// import the entry
		return addEntryViaData(strEntryName, strEntryData);
	}

	// entry name is found in IMG
	if (getKGM()->getSettingsManager()->getSettingBool("AskBeforeOverwritingFiles"))
	{
		// Setting is enabled: Ask before overwriting files

		// show popup - ask to replace or import
		COverwriteEntryDialogData *pOverwriteEntryDialogData = getKGM()->getGUIManager()->showOverwriteEntryDialog();
		if (pOverwriteEntryDialogData->m_bCancel)
		{
			// cancel
			delete pOverwriteEntryDialogData;
			return;
		}
		if (pOverwriteEntryDialogData->m_uiResultOption == 0) // import
		{
			delete pOverwriteEntryDialogData;
			return addEntryViaData(strEntryName, strEntryData);
		}
		else if (pOverwriteEntryDialogData->m_uiResultOption == 1) // replace
		{
			delete pOverwriteEntryDialogData;
			return replaceEntryViaData(strEntryName, strEntryData);
		}

		delete pOverwriteEntryDialogData;
	}

	if (pIMGEntry->isProtectedEntry())
	{
		// entry is protected

		if (getKGM()->getSettingsManager()->getSettingBool("OverwriteProtectedFiles"))
		{
			// Setting is enabled: Overwrite protected files
			return replaceEntryViaData(strEntryName, strEntryData);
		}
		else
		{
			// Setting is disabled: Overwrite protected files
			// do nothing, don't replace the file
			return;
		}
	}
	
	return replaceEntryViaData(strEntryName, strEntryData);
}
void					CEntryListWindowTab::removeEntry(CIMGEntry *pIMGEntry)
{
	getIMGFile()->removeEntry(pIMGEntry);
	updateEntryCountText();
	updateIMGText();
}

void					CEntryListWindowTab::addColumnsToMainListView(void)
{
	getKGM()->getEntryListWindow()->addColumnsToMainListView(getIMGFile()->getIMGVersion());
}
void					CEntryListWindowTab::readdAllEntriesToMainListView(void)
{
	getWindow()->getEntryListControl()->removeAllEntries();
	
	getKGM()->getEntryListWindow()->setSelectedEntryCount(0);
	getKGM()->getEntryListWindow()->updateSelectedEntryCountText();

	addAllEntriesToMainListView();

	getWindow()->getEntryListControl()->getWindow()->render();
}
void					CEntryListWindowTab::addAllEntriesToMainListView(void)
{
	//getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getIMGFile()->m_vecEntries.size());
	// setProgressMaxTicks() is called in CKGM::addMainWindowTab(). (as the bottom of this code contains a call to onProgressTick()).

	/*
	todo

	int iCurSel;
	::CString strText1;

	iCurSel = ((CComboBox*)getKGM()->getDialog()->GetDlgItem(54))->GetCurSel();
	bool bFilterCheckBox_Extensions = iCurSel != 0;
	uint32 uiFilterCombo_Extensions = iCurSel;
	
	CComboBox *pComboBox1 = ((CComboBox*)getKGM()->getDialog()->GetDlgItem(5));
	iCurSel = pComboBox1->GetCurSel();
	bool bFilterCheckBox_Version = iCurSel != 0;
	uint32 uiFilterCombo_Version = iCurSel;
	*/

	for (auto pIMGEntry : getIMGFile()->getEntries())
	{
		bool bAddEntry = true;

		// RW version
		/*
		todo

		string strEntryExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
		if (bFilterCheckBox_Version)
		{
			if (uiFilterCombo_Version == getKGM()->m_iFilterMapping_UnknownVersion) // Unknown Version
			{
				if (strEntryExtensionUpper == "COL")
				{
					eCOLVersion eEntryVersion = pIMGEntry->getCOLVersion() == nullptr ? COL_UNKNOWN : pIMGEntry->getCOLVersion()->getVersionId();
					if (eEntryVersion == COL_UNKNOWN)
					{
						// add the entry
					}
					else
					{
						// don't add the entry
						bAddEntry = false;
					}
				}
				else if (strEntryExtensionUpper == "DFF" || strEntryExtensionUpper == "TXD")
				{
					if (pIMGEntry->getRWVersion() == nullptr)
					{
						// add the entry
					}
					else
					{
						// don't add the entry
						bAddEntry = false;
					}
				}
				else if (strEntryExtensionUpper == "IPL")
				{
					// don't add the entry
					bAddEntry = false;
				}
				else
				{
					// add the entry
				}
			}
			else if (getKGM()->m_umapFilterMapping_COLVersion.count(uiFilterCombo_Version) > 0)
			{
				eCOLVersion eStoredVersion = getKGM()->m_umapFilterMapping_COLVersion[uiFilterCombo_Version];
				eCOLVersion eEntryVersion = pIMGEntry->getCOLVersion() == nullptr ? COL_UNKNOWN : pIMGEntry->getCOLVersion()->getVersionId();

				if (strEntryExtensionUpper != "COL" || eEntryVersion != eStoredVersion)
				{
					bAddEntry = false;
				}
			}
			else if (getKGM()->m_umapFilterMapping_RWVersion.count(uiFilterCombo_Version) > 0)
			{
				eRWVersion eVersion = getKGM()->m_umapFilterMapping_RWVersion[uiFilterCombo_Version];
				if (strEntryExtensionUpper == "DFF" || strEntryExtensionUpper == "TXD")
				{
					if (pIMGEntry->getRWVersion() == nullptr)
					{
						bAddEntry = false;
					}
					else if (pIMGEntry->getRWVersion()->getVersionId() == eVersion)
					{
						// add the entry
					}
					else
					{
						bAddEntry = false;
					}
				}
				else
				{
					bAddEntry = false;
				}
			}
		}

		// extensions
		if (bFilterCheckBox_Extensions)
		{
			vector<string> vecExtensions;
			
			// dropdown
			::CString cstrExtensionText;
			if (uiFilterCombo_Extensions != -1)
			{
				((CComboBox*)getKGM()->getDialog()->GetDlgItem(54))->GetLBText(uiFilterCombo_Extensions, cstrExtensionText);
				vecExtensions.push_back(CStringUtility::convertCStringToStdString(cstrExtensionText));
			}

			// test for a match
			if (std::find(vecExtensions.begin(), vecExtensions.end(), strEntryExtensionUpper) == vecExtensions.end())
			{
				bAddEntry = false;
			}
		}
		*/

		if (bAddEntry)
		{
			addEntryToMainListView(pIMGEntry);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// todo
	//updateEntryCountText();
	//updateIMGText();
}
void					CEntryListWindowTab::addEntryToMainListView(CIMGEntry *pIMGEntry)
{
	CWindowControlEntry_List *pListEntry = new CWindowControlEntry_List;

	pListEntry->setList(getWindow()->getEntryListControl());

	uint32 uiEntryIndex = getWindow()->getEntryListControl()->getEntryCount();
	string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
	bool bFastman92IMGFormat = pIMGEntry->getIMGFile()->getIMGVersion() == IMG_FASTMAN92;

	vector<string> vecText;
	vecText.resize(bFastman92IMGFormat ? 8 : 6);
	vecText[0] = CStringUtility::addNumberGrouping(CStringUtility::toString(uiEntryIndex + 1));
	vecText[1] = strExtensionUpper;
	vecText[2] = pIMGEntry->getEntryName();
	vecText[3] = CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntryOffset()));
	vecText[4] = CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntrySize()));
	vecText[5] = pIMGEntry->getVersionText();
	if (bFastman92IMGFormat)
	{
		vecText[6] = CIMGManager::getCompressionTypeText(pIMGEntry->getCompressionAlgorithmId());
		vecText[7] = CIMGManager::getEncryptionText(pIMGEntry->isEncrypted());
	}

	pListEntry->getText().push_back(vecText);
	getWindow()->getEntryListControl()->addEntry(pListEntry);

	/*
	todo
	uint32 uiEntryIndex = getListView()->GetItemCount();
	string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
	getListView()->InsertItem(LVIF_TEXT | LVIF_PARAM, uiEntryIndex, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(uiEntryIndex + 1))).c_str(), 0, 0, 0, (DWORD)pIMGEntry);
	getListView()->SetItem(uiEntryIndex, 1, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(strExtensionUpper).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 2, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(pIMGEntry->getEntryName()).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 3, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntryOffset()))).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 4, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntrySize()))).c_str(), 0, 0, 0, 0);
	getKGM()->getEntryListWindow()->applyVersionAndResourceTypeColumn(uiEntryIndex, getKGM()->getEntryListTab()->getIMGFile(), pIMGEntry);
	if (pIMGEntry->getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
	{
		getListView()->SetItem(uiEntryIndex, 6, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CIMGManager::getCompressionTypeText(pIMGEntry->getCompressionAlgorithmId())).c_str(), 0, 0, 0, 0);
		getListView()->SetItem(uiEntryIndex, 7, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CIMGManager::getEncryptionText(pIMGEntry->isEncrypted())).c_str(), 0, 0, 0, 0);
	}
	*/
}
void					CEntryListWindowTab::updateEntryInMainListView(CIMGEntry *pIMGEntry)
{
	/*
	todo
	uint32 uiEntryIndex = getMainListViewItemIndexByItemData(pIMGEntry);
	if (uiEntryIndex == -1)
	{
		// IMG entry is not currently displayed, e.g. filter.
		return;
	}
	string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
	getListView()->SetItem(uiEntryIndex, 0, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(uiEntryIndex + 1))).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 1, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(strExtensionUpper).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 2, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(pIMGEntry->getEntryName()).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 3, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntryOffset()))).c_str(), 0, 0, 0, 0);
	getListView()->SetItem(uiEntryIndex, 4, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::addNumberGrouping(CStringUtility::toString(pIMGEntry->getEntrySize()))).c_str(), 0, 0, 0, 0);
	//getKGM()->getEntryListWindow()->applyVersionAndResourceTypeColumn(uiEntryIndex, getKGM()->getEntryListTab()->getIMGFile(), pIMGEntry);
	if (pIMGEntry->getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
	{
		getListView()->SetItem(uiEntryIndex, 6, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CIMGManager::getCompressionTypeText(pIMGEntry->getCompressionAlgorithmId())).c_str(), 0, 0, 0, 0);
		getListView()->SetItem(uiEntryIndex, 7, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CIMGManager::getEncryptionText(pIMGEntry->isEncrypted())).c_str(), 0, 0, 0, 0);
	}
	*/
}
uint32			CEntryListWindowTab::getMainListViewItemIndexByItemData(CIMGEntry *pIMGEntry)
{
	/*
	todo
	for (uint32 i = 0, j = getListView()->GetItemCount(); i < j; i++)
	{
		if ((CIMGEntry*)getListView()->GetItemData(i) == pIMGEntry)
		{
			return i;
		}
	}
	*/
	return -1;
}
void					CEntryListWindowTab::updateEntryCountText(void)
{
	/*
	todo
	uint32
		uiFilteredEntryCount = getListView()->GetItemCount(),
		uiEntryCount = getIMGFile()->getEntryCount();
	if (isFilterActive())
	{
		((CStatic*)getKGM()->getDialog()->GetDlgItem(20))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_FilteredEntryCount", uiFilteredEntryCount, uiEntryCount).c_str());
	}
	else
	{
		((CStatic*)getKGM()->getDialog()->GetDlgItem(20))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_EntryCount", uiEntryCount).c_str());
	}
	*/
}
void					CEntryListWindowTab::updateIMGText(void)
{
	/*
	todo
	string strPlatformName = CPlatformManager::getInstance()->getPlatformName(getIMGFile()->getPlatform());

	if (getIMGFile()->getIMGVersion() == IMG_FASTMAN92)
	{
		uint32 uiEntryCount = getIMGFile()->getEntryCount();
		uint32 uiUncompressedEntryCount = getIMGFile()->getEntryCountForCompressionType(IMGCOMPRESSION_NONE);
		string strVersionSuffix = "";
		if (uiEntryCount == uiUncompressedEntryCount)
		{
			strVersionSuffix = CLocalizationManager::getInstance()->getTranslatedText("CompressionValue_Uncompressed");
		}
		else if (uiUncompressedEntryCount == 0)
		{
			strVersionSuffix = CLocalizationManager::getInstance()->getTranslatedText("CompressionValue_Compressed");
		}
		else
		{
			strVersionSuffix = CLocalizationManager::getInstance()->getTranslatedText("CompressionValue_PartiallyCompressed");
		}
		((CStatic*)getKGM()->getDialog()->GetDlgItem(19))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("IMGVersion", CIMGManager::getIMGVersionName(IMG_FASTMAN92, getIMGFile()->isEncrypted()).c_str(), strPlatformName.c_str(), strVersionSuffix.c_str()).c_str());
	}
	else
	{
		((CStatic*)getKGM()->getDialog()->GetDlgItem(19))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("IMGVersion", CIMGManager::getIMGVersionName(getIMGFile()->getIMGVersion(), getIMGFile()->isEncrypted()).c_str(), strPlatformName.c_str(), CIMGManager::getIMGVersionGames(getIMGFile()->getIMGVersion()).c_str()).c_str());
	}
	*/
}
CIMGEntry*				CEntryListWindowTab::getEntryByName(string strEntryName)
{
	for (auto pIMGEntry : getIMGFile()->getEntries())
	{
		if (strEntryName == pIMGEntry->getEntryName())
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

void					CEntryListWindowTab::rebuild(string strIMGPath, bool bLog)
{
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getIMGFile()->getEntryCount() * 3);
	getIMGFile()->serializeViaFile(strIMGPath == "" ? getIMGFile()->getFilePath() : strIMGPath);
	setIMGModifiedSinceRebuild(false);
	if (bLog)
	{
		log(CLocalizationManager::getInstance()->getTranslatedText("Log_127"));
	}
}
uint32			CEntryListWindowTab::merge(string strPath, vector<string>& vecImportedEntryNames)
{
	return getIMGFile()->merge(strPath, vecImportedEntryNames);
}
void					CEntryListWindowTab::splitSelectedEntries(string strPath, eIMGVersion eIMGVersion, bool bDeleteFromSource, vector<string>& vecSplitEntryNames)
{
	/*
	todo
	vector<CIMGEntry*> vecIMGEntries;
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry = nullptr;
	if (pos == NULL)
	{
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount() * (bDeleteFromSource ? 2 : 1));

	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		vecIMGEntries.push_back(pIMGEntry);
		vecSplitEntryNames.push_back(pIMGEntry->getEntryName());

		if (bDeleteFromSource)
		{
			pListControl->DeleteItem(nItem);

			pos = pListControl->GetFirstSelectedItemPosition();
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getIMGFile()->split(vecIMGEntries, strPath, eIMGVersion);

	if (bDeleteFromSource)
	{
		for (auto pIMGEntry : vecIMGEntries)
		{
			removeEntry(pIMGEntry);

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_128", vecIMGEntries.size(), CPathUtility::getFileName(strPath).c_str()));
	*/
}
void					CEntryListWindowTab::replace(vector<string>& vecPaths, vector<string>& vecReplacedEntryNames)
{
	vector<CIMGEntry*> vecReplacedEntries;
	uint32 uiReplaceCount = getIMGFile()->replaceEntries(vecPaths, vecReplacedEntryNames, vecReplacedEntries);

	for (auto pIMGEntry : vecReplacedEntries)
	{
		getKGM()->getEntryListTab()->onEntryChange(pIMGEntry);
	}
}
bool					sortStdVectorAzCaseInsensitive(CSearchEntry *pSearchEntry1, CSearchEntry *pSearchEntry2)
{
	return strcmp(pSearchEntry1->getIMGEntry()->getEntryName().c_str(), pSearchEntry2->getIMGEntry()->getEntryName().c_str()) < 0;
}
void					CEntryListWindowTab::searchText(void)
{
	/*
	todo

	CListCtrl
		*pListControl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(22),
		*pListControlMain = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	pListControl->DeleteAllItems();
	for (auto pSearchEntry : getKGM()->getEntryListWindow()->getSearchEntries())
	{
		delete pSearchEntry;
	}
	getKGM()->getEntryListWindow()->getSearchEntries().clear();

	string strSearchText = CStringUtility::toUpperCase(m_strSearchText);
	bool bAllTabs = ((CButton*)getKGM()->getDialog()->GetDlgItem(46))->GetCheck() == BST_CHECKED;

	if (strSearchText == "")
	{
		((CStatic*)getKGM()->getDialog()->GetDlgItem(0))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", 0).c_str());
		return;
	}

	vector<CWindowTab*> vecWindowTabs;
	uint32 uiTotalEntryCount;
	if (bAllTabs)
	{
		vecWindowTabs = getKGM()->getEntryListWindow()->getEntries();
		uiTotalEntryCount = getKGM()->getEntryListWindow()->getEntryCountForAllTabs();
	}
	else
	{
		vecWindowTabs.push_back(this);
		uiTotalEntryCount = getIMGFile()->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTotalEntryCount);

	uint32
		uiMatchCount = 0,
		uiFileCountWithMatches = 0;
	for (auto pWindowTab : vecWindowTabs)
	{
		bool bMatchFoundInFile = false;
		uint32 i = 0;
		for (auto pIMGEntry : ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntries())
		{
			string strEntryExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			bool bMatch = false;
			if (CStringUtility::toUpperCase(pIMGEntry->getEntryName()).find(strSearchText) != string::npos)
			{
				bMatch = true;
			}
			else if (CStringUtility::toString(pIMGEntry->getEntryOffsetInSectors() * 2048).find(strSearchText) != string::npos)
			{
				bMatch = true;
			}
			else if (CStringUtility::toString(pIMGEntry->getEntrySize()).find(strSearchText) != string::npos)
			{
				bMatch = true;
			}
			else
			{
				if (strEntryExtension == "COL")
				{
					if (CStringUtility::toUpperCase(CCOLManager::getInstance()->getCOLVersionText(pIMGEntry->getCOLVersion())).find(strSearchText) != string::npos)
					{
						bMatch = true;
					}
				}
				else if (strEntryExtension == "TXD" || CPathUtility::isModelExtension(strEntryExtension))
				{
					if (pIMGEntry->getRWVersion() == nullptr)
					{
						string strText1 = "Unknown";
						if (CStringUtility::toUpperCase(strText1).find(strSearchText) != string::npos)
						{
							bMatch = true;
						}
					}
					else
					{
						if (CStringUtility::toUpperCase(pIMGEntry->getRWVersion()->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pIMGEntry->getRWVersion()->getLocalizationKey()) + ")").find(strSearchText) != string::npos)
						{
							bMatch = true;
						}
					}
				}
			}

			if (bMatch)
			{
				pListControlMain->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
				pListControlMain->SetSelectionMark(i);

				CSearchEntry *pSearchEntry = new CSearchEntry;
				pSearchEntry->setWindowTab((CEntryListWindowTab*)pWindowTab);
				pSearchEntry->setIMGEntry(pIMGEntry);
				getKGM()->getEntryListWindow()->getSearchEntries().push_back(pSearchEntry);

				uiMatchCount++;
				bMatchFoundInFile = true;
			}
			else
			{
				pListControlMain->SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
			}
			i++;

			getKGM()->getTaskManager()->onTaskProgressTick();
		}

		if (bMatchFoundInFile)
		{
			uiFileCountWithMatches++;
		}
	}

	// sort search results list view by entry name A-Z case-insensitive
	std::sort(getKGM()->getEntryListWindow()->getSearchEntries().begin(), getKGM()->getEntryListWindow()->getSearchEntries().end(), sortStdVectorAzCaseInsensitive);

	// add all entries to search results list view
	for (auto pSearchEntry : getKGM()->getEntryListWindow()->getSearchEntries())
	{
		CIMGEntry *pIMGEntry = pSearchEntry->getIMGEntry();
		uint32 uiRowIndex = pListControl->GetItemCount();
		string strEntryExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
		string strExtraInfo;
		if (strEntryExtension == "COL")
		{
			strExtraInfo = CCOLManager::getCOLVersionText(pIMGEntry->getCOLVersion());
		}
		else if (strEntryExtension == "TXD" || CPathUtility::isModelExtension(strEntryExtension))
		{
			strExtraInfo = pIMGEntry->getRWVersion() == nullptr ? CLocalizationManager::getInstance()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown") : pIMGEntry->getRWVersion()->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pIMGEntry->getRWVersion()->getLocalizationKey()) + ")";
		}
		pListControl->InsertItem(LVIF_TEXT | LVIF_PARAM, uiRowIndex, CStringUtility::convertStdStringToStdWString(pIMGEntry->getEntryName()).c_str(), 0, 0, 0, (DWORD)pSearchEntry);
		pListControl->SetItem(uiRowIndex, 1, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CPathUtility::getFileName(pSearchEntry->getWindowTab()->getIMGFile()->getFilePath())).c_str(), 0, 0, 0, 0);
		pListControl->SetItem(uiRowIndex, 2, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(strExtraInfo).c_str(), 0, 0, 0, 0);
	}

	// result text
	wstring wstrSearchResultText;
	if (bAllTabs)
	{
		wstrSearchResultText = CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_AllTabs", uiMatchCount, uiFileCountWithMatches);
	}
	else
	{
		wstrSearchResultText = CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", uiMatchCount);
	}

	((CStatic*)getKGM()->getDialog()->GetDlgItem(0))->SetWindowTextW(wstrSearchResultText.c_str());
	pListControlMain->SetFocus();

	getKGM()->getEntryListWindow()->setSearchHitCount(uiMatchCount);
	getKGM()->getEntryListWindow()->setSearchFileCount(uiFileCountWithMatches);
	*/
}

void					CEntryListWindowTab::storeFilterOptions(void)
{
	/*
	todo
	CComboBox *pComboBox1 = (CComboBox*)getKGM()->getDialog()->GetDlgItem(54);
	CComboBox *pComboBox2 = (CComboBox*)getKGM()->getDialog()->GetDlgItem(5);
	::CString cstr1, cstr2;
	pComboBox1->GetWindowTextW(cstr1);
	pComboBox2->GetWindowTextW(cstr2);
	setActiveFilter("type", CStringUtility::convertCStringToStdString(cstr1));
	setActiveFilter("version", CStringUtility::convertCStringToStdString(cstr2));
	*/
}
void					CEntryListWindowTab::restoreFilterOptions(void)
{
	/*
	todo
	m_bRestoringFilterOptions = true;
	((CButton*)getKGM()->getDialog()->GetDlgItem(44))->SetCheck(m_filterOptions.m_bCheckboxes[0] ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)getKGM()->getDialog()->GetDlgItem(3))->SetCheck(m_filterOptions.m_bCheckboxes[1] ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)getKGM()->getDialog()->GetDlgItem(4))->SetCheck(m_filterOptions.m_bCheckboxes[2] ? BST_CHECKED : BST_UNCHECKED);
	((CButton*)getKGM()->getDialog()->GetDlgItem(10))->SetCheck(m_filterOptions.m_bCheckboxes[3] ? BST_CHECKED : BST_UNCHECKED);
	((CComboBox*)getKGM()->getDialog()->GetDlgItem(7))->SetCurSel(m_filterOptions.m_iComboBoxes[0]);
	((CComboBox*)getKGM()->getDialog()->GetDlgItem(6))->SetCurSel(m_filterOptions.m_iComboBoxes[1]);
	((CComboBox*)getKGM()->getDialog()->GetDlgItem(5))->SetCurSel(m_filterOptions.m_iComboBoxes[2]);
	((CEdit*)getKGM()->getDialog()->GetDlgItem(9))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(m_filterOptions.m_strEditBoxes[0]).c_str());
	((CEdit*)getKGM()->getDialog()->GetDlgItem(8))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(m_filterOptions.m_strEditBoxes[1]).c_str());
	((CEdit*)getKGM()->getDialog()->GetDlgItem(48))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(m_filterOptions.m_strEditBoxes[2]).c_str());
	m_bRestoringFilterOptions = false;
	*/
}
bool					CEntryListWindowTab::isFilterActive(void)
{
	/*
	todo
	bool bFilterCheckBox_Offset = ((CButton*)getKGM()->getDialog()->GetDlgItem(44))->GetCheck() == BST_CHECKED;
	bool bFilterCheckBox_Size = ((CButton*)getKGM()->getDialog()->GetDlgItem(3))->GetCheck() == BST_CHECKED;
	bool bFilterCheckBox_RWVersion = ((CButton*)getKGM()->getDialog()->GetDlgItem(4))->GetCheck() == BST_CHECKED;
	bool bFilterCheckBox_Extensions = ((CButton*)getKGM()->getDialog()->GetDlgItem(10))->GetCheck() == BST_CHECKED;
	
	return bFilterCheckBox_Offset || bFilterCheckBox_Size || bFilterCheckBox_RWVersion || bFilterCheckBox_Extensions;
	*/
	/*
	todo
	return
		((CComboBox*)getKGM()->getDialog()->GetDlgItem(54))->GetCurSel() != 0
		|| ((CComboBox*)getKGM()->getDialog()->GetDlgItem(5))->GetCurSel() != 0;
		*/
	return true;
}

void					CEntryListWindowTab::sortEntries(void)
{
	// sort
	getKGM()->getSortManager()->sort(getIMGFile());

	// log
	vector<string> vecExtendedLogLines;
	CSortPriorities *pSortPriorities = getKGM()->getSortManager()->getSortPriorities();
	uint32 i = 0;
	CSortPriority *pSortPriority2 = nullptr;
	for (auto pSortPriority : pSortPriorities->getEntries())
	{
		if (!pSortPriority->isEnabled())
		{
			break;
		}
		vecExtendedLogLines.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Sort_Priority_N_WithName", (i + 1), pSortPriority->getType()->getText().c_str()));
		i++;
		pSortPriority2 = pSortPriority;
	}
	if (vecExtendedLogLines.size() == 1)
	{
		log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_129", pSortPriority2->getType()->getText().c_str()));
	}
	else
	{
		log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_130", vecExtendedLogLines.size()));
	}
	log(CLocalizationManager::getInstance()->getTranslatedText("Log_131"), true);
	log(CStringUtility::join(vecExtendedLogLines, "\n"), true);

	// render
	readdAllEntriesToMainListView();

	// post
	setIMGModifiedSinceRebuild(true);
}

void					CEntryListWindowTab::onEntryChange(CIMGEntry *pIMGEntry)
{
	loadProtectedEntryState(pIMGEntry);
}

void					CEntryListWindowTab::loadProtectedEntryState(CIMGEntry *pIMGEntry)
{
	if (m_pDBFile->isIMGEntryFound(getIMGFile(), pIMGEntry))
	{
		pIMGEntry->setProtectedEntry(true);
	}
	else
	{
		pIMGEntry->setProtectedEntry(false);
	}
}
void				CEntryListWindowTab::loadProtectedEntryStates(void)
{
	for (auto pIMGEntry : getIMGFile()->getEntries())
	{
		loadProtectedEntryState(pIMGEntry);
	}
}

void				CEntryListWindowTab::loadFilter_Type(void)
{
	/*
	todo
	CComboBox *pComboBox = (CComboBox*)getKGM()->getDialog()->GetDlgItem(54);
	unloadFilter_Type();
	vector<string> vecExtensions = getIMGFile()->getEntryExtensions();
	CVectorUtility::sortAZCaseInsensitive(vecExtensions);
	uint32
		i = 1,
		uiCurSel = 0;
	for (auto strExtension : vecExtensions)
	{
		pComboBox->InsertString(i, CStringUtility::convertStdStringToStdWString(strExtension).c_str());
		if (getActiveFilter("type") == strExtension)
		{
			uiCurSel = i;
		}
		i++;
	}
	pComboBox->SetCurSel(uiCurSel);
	*/
}
void				CEntryListWindowTab::loadFilter_Version(void)
{
	/*
	todo
	CComboBox *pComboBox = (CComboBox*)getKGM()->getDialog()->GetDlgItem(5);
	unloadFilter_Version();

	todo
	//getKGM()->m_umapFilterMapping_COLVersion.clear();
	//getKGM()->m_umapFilterMapping_RWVersion.clear();
	//getKGM()->m_iFilterMapping_UnknownVersion = 0;

	vector<eCOLVersion> vecCOLVersions;
	vector<eRWVersion> vecRWVersions;
	vector<string> vecVersions = getIMGFile()->getEntryVersions(vecCOLVersions, vecRWVersions);
	uint32
		i = 1,
		uiCurSel = 0;
	string strUnknownVersionText = CLocalizationManager::getInstance()->getTranslatedText("UnknownVersion");
	for (auto strVersionText : vecVersions)
	{
		pComboBox->InsertString(i, CStringUtility::convertStdStringToStdWString(strVersionText).c_str());
		if (getActiveFilter("version") == strVersionText)
		{
			uiCurSel = i;
		}

		todo
		if(strVersionText == strUnknownVersionText)
		{
			getKGM()->m_iFilterMapping_UnknownVersion = i;
		}
		else if (i <= vecCOLVersions.size())
		{
			getKGM()->m_umapFilterMapping_COLVersion[i] = vecCOLVersions[i - 1];
		}
		else
		{
			getKGM()->m_umapFilterMapping_RWVersion[i] = vecRWVersions[(i - 1) - vecCOLVersions.size()];
		}

		i++;
	}
	pComboBox->SetCurSel(uiCurSel);
	*/
}

void				CEntryListWindowTab::unloadFilter_Type(void)
{
	/*
	todo
	CComboBox *pComboBox = (CComboBox*)getKGM()->getDialog()->GetDlgItem(54);

	for (uint32 i = 0, j = pComboBox->GetCount(); i < j; i++)
	{
		pComboBox->DeleteString(0);
	}

	pComboBox->InsertString(0, CLocalizationManager::getInstance()->getTranslatedTextW("AllTypes").c_str());

	pComboBox->SetCurSel(0);
	*/
}
void				CEntryListWindowTab::unloadFilter_Version(void)
{
	/*
	todo
	CComboBox *pComboBox = (CComboBox*)getKGM()->getDialog()->GetDlgItem(5);

	for (uint32 i = 0, j = pComboBox->GetCount(); i < j; i++)
	{
		pComboBox->DeleteString(0);
	}

	pComboBox->InsertString(0, CLocalizationManager::getInstance()->getTranslatedTextW("AllVersions").c_str());

	pComboBox->SetCurSel(0);
	*/
}

void				CEntryListWindowTab::reassignEntryIds(void)
{
	/*
	todo
	CListCtrl *pListControl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		pListControl->SetItem(i, 0, LVIF_TEXT, CStringUtility::convertStdStringToStdWString(CStringUtility::toString(i + 1)).c_str(), 0, 0, 0, 0);
	}
	*/
}

vector<CIMGEntry*>	CEntryListWindowTab::getSelectedEntries(void)
{
	/*
	todo
	vector<CIMGEntry*> vecIMGEntries;

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return vecIMGEntries;
	}

	CIMGEntry *pIMGEntry;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);

		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		vecIMGEntries.push_back(pIMGEntry);
	}

	return vecIMGEntries;
	*/
	vector<CIMGEntry*> vecIMGEntries;
	return vecIMGEntries;
}