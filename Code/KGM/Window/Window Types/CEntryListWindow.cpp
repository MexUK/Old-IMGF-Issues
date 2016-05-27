#include "CEntryListWindow.h"
#include "Globals.h"
#include "CKGM.h"
#include "IMG/CIMGManager.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "File/CFileUtility.h"
#include "Localization/CLocalizationManager.h"
#include "CSettingsManager.h"
#include "COL/CCOLManager.h"
#include "IMG/CIMGEntry.h"
#include "Engine/RW/CRWVersion.h"
#include "Task/CTaskManager.h"

// for menu start - todo
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Sort/CSortManager.h"
#include "Sort/CSortType.h"
#include "Sort/CSortPriority.h"
#include "Sort/CSortTypes.h"
#include "Sort/CSortPriorities.h"
#include "Sort/CSortMenuItems.h"
#include "Image/CImageManager.h"
#include "Image/CRasterDataFormatManager.h"
#include "COL/CCOLManager.h"
#include "COL/CCOLVersionManager.h"
#include "COL/CCOLVersion.h"
// for menu end

using namespace std;

CEntryListWindowTab*		CEntryListWindow::addBlankTab(string strIMGPath, eIMGVersion eIMGVersionValue)
{
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(1);

	CIMGFormat *pIMGFormat = new CIMGFormat;
	pIMGFormat->setFilePath(strIMGPath);
	pIMGFormat->setIMGVersion(eIMGVersionValue);

	CEntryListWindowTab *pWindowTab = _addTab(pIMGFormat);
	if (!pWindowTab)
	{
		getKGM()->getTaskManager()->onTaskProgressTick();
		return nullptr;
	}

	pWindowTab->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_26", CPathUtility::getFileName(strIMGPath).c_str()));
	pWindowTab->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_27", CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, pIMGFormat->isEncrypted()).c_str()), true);
	getKGM()->getTaskManager()->onTaskProgressTick();

	return pWindowTab;
}

CEntryListWindowTab*		CEntryListWindow::addTab(string strIMGPath, eIMGVersion eIMGVersionValue)
{
	uint32 uiMultiplier = eIMGVersionValue == IMG_3 ? 4 : 3; // x3 for: 1 for reading entry data (parsing), 1 for reading RW versions (parsing), and 1 for adding all entries to main list view. and x4 (+1) for version 3 entry names.
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(CIMGManager::getIMGEntryCount(strIMGPath, eIMGVersionValue) * uiMultiplier);

	CIMGFormat *pIMGFormat = CIMGManager::getInstance()->parseViaFile(strIMGPath/*, eIMGVersionValue - todo*/);

	CEntryListWindowTab *pWindowTab = _addTab(pIMGFormat);
	if (!pWindowTab)
	{
		getKGM()->getTaskManager()->onTaskProgressComplete();
		return nullptr;
	}

	pWindowTab->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_28", CPathUtility::getFileName(strIMGPath).c_str()));
	pWindowTab->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_27", CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, pIMGFormat->isEncrypted()).c_str()), true);
	pWindowTab->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_29", CFileUtility::getFileSize(strIMGPath)), true);
	
	return pWindowTab;
}

CEntryListWindowTab*		CEntryListWindow::_addTab(CIMGFormat *pIMGFormat)
{
	uint32 uiTabIndex = getNextTabIndex();

	CEntryListWindowTab *pWindowTab = new CEntryListWindowTab;
	pWindowTab->setWindow(this);
	pWindowTab->setIndex(uiTabIndex);
	pWindowTab->setIMGFile(pIMGFormat);

	if (pWindowTab->onTabFormatReady())
	{
		addEntry(pWindowTab);
		setActiveTab(pWindowTab);
		return pWindowTab;
	}
	else
	{
		pWindowTab->unload();
		delete pWindowTab;

		getKGM()->getTaskManager()->setTaskMaxProgressTickCount(1); // todo - have like getKGM()->getTaskManager()->setProgressComplete(void) instead of these 2 lines
		getKGM()->getTaskManager()->onTaskProgressTick();

		return nullptr;
	}
}

void				CEntryListWindow::removeTab(CWindowTab *pWindowTab)
{
	// fetch current tab index
	uint32 uiTabIndex = pWindowTab->getIndex();

	// remove from pool and unload
	removeEntry(pWindowTab);

	// shift down higher indices by 1
	//((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->DeleteItem(uiTabIndex);
	for (CWindowTab *pWindowTab2 : getEntries())
	{
		if (pWindowTab2->getIndex() > uiTabIndex)
		{
			pWindowTab2->setIndex(pWindowTab2->getIndex() - 1);
		}
	}

	// set active tab
	if (getEntryCount() == 0)
	{
		// blank
		setActiveTab(nullptr);
	}
	else if (getEntryByIndex(uiTabIndex) != nullptr)
	{
		// tab to right
		setActiveTab((CEntryListWindowTab*)getEntryByIndex(uiTabIndex));
	}
	else if (getEntryByIndex(uiTabIndex - 1) != nullptr)
	{
		// tab to left
		setActiveTab((CEntryListWindowTab*)getEntryByIndex(uiTabIndex - 1));
	}
	else
	{
		// first tab
		setActiveTab((CEntryListWindowTab*)getEntryByIndex(0));
	}
}

void				CEntryListWindow::setActiveTab(CEntryListWindowTab *pWindowTab)
{
	CTabbedWindow::setActiveTab(pWindowTab);
	
	pWindowTab->readdAllEntriesToMainListView();
	
	// todo
	return;

	vector<uint16> vecButtonIds = {16,17,18,25,26,27,28,29,31,32,33,34,35,36,40,41,42,43,45,47};

	if (pWindowTab == nullptr)
	{
		/*
		todo
		getKGM()->getTaskManager()->setTaskMaxProgressTickCount(((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37))->GetItemCount());

		// disable buttons
		for (uint16 usButtonId : vecButtonIds)
		{
			((CButton*)getKGM()->getDialog()->GetDlgItem(usButtonId))->EnableWindow(0);
		}

		// IMG path
		((CEdit*)getKGM()->getDialog()->GetDlgItem(38))->SetWindowTextW(_T(""));

		// IMG entry count
		((CStatic*)getKGM()->getDialog()->GetDlgItem(20))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_EntryCount", 0).c_str());

		// selected entry count
		((CStatic*)getKGM()->getDialog()->GetDlgItem(51))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SelectedEntryCount", 0).c_str());

		// IMG version
		((CStatic*)getKGM()->getDialog()->GetDlgItem(19))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_Text_IMGVersion").c_str());

		// 6th column name in main list view
		readdColumnsToMainListView(IMG_UNKNOWN);

		// search text
		((CEdit*)getKGM()->getDialog()->GetDlgItem(24))->SetWindowTextW(_T(""));
		((CStatic*)getKGM()->getDialog()->GetDlgItem(0))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", 0).c_str());
		((CListCtrl*)getKGM()->getDialog()->GetDlgItem(22))->DeleteAllItems();
		for (auto pSearchEntry : m_vecSearchEntries)
		{
			delete pSearchEntry;
		}
		m_vecSearchEntries.clear();

		// filter
		pWindowTab->unloadFilter_Type();
		pWindowTab->unloadFilter_Version();

		// log
		((CEdit*)getKGM()->getDialog()->GetDlgItem(14))->SetWindowTextW(_T(""));

		// IMG entries
		//((CListCtrl*)m_pDialog->GetDlgItem(37))->DeleteAllItems();
		CListCtrl *pMainListCtrl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
		for (uint32 i = 0, j = pMainListCtrl->GetItemCount(); i < j; i++)
		{
			pMainListCtrl->DeleteItem((j - 1) - i);
			getKGM()->getTaskManager()->onTaskProgressTick();
		}
		*/
	}
	else
	{
		/*
		todo
		// enable buttons
		for (uint16 usButtonId : vecButtonIds)
		{
			((CButton*)getKGM()->getDialog()->GetDlgItem(usButtonId))->EnableWindow(1);
		}

		// tab focus
		((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->SetCurFocus(pWindowTab->getIndex());

		// IMG path
		((CEdit*)getKGM()->getDialog()->GetDlgItem(38))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(pWindowTab->getIMGFile()->getFilePath()).c_str());

		// IMG entry count
		pWindowTab->updateEntryCountText();

		// selected entry count
		m_uiSelectedEntryCount = 0;
		((CStatic*)getKGM()->getDialog()->GetDlgItem(51))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SelectedEntryCount", 0).c_str());

		// IMG version
		pWindowTab->updateIMGText();

		// 6th column name in main list view
		readdColumnsToMainListView(pWindowTab->getIMGFile()->getIMGVersion());

		// search text
		if (((CButton*)getKGM()->getDialog()->GetDlgItem(46))->GetCheck() == BST_UNCHECKED)
		{
			((CEdit*)getKGM()->getDialog()->GetDlgItem(24))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(pWindowTab->getSearchText()).c_str());
			pWindowTab->searchText();
		}

		// filter
		pWindowTab->loadFilter_Type();
		pWindowTab->loadFilter_Version();

		// log
		CEdit *pEdit = ((CEdit*)getKGM()->getDialog()->GetDlgItem(14));
		pEdit->SetWindowTextW(CStringUtility::convertStdStringToStdWString(CStringUtility::join(pWindowTab->getLogLinesGUI(), "\r\n")).c_str());
		pEdit->LineScroll(pEdit->GetLineCount());

		// IMG entries
		pWindowTab->readdAllEntriesToMainListView();
		*/
	}
}

void					CEntryListWindow::readdColumnsToMainListView(eIMGVersion eIMGVersionValue)
{
	removeColumnsFromMainListView();
	addColumnsToMainListView(eIMGVersionValue);
}
void					CEntryListWindow::addColumnsToMainListView(eIMGVersion eIMGVersionValue)
{
	/*
	todo
	CListCtrl *pListCtrl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	pListCtrl->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	pListCtrl->InsertColumn(0, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_ListView_ColumnTitle_ID").c_str(), LVCFMT_LEFT, 45);
	pListCtrl->InsertColumn(1, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_ListView_ColumnTitle_Type").c_str(), LVCFMT_LEFT, 40);
	pListCtrl->InsertColumn(2, CLocalizationManager::getInstance()->getTranslatedTextW("Window_Main_ListView_ColumnTitle_Name").c_str(), LVCFMT_LEFT, 150);
	pListCtrl->InsertColumn(3, CLocalizationManager::getInstance()->getTranslatedTextW("Offset").c_str(), LVCFMT_RIGHT, 85);
	pListCtrl->InsertColumn(4, CLocalizationManager::getInstance()->getTranslatedTextW("Size").c_str(), LVCFMT_RIGHT, 70);

	switch (eIMGVersionValue)
	{
	case IMG_UNKNOWN:
	case IMG_1:
	case IMG_2:
		pListCtrl->InsertColumn(5, CLocalizationManager::getInstance()->getTranslatedTextW("Version").c_str(), LVCFMT_LEFT, 125);
		break;
	case IMG_3:
		pListCtrl->InsertColumn(5, CLocalizationManager::getInstance()->getTranslatedTextW("ResourceType").c_str(), LVCFMT_LEFT, 125);
		break;
	case IMG_FASTMAN92:
		pListCtrl->InsertColumn(5, CLocalizationManager::getInstance()->getTranslatedTextW("Version").c_str(), LVCFMT_LEFT, 125);
		pListCtrl->InsertColumn(6, CLocalizationManager::getInstance()->getTranslatedTextW("Compression").c_str(), LVCFMT_LEFT, 125);
		pListCtrl->InsertColumn(7, CLocalizationManager::getInstance()->getTranslatedTextW("Encryption").c_str(), LVCFMT_LEFT, 125);
		break;
	}
	*/
}
void					CEntryListWindow::removeColumnsFromMainListView(void)
{
	/*
	todo
	CListCtrl *pListCtrl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	while (pListCtrl->DeleteColumn(0) != 0)
	{
	}
	*/
}
int						CEntryListWindow::getMainListControlItemByEntry(CIMGEntry *pIMGEntry)
{
	/*
	todo
	CListCtrl *pListControlMain = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	for (uint32 i = 0, j = pListControlMain->GetItemCount(); i < j; i++)
	{
		if (pListControlMain->GetItemData(i) == (DWORD)pIMGEntry)
		{
			return i;
		}
	}
	*/
	return 0;
}

void					CEntryListWindow::onSelectIMGEntry(bool bEntryIsSelected)
{
	// todo - have onSelectIMGEntry and onUnselectIMGEntry
	if (bEntryIsSelected)
	{
		m_uiSelectedEntryCount++;
	}
	else
	{
		m_uiSelectedEntryCount--;
	}

	// selected entry count
	updateSelectedEntryCountText();
}

void					CEntryListWindow::updateSelectedEntryCountText(void)
{
	// todo ((CStatic*)getKGM()->getDialog()->GetDlgItem(51))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SelectedEntryCount", m_uiSelectedEntryCount).c_str());
}

void					CEntryListWindow::logAllTabs(string strText, bool bExtendedModeOnly)
{
	for (auto pWindowTab : getEntries())
	{
		((CEntryListWindowTab*)pWindowTab)->log(strText, bExtendedModeOnly);
	}
}

void					CEntryListWindow::logWithNoTabsOpen(string strText, bool bExtendedModeOnly)
{
	string strLogEntryWithTimestampAndIMG = "[" + CStringUtility::getTimestampText() + "] [" + CLocalizationManager::getInstance()->getTranslatedText("NoTabsOpen") + "] " + strText;

	if (bExtendedModeOnly)
	{
		//m_vecLogLinesExtended.push_back(strLogEntryWithTimestampAndIMG);

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
		//m_vecLogLinesGUI.push_back(strText);
		//m_vecLogLinesBasic.push_back(strLogEntryWithTimestampAndIMG);
		//m_vecLogLinesExtended.push_back(strLogEntryWithTimestampAndIMG);

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
}

uint32					CEntryListWindow::getEntryCountForAllTabs(void)
{
	uint32 uiTotalEntryCount = 0;
	for (auto pWindowTab : getEntries())
	{
		uiTotalEntryCount += ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntryCount();
	}
	return uiTotalEntryCount;
}

vector<CIMGFormat*>		CEntryListWindow::getAllMainWindowTabsIMGFiles(void)
{
	vector<CIMGFormat*> veCIMGFormats;
	for (auto pWindowTab : getEntries())
	{
		veCIMGFormats.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile());
	}
	return veCIMGFormats;
}

void					CEntryListWindow::refreshActiveTab(void)
{
	setActiveTab(getActiveTab());
}

void					CEntryListWindow::initMenu(void) // todo - move menu stuff to like CMenuManager
{
	/*
	HMENU hMenubar = CreateMenu();

	getKGM()->m_hMenu_File = CreateMenu();
	HMENU
		hMenu_File = getKGM()->m_hMenu_File,
		hMenu_Edit = CreateMenu(),
		hMenu_DAT = CreateMenu(),
		hMenu_IMG = CreateMenu(),
		hMenu_Model = CreateMenu(),
		hMenu_Texture = CreateMenu(),
		hMenu_Collision = CreateMenu(),
		hMenu_ItemDefinition = CreateMenu(),
		hMenu_ItemPlacement = CreateMenu(),
		hMenu_Entry = CreateMenu(),
		hMenu_Settings = CreateMenu(),
		hMenu_Help = CreateMenu()
		;

	HMENU hMenu_File_New = CreatePopupMenu();
	HMENU hMenu_File_New_Version3 = CreatePopupMenu();
	HMENU hMenu_File_SaveLog = CreatePopupMenu();
	HMENU hMenu_File_SaveLog_ActiveTab = CreatePopupMenu();
	HMENU hMenu_File_SaveLog_AllTabs = CreatePopupMenu();
	getKGM()->m_hSubMenu_File_OpenRecent = CreatePopupMenu();
	getKGM()->m_hSubMenu_File_Sessions = CreatePopupMenu();
	HMENU hMenu_File_Sessions = CreateMenu();

	HMENU hMenu_Edit_Select = CreatePopupMenu();
	HMENU hMenu_Edit_SelectViaRWVersion = CreatePopupMenu();

	HMENU hMenu_IMG_ConvertTo = CreatePopupMenu();
	HMENU hMenu_IMG_ConvertTo_Version3 = CreatePopupMenu();
	HMENU hMenu_IMG_Split = CreatePopupMenu();
	HMENU hMenu_IMG_Validation = CreatePopupMenu();

	getKGM()->m_hMenu_Entry_Sort = CreatePopupMenu();
	HMENU hMenu_Entry_Import = CreatePopupMenu();
	HMENU hMenu_Entry_Export = CreatePopupMenu();
	HMENU hMenu_Entry_Remove = CreatePopupMenu();
	HMENU hMenu_Entry_Replace = CreatePopupMenu();
	HMENU hMenu_Entry_Case = CreatePopupMenu();
	HMENU hMenu_Entry_Copy = CreatePopupMenu();
	HMENU hMenu_Entry_LowerCase = CreatePopupMenu();
	HMENU hMenu_Entry_UpperCase = CreatePopupMenu();
	HMENU hMenu_Entry_TitleCase = CreatePopupMenu();
	HMENU hMenu_Entry_Shift = CreatePopupMenu();

	HMENU hMenu_Collision_Convert = CreatePopupMenu();

	HMENU hMenu_Model_Convert = CreatePopupMenu();
	HMENU hMenu_Model_Convert_DFFRWVersion = CreatePopupMenu();
	HMENU hMenu_Model_Convert_DFFFileFormat = CreatePopupMenu();

	HMENU hMenu_Texture_Convert = CreatePopupMenu();
	HMENU hMenu_Texture_Convert_TXDGame = CreatePopupMenu();
	HMENU hMenu_Texture_Convert_TXDRWVersion = CreatePopupMenu();
	HMENU hMenu_Texture_Convert_TXDTextureFormat = CreatePopupMenu();
	HMENU hMenu_Texture_Convert_WTDFileFormat = CreatePopupMenu();

	HMENU hMenu_ItemDefinition_FindOrphanEntries = CreatePopupMenu();
	HMENU hMenu_ItemDefinition_FindMissing = CreatePopupMenu();

	HMENU hMenu_ItemPlacement_FindMissing = CreatePopupMenu();

	HMENU hMenu_Entry_Sort_Priorities[10];
	for (uint32 i = 0; i < 10; i++)
	{
		hMenu_Entry_Sort_Priorities[i] = CreatePopupMenu();
	}

	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_File, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_File").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Edit, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Edit").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_DAT, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_DAT").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_IMG, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_IMG").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Model, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Model").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Texture, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Texture").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Collision, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Collision").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_ItemDefinition, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ItemDefinition").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_ItemPlacement, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ItemPlacement").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Entry, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Entry").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Settings, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Settings").c_str());
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_Help, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Help").c_str());

	//AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hMenu_File, _T("&File"));
	AppendMenu(hMenu_File, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_New, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NewIMG").c_str());
	AppendMenu(hMenu_File_New, MF_STRING, 1108, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_V1").c_str());
	AppendMenu(hMenu_File_New, MF_STRING, 1109, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_V2").c_str());
	AppendMenu(hMenu_File_New, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_New_Version3, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_V3").c_str());
	AppendMenu(hMenu_File_New_Version3, MF_STRING, 1112, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_V3_Encrypted").c_str());
	AppendMenu(hMenu_File_New_Version3, MF_STRING, 1113, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_V3_Unencrypted").c_str());
	AppendMenu(hMenu_File_New, MF_SEPARATOR, 1942, _T(""));
	AppendMenu(hMenu_File_New, MF_STRING, 1943, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_New_VFastman92").c_str());
	AppendMenu(hMenu_File, MF_STRING, 2084, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NewWindow").c_str());
	AppendMenu(hMenu_File, MF_SEPARATOR, 1760, _T(""));
	AppendMenu(hMenu_File, MF_STRING, 1100, CLocalizationManager::getInstance()->getTranslatedTextW("Open").c_str());
	AppendMenu(hMenu_File, MF_STRING | MF_POPUP, (UINT_PTR)getKGM()->m_hSubMenu_File_OpenRecent, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OpenRecent").c_str());
	AppendMenu(hMenu_File, MF_STRING, 1117, CStringUtility::convertStdStringToStdWString(CLocalizationManager::getInstance()->getTranslatedText("Menu_OpenLast") + (getKGM()->getRecentlyOpenManager()->getLastOpenEntry() == "" ? "" : " (" + CPathUtility::getFileName(getKGM()->getRecentlyOpenManager()->getLastOpenEntry()) + ")")).c_str());
	AppendMenu(hMenu_File, MF_STRING, 1114, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Reopen").c_str());
	//AppendMenu(hMenu_File, MF_STRING, 1109, _T("Associate IMG extension"));
	AppendMenu(hMenu_File, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_Sessions, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Sessions").c_str());
	AppendMenu(hMenu_File_Sessions, MF_STRING | MF_POPUP, (UINT_PTR)getKGM()->m_hSubMenu_File_Sessions, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_LoadSession").c_str());
	AppendMenu(hMenu_File_Sessions, MF_STRING, 1111, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_SaveSession").c_str());
	AppendMenu(hMenu_File_Sessions, MF_STRING, 1118, CLocalizationManager::getInstance()->getTranslatedTextW("SessionManager").c_str());
	AppendMenu(hMenu_File, MF_SEPARATOR, 1761, _T(""));
	AppendMenu(hMenu_File, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_SaveLog, CLocalizationManager::getInstance()->getTranslatedTextW("Log").c_str());
	AppendMenu(hMenu_File_SaveLog, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_SaveLog_ActiveTab, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab").c_str());
	AppendMenu(hMenu_File_SaveLog, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_File_SaveLog_AllTabs, CLocalizationManager::getInstance()->getTranslatedTextW("AllTabs").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1101, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_SaveBasic").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1102, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_SaveExtended").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1121, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_Clear").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1103, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_OpenFileBasic").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1104, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_OpenFileExtended").c_str());
	AppendMenu(hMenu_File_SaveLog_ActiveTab, MF_STRING, 1120, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_ActiveTab_OpenFolder").c_str());
	AppendMenu(hMenu_File_SaveLog_AllTabs, MF_STRING, 1115, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_AllTabs_SaveBasic").c_str());
	AppendMenu(hMenu_File_SaveLog_AllTabs, MF_STRING, 1116, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_AllTabs_SaveExtended").c_str());
	AppendMenu(hMenu_File_SaveLog_AllTabs, MF_STRING, 1122, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Log_AllTabs_Clear").c_str());
	AppendMenu(hMenu_File, MF_SEPARATOR, 1762, _T(""));
	AppendMenu(hMenu_File, MF_STRING, 1926, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_RepeatLastAction").c_str());
	AppendMenu(hMenu_File, MF_STRING, 1179, CLocalizationManager::getInstance()->getTranslatedTextW("RenameIMG").c_str());
	AppendMenu(hMenu_File, MF_STRING, 1752, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OpenIMGFolder").c_str());
	AppendMenu(hMenu_File, MF_SEPARATOR, 1927, _T(""));
	AppendMenu(hMenu_File, MF_STRING, 1105, CLocalizationManager::getInstance()->getTranslatedTextW("Close").c_str());
	AppendMenu(hMenu_File, MF_STRING, 1106, CLocalizationManager::getInstance()->getTranslatedTextW("CloseAll").c_str());
	AppendMenu(hMenu_File, MF_STRING, 1107, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ExitTool").c_str());

	AppendMenu(hMenu_Edit, MF_STRING, 1180, CLocalizationManager::getInstance()->getTranslatedTextW("Find").c_str());
	AppendMenu(hMenu_Edit, MF_STRING, 1181, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Find_AllTabs").c_str());
	AppendMenu(hMenu_Edit, MF_SEPARATOR, 1720, _T(""));
	AppendMenu(hMenu_Edit, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Edit_Select, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Select").c_str());
	AppendMenu(hMenu_Edit_Select, MF_STRING, 1167, CLocalizationManager::getInstance()->getTranslatedTextW("Select_All").c_str());
	AppendMenu(hMenu_Edit_Select, MF_STRING, 1168, CLocalizationManager::getInstance()->getTranslatedTextW("Select_Inverse").c_str());
	AppendMenu(hMenu_Edit_Select, MF_SEPARATOR, 1730, _T(""));
	AppendMenu(hMenu_Edit_Select, MF_STRING, 1560, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Select_Extensions").c_str());
	AppendMenu(hMenu_Edit_Select, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Edit_SelectViaRWVersion, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Select_RWVersion").c_str());
	AppendMenu(hMenu_Edit_Select, MF_STRING, 1900, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Select_IDE").c_str());
	uint32 i = 0;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		getKGM()->m_umapMenuItemMapping_SelectRWVersion[1561 + i] = pRWVersion;
		string strMenuText = CLocalizationManager::getInstance()->getTranslatedText("Menu_Select_RWVersion_Prefix") + " " + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(0, 2)) + "&" + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(2));
		AppendMenu(hMenu_Edit_SelectViaRWVersion, MF_STRING, 1561 + i, CStringUtility::convertStdStringToStdWString(strMenuText).c_str());
		i++;
	}
	AppendMenu(hMenu_Edit_SelectViaRWVersion, MF_SEPARATOR, 1731, _T(""));
	AppendMenu(hMenu_Edit_SelectViaRWVersion, MF_STRING, 1568, CStringUtility::convertStdStringToStdWString(CLocalizationManager::getInstance()->getTranslatedText("Menu_Select_RWVersion_Prefix") + " " + CLocalizationManager::getInstance()->getTranslatedText("Window_Main_Combo_RWVersion_Unknown")).c_str());
	AppendMenu(hMenu_Edit, MF_SEPARATOR, 1185, _T(""));
	AppendMenu(hMenu_Edit, MF_STRING, 1182, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import").c_str());
	AppendMenu(hMenu_Edit, MF_STRING, 1183, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Replace").c_str());
	AppendMenu(hMenu_Edit, MF_STRING, 1184, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove").c_str());

	AppendMenu(hMenu_DAT, MF_STRING, 2056, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_DATPathsMover").c_str());
	AppendMenu(hMenu_DAT, MF_STRING, 2057, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_DATModelList").c_str());

	AppendMenu(hMenu_IMG, MF_STRING, 1140, CLocalizationManager::getInstance()->getTranslatedTextW("Rebuild").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 1141, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_RebuildAs").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 1142, CLocalizationManager::getInstance()->getTranslatedTextW("RebuildAll").c_str());
	AppendMenu(hMenu_IMG, MF_SEPARATOR, 1740, _T(""));
	AppendMenu(hMenu_IMG, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_IMG_ConvertTo, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion").c_str());
	AppendMenu(hMenu_IMG_ConvertTo, MF_STRING, 1143, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion1").c_str());
	AppendMenu(hMenu_IMG_ConvertTo, MF_STRING, 1144, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion2").c_str());
	AppendMenu(hMenu_IMG_ConvertTo, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_IMG_ConvertTo_Version3, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion3").c_str());
	AppendMenu(hMenu_IMG_ConvertTo_Version3, MF_STRING, 1145, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion3Encrypted").c_str());
	AppendMenu(hMenu_IMG_ConvertTo_Version3, MF_STRING, 1146, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersion3Unencrypted").c_str());
	AppendMenu(hMenu_IMG_ConvertTo, MF_SEPARATOR, 2200, _T(""));
	AppendMenu(hMenu_IMG_ConvertTo, MF_STRING, 2201, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ConvertToVersionFastman92").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 1147, CLocalizationManager::getInstance()->getTranslatedTextW("Merge").c_str());
	AppendMenu(hMenu_IMG, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_IMG_Split, CLocalizationManager::getInstance()->getTranslatedTextW("Split").c_str());
	AppendMenu(hMenu_IMG_Split, MF_STRING, 1156, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_SplitSelected").c_str());
	AppendMenu(hMenu_IMG_Split, MF_STRING, 1157, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_SplitIDE").c_str());
	AppendMenu(hMenu_IMG_Split, MF_STRING, 1158, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_SplitTextLines").c_str());
	AppendMenu(hMenu_IMG, MF_SEPARATOR, 1741, _T(""));

	AppendMenu(hMenu_IMG, MF_STRING | MF_POPUP, (UINT_PTR)getKGM()->m_hMenu_Entry_Sort, CLocalizationManager::getInstance()->getTranslatedTextW("Sort").c_str());
	for (uint32 i = 0; i < 10; i++)
	{
		AppendMenu(getKGM()->m_hMenu_Entry_Sort, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Sort_Priorities[i], CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Sort_Priority_N", (i + 1)).c_str());
		uint32 i2 = 0;
		for (auto pSortType : getKGM()->getSortManager()->getSortTypes()->getEntries())
		{
			CSortPriority *pSortPriority = getKGM()->getSortManager()->getSortPriorities()->getEntryByIndex((uint16)i);

			uint32 uiMenuHandle = 1200 + (20 * i) + i2;
			AppendMenu(hMenu_Entry_Sort_Priorities[i], MF_STRING, uiMenuHandle, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Sort_ByText", pSortType->getTextForMenu().c_str()).c_str());
			getKGM()->getSortManager()->getSortMenuItems()->addEntry(new CSortMenuItem(uiMenuHandle, pSortPriority, pSortType));
			i2++;
		}
	}

	AppendMenu(hMenu_IMG, MF_STRING, 2050, CLocalizationManager::getInstance()->getTranslatedTextW("Renamer").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 1155, CLocalizationManager::getInstance()->getTranslatedTextW("UpdateLST").c_str());
	AppendMenu(hMenu_IMG, MF_SEPARATOR, 1743, _T(""));
	AppendMenu(hMenu_IMG, MF_STRING, 2090, CLocalizationManager::getInstance()->getTranslatedTextW("CloneIMG").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 1153, CLocalizationManager::getInstance()->getTranslatedTextW("DuplicateEntries").c_str());
	AppendMenu(hMenu_IMG, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_IMG_Validation, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Validation").c_str());
	AppendMenu(hMenu_IMG_Validation, MF_STRING, 2002, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Validation_SaveIMGSignature").c_str());
	AppendMenu(hMenu_IMG_Validation, MF_STRING, 2003, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Validation_VerifyIMGSignature").c_str());
	AppendMenu(hMenu_IMG_Validation, MF_STRING, 2005, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Validation_DFF").c_str());
	AppendMenu(hMenu_IMG_Validation, MF_STRING, 2006, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Validation_TXD").c_str());
	AppendMenu(hMenu_IMG, MF_STRING, 2004, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Compare").c_str());
	AppendMenu(hMenu_IMG, MF_SEPARATOR, 1742, _T(""));
	AppendMenu(hMenu_IMG, MF_STRING, 1149, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Stats").c_str());
	AppendMenu(hMenu_IMG, MF_SEPARATOR, 2052, _T(""));
	AppendMenu(hMenu_IMG, MF_STRING, 2053, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_IMGVersionSettings").c_str());

	//AppendMenu(hMenu_Model, MF_STRING, 2105, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ExtractDVCAndNVCColoursIntoDFFs").c_str());
	//AppendMenu(hMenu_Model, MF_STRING, 2106, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Extract2dEffectsIntoDFFs").c_str());
	AppendMenu(hMenu_Model, MF_SEPARATOR, 2052, _T(""));
	AppendMenu(hMenu_Model, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Model_Convert, CLocalizationManager::getInstance()->getTranslatedTextW("Convert").c_str());
	AppendMenu(hMenu_Model_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Model_Convert_DFFRWVersion, CLocalizationManager::getInstance()->getTranslatedTextW("DFFRWVersion").c_str());
	AppendMenu(hMenu_Model, MF_STRING, 2081, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Model_RemoveOrphanTextures").c_str());

	i = 0;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		if (pRWVersion->getVersionType() != TYPE_MODEL)
		{
			continue;
		}

		getKGM()->m_umapMenuItemMapping_ConvertDFFtoRWVersion[1590 + i] = pRWVersion;
		string strMenuText = "Convert DFF to " + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(0, 2)) + "&" + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(2));
		AppendMenu(hMenu_Model_Convert_DFFRWVersion, MF_STRING, 1590 + i, CStringUtility::convertStdStringToStdWString(strMenuText).c_str());
		i++;
	}

	//AppendMenu(hMenu_Model_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Model_Convert_FileFormat, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_FileFormat").c_str());
	//AppendMenu(hMenu_Model_Convert_FileFormat, MF_STRING, 1840, _T("Convert DFF to WDR"));

	AppendMenu(hMenu_Texture, MF_STRING, 1154, CLocalizationManager::getInstance()->getTranslatedTextW("Dump").c_str());
	AppendMenu(hMenu_Texture, MF_STRING, 2051, CLocalizationManager::getInstance()->getTranslatedTextW("BuildTXD").c_str());
	AppendMenu(hMenu_Texture, MF_STRING, 2055, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_TXDOrganizer").c_str());
	AppendMenu(hMenu_Texture, MF_SEPARATOR, 2080, _T(""));

	AppendMenu(hMenu_Texture, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Texture_Convert, CLocalizationManager::getInstance()->getTranslatedTextW("Convert").c_str());

	AppendMenu(hMenu_Texture_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Texture_Convert_TXDGame, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_TXD_Game").c_str());
	AppendMenu(hMenu_Texture_Convert_TXDGame, MF_STRING, 1650, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_III").c_str()).c_str());
	AppendMenu(hMenu_Texture_Convert_TXDGame, MF_STRING, 1651, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_VC").c_str()).c_str());
	AppendMenu(hMenu_Texture_Convert_TXDGame, MF_STRING, 1652, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_SA").c_str()).c_str());
	AppendMenu(hMenu_Texture_Convert_TXDGame, MF_STRING, 1653, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_Android_GTA").c_str()).c_str());
	AppendMenu(hMenu_Texture_Convert_TXDGame, MF_STRING, 1654, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_XBOX_GTA").c_str()).c_str());

	AppendMenu(hMenu_Texture_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Texture_Convert_TXDRWVersion, CLocalizationManager::getInstance()->getTranslatedTextW("TXDRWVersion").c_str());
	i = 0;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		if (pRWVersion->getVersionType() != TYPE_MODEL)
		{
			continue;
		}

		getKGM()->m_umapMenuItemMapping_ConvertTXDtoRWVersion[1660 + i] = pRWVersion;
		string strString1 = CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(0, 2)) + "&" + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(2));
		string strMenuText = CLocalizationManager::getInstance()->getTranslatedFormattedText("Menu_Convert_TXD_To", strString1.c_str());
		AppendMenu(hMenu_Texture_Convert_TXDRWVersion, MF_STRING, 1660 + i, CStringUtility::convertStdStringToStdWString(strMenuText).c_str());
		i++;
	}

	AppendMenu(hMenu_Texture_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Texture_Convert_TXDTextureFormat, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_TXD_RasterFormat").c_str());
	i = 0;
	for (auto pRasterDataFormat : CImageManager::getInstance()->getRasterDataFormatManager()->getEntries())
	{
		getKGM()->m_umapMenuItemMapping_ConvertTXDtoTextureFormat[2100 + i] = pRasterDataFormat;
		AppendMenu(hMenu_Texture_Convert_TXDTextureFormat, MF_STRING, 2100 + i, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", pRasterDataFormat->getTextForMenu().c_str()).c_str());
		i++;
	}

	//AppendMenu(hMenu_Texture_Convert, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Texture_Convert_WTDFileFormat, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_WTDFileFormat").c_str());
	//AppendMenu(hMenu_Texture_Convert_WTDFileFormat, MF_STRING, 2500, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_WTD_TXD").c_str());

	AppendMenu(hMenu_Texture, MF_STRING, 1150, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_TextureList").c_str());
	AppendMenu(hMenu_Texture, MF_STRING, 1620, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingTexture").c_str());

	AppendMenu(hMenu_Collision, MF_STRING, 1681, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_AlignMeshes_COL_DFF").c_str());
	AppendMenu(hMenu_Collision, MF_SEPARATOR, 1702, _T(""));

	AppendMenu(hMenu_Collision, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Collision_Convert, CLocalizationManager::getInstance()->getTranslatedTextW("Convert").c_str());

	i = 0;
	for (auto pCOLVersion : CCOLManager::getInstance()->getVersionManager()->getEntries())
	{
		getKGM()->m_umapMenuItemMapping_ConvertCOLtoCOLVersion[2400 + i] = pCOLVersion;
		AppendMenu(hMenu_Collision_Convert, MF_STRING, 2400 + i, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_COL_COL", pCOLVersion->getText().c_str()).c_str());
		i++;
	}

	AppendMenu(hMenu_Collision, MF_STRING, 1680, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_CenterCOLMeshes").c_str());

	AppendMenu(hMenu_ItemDefinition, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ItemDefinition_FindOrphanEntries, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_FindOrphanEntries").c_str());
	AppendMenu(hMenu_ItemDefinition_FindOrphanEntries, MF_STRING, 2091, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OrphanEntries_IDE_COL").c_str());
	AppendMenu(hMenu_ItemDefinition_FindOrphanEntries, MF_STRING, 2092, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OrphanEntries_IDE_DFF").c_str());
	AppendMenu(hMenu_ItemDefinition_FindOrphanEntries, MF_STRING, 2093, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OrphanEntries_IDE_IMG").c_str());
	AppendMenu(hMenu_ItemDefinition_FindOrphanEntries, MF_STRING, 2094, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OrphanEntries_IDE_IPL").c_str());
	AppendMenu(hMenu_ItemDefinition_FindOrphanEntries, MF_STRING, 2095, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_OrphanEntries_IDE_TXD").c_str());
	AppendMenu(hMenu_ItemDefinition, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ItemDefinition_FindMissing, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_FindMissingEntries").c_str());
	AppendMenu(hMenu_ItemDefinition_FindMissing, MF_STRING, 2059, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingEntries_IDE_IMG_TXD").c_str());
	AppendMenu(hMenu_ItemDefinition_FindMissing, MF_STRING, 2060, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingEntries_IDE_COL_COL").c_str());
	AppendMenu(hMenu_ItemDefinition_FindMissing, MF_STRING, 2061, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingEntries_IDE_IMG_DFF").c_str());
	AppendMenu(hMenu_ItemDefinition, MF_STRING, 2062, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_SortDATIDEAndIPL_ByObjectId").c_str());

	AppendMenu(hMenu_ItemPlacement, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ItemPlacement_FindMissing, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_FindMissingEntries").c_str());
	AppendMenu(hMenu_ItemPlacement_FindMissing, MF_STRING, 2097, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingEntries_IPL_IDE_DFF").c_str());

	AppendMenu(hMenu_ItemPlacement, MF_STRING, 2058, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MapMoverAndIDShifter").c_str());

	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Import, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import").c_str());
	AppendMenu(hMenu_Entry_Import, MF_STRING, 1160, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import_Files").c_str());
	AppendMenu(hMenu_Entry_Import, MF_STRING, 1161, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import_IDE").c_str());
	AppendMenu(hMenu_Entry_Import, MF_STRING, 1169, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import_TextLines").c_str());
	AppendMenu(hMenu_Entry_Import, MF_STRING, 1170, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Import_Folder").c_str());
	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Export, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1162, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_Selected").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1163, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_IDE").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1350, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_IPL").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1164, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_TextLines").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1192, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_DAT").c_str());
	AppendMenu(hMenu_Entry_Export, MF_SEPARATOR, 1710, _T(""));
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1550, CLocalizationManager::getInstance()->getTranslatedTextW("QuickExport").c_str());
	AppendMenu(hMenu_Entry_Export, MF_SEPARATOR, 1711, _T(""));
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1551, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_AllTabs").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1554, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_AllTabsIntoFolders").c_str());
	AppendMenu(hMenu_Entry_Export, MF_SEPARATOR, 1712, _T(""));
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1552, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_AllTabsIDE").c_str());
	AppendMenu(hMenu_Entry_Export, MF_STRING, 1553, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_AllTabsTextLines").c_str());
	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Remove, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove").c_str());
	AppendMenu(hMenu_Entry_Remove, MF_STRING, 1165, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove_Selected").c_str());
	AppendMenu(hMenu_Entry_Remove, MF_STRING, 1177, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove_IDE").c_str());
	AppendMenu(hMenu_Entry_Remove, MF_STRING, 1178, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove_TextLines").c_str());
	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Replace, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Replace").c_str());
	AppendMenu(hMenu_Entry_Replace, MF_STRING, 1176, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Replace_Selected").c_str());
	AppendMenu(hMenu_Entry_Replace, MF_STRING, 1621, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Replace_Folder").c_str());
	AppendMenu(hMenu_Entry, MF_SEPARATOR, 1700, _T(""));

	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Shift, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift").c_str());
	AppendMenu(hMenu_Entry_Shift, MF_STRING, 1540, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift_Up").c_str());
	AppendMenu(hMenu_Entry_Shift, MF_STRING, 1541, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift_Down").c_str());
	AppendMenu(hMenu_Entry, MF_STRING, 1166, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Rename").c_str());
	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Case, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase").c_str());
	AppendMenu(hMenu_Entry_Case, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_LowerCase, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Lowercase").c_str());
	AppendMenu(hMenu_Entry_LowerCase, MF_STRING, 1500, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_Entry_LowerCase, MF_STRING, 1501, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_Entry_LowerCase, MF_STRING, 1502, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu_Entry_Case, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_UpperCase, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Uppercase").c_str());
	AppendMenu(hMenu_Entry_UpperCase, MF_STRING, 1510, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_Entry_UpperCase, MF_STRING, 1511, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_Entry_UpperCase, MF_STRING, 1512, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu_Entry_Case, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_TitleCase, CLocalizationManager::getInstance()->getTranslatedTextW("Titlecase").c_str());
	AppendMenu(hMenu_Entry_TitleCase, MF_STRING, 1520, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_Entry_TitleCase, MF_STRING, 1521, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_Entry_TitleCase, MF_STRING, 1522, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu_Entry, MF_SEPARATOR, 1701, _T(""));
	AppendMenu(hMenu_Entry, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Entry_Copy, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1530, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_ID").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1531, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_Name").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1532, CLocalizationManager::getInstance()->getTranslatedTextW("Offset").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1533, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_ResourceType").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1534, CLocalizationManager::getInstance()->getTranslatedTextW("RWVersion").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1535, CLocalizationManager::getInstance()->getTranslatedTextW("Size").c_str());
	AppendMenu(hMenu_Entry_Copy, MF_STRING, 1536, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_Type").c_str());
	//AppendMenu(hMenu_Entry, MF_SEPARATOR, 1703, _T(""));

	AppendMenu(hMenu_Settings, MF_STRING, 1580, CLocalizationManager::getInstance()->getTranslatedTextW("Settings").c_str());

	AppendMenu(hMenu_Help, MF_STRING, 1570, CLocalizationManager::getInstance()->getTranslatedTextW("Version").c_str());
	AppendMenu(hMenu_Help, MF_STRING, 1572, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Update").c_str());
	AppendMenu(hMenu_Help, MF_SEPARATOR, 1574, _T(""));
	AppendMenu(hMenu_Help, MF_STRING, 1571, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Website").c_str());
	AppendMenu(hMenu_Help, MF_STRING, 1575, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_ReportIssueOrIdea").c_str());
	AppendMenu(hMenu_Help, MF_SEPARATOR, 1574, _T(""));
	AppendMenu(hMenu_Help, MF_STRING, 1573, CLocalizationManager::getInstance()->getTranslatedTextW("Credits").c_str());

	::SetMenu(getKGM()->getDialog()->GetSafeHwnd(), hMenubar);
	*/
}

void					CEntryListWindow::loadRightClickMenu(int xPos, int yPos)
{
	/*
	todo
	CListCtrl *pListCtrl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);

	RECT rect;
	pListCtrl->GetWindowRect(&rect);
	getKGM()->getDialog()->ScreenToClient(&rect);

	POINT point;
	point.x = xPos;
	point.y = yPos;
	ScreenToClient(getKGM()->getDialog()->GetSafeHwnd(), &point);
	if (point.x >= rect.left &&
		point.x <= rect.right &&
		point.y >= rect.top &&
		point.y <= rect.bottom)
	{
	}
	else
	{
		// cursor right click was not in the main list view
		return;
	}

	// create menu
	HMENU hMenu = CreatePopupMenu();

	// create popup menus
	HMENU hMenu_Shift = CreatePopupMenu();
	HMENU hMenu_NameCase = CreatePopupMenu();
	HMENU hMenu_NameCase_LowerCase = CreatePopupMenu();
	HMENU hMenu_NameCase_UpperCase = CreatePopupMenu();
	HMENU hMenu_NameCase_TitleCase = CreatePopupMenu();
	HMENU hMenu_Copy = CreatePopupMenu();

	HMENU hMenu_ConvertDFF = CreatePopupMenu();
	HMENU hMenu_ConvertDFF_DFFRWVersion = CreatePopupMenu();

	HMENU hMenu_ConvertTXD = CreatePopupMenu();
	HMENU hMenu_ConvertTXD_TXDGame = CreatePopupMenu();
	HMENU hMenu_ConvertTXD_TXDRWVersion = CreatePopupMenu();
	HMENU hMenu_ConvertTXD_TXDTextureFormat = CreatePopupMenu();
	HMENU hMenu_ConvertTXD_WTDFileFormat = CreatePopupMenu();

	HMENU hMenu_ConvertCOL = CreatePopupMenu();

	// IMG Entry
	uint32 i;

	AppendMenu(hMenu, MF_STRING, 1550, CLocalizationManager::getInstance()->getTranslatedTextW("QuickExport").c_str());
	AppendMenu(hMenu, MF_STRING, 1162, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Export_Selected").c_str());
	AppendMenu(hMenu, MF_STRING, 1165, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Remove_Selected").c_str());
	AppendMenu(hMenu, MF_STRING, 1176, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Replace_Selected").c_str());
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Shift, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift").c_str());
	AppendMenu(hMenu_Shift, MF_STRING, 1540, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift_Up").c_str());
	AppendMenu(hMenu_Shift, MF_STRING, 1541, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Shift_Down").c_str());
	AppendMenu(hMenu, MF_STRING, 1166, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Rename").c_str());
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_NameCase, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase").c_str());
	AppendMenu(hMenu_NameCase, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_NameCase_LowerCase, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Lowercase").c_str());
	AppendMenu(hMenu_NameCase_LowerCase, MF_STRING, 1500, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_NameCase_LowerCase, MF_STRING, 1501, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_NameCase_LowerCase, MF_STRING, 1502, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu_NameCase, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_NameCase_UpperCase, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Uppercase").c_str());
	AppendMenu(hMenu_NameCase_UpperCase, MF_STRING, 1510, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_NameCase_UpperCase, MF_STRING, 1511, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_NameCase_UpperCase, MF_STRING, 1512, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu_NameCase, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_NameCase_TitleCase, CLocalizationManager::getInstance()->getTranslatedTextW("Titlecase").c_str());
	AppendMenu(hMenu_NameCase_TitleCase, MF_STRING, 1520, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithExtension").c_str());
	AppendMenu(hMenu_NameCase_TitleCase, MF_STRING, 1521, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_NameWithoutExtension").c_str());
	AppendMenu(hMenu_NameCase_TitleCase, MF_STRING, 1522, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_NameCase_Extension").c_str());
	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_Copy, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1530, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_ID").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1531, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_Name").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1532, CLocalizationManager::getInstance()->getTranslatedTextW("Offset").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1533, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_ResourceType").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1534, CLocalizationManager::getInstance()->getTranslatedTextW("RWVersion").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1535, CLocalizationManager::getInstance()->getTranslatedTextW("Size").c_str());
	AppendMenu(hMenu_Copy, MF_STRING, 1536, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Copy_Type").c_str());

	// DFF Entry
	AppendMenu(hMenu, MF_SEPARATOR, 1701, _T(""));

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertDFF, CLocalizationManager::getInstance()->getTranslatedTextW("ConvertModel").c_str());
	AppendMenu(hMenu_ConvertDFF, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertDFF_DFFRWVersion, CLocalizationManager::getInstance()->getTranslatedTextW("DFFRWVersion").c_str());
	AppendMenu(hMenu, MF_STRING, 2081, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Model_RemoveOrphanTextures").c_str());

	i = 0;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		if (pRWVersion->getVersionType() != TYPE_MODEL)
		{
			continue;
		}

		string strMenuText = "Convert DFF to " + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(0, 2)) + "&" + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(2));
		AppendMenu(hMenu_ConvertDFF_DFFRWVersion, MF_STRING, 1590 + i, CStringUtility::convertStdStringToStdWString(strMenuText).c_str());
		i++;
	}

	// TXD Entry
	AppendMenu(hMenu, MF_SEPARATOR, 1701, _T(""));

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertTXD, CLocalizationManager::getInstance()->getTranslatedTextW("ConvertTexture").c_str());

	AppendMenu(hMenu_ConvertTXD, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertTXD_TXDGame, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_TXD_Game").c_str());
	AppendMenu(hMenu_ConvertTXD_TXDGame, MF_STRING, 1650, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_III").c_str()).c_str());
	AppendMenu(hMenu_ConvertTXD_TXDGame, MF_STRING, 1651, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_VC").c_str()).c_str());
	AppendMenu(hMenu_ConvertTXD_TXDGame, MF_STRING, 1652, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_PC_GTA_SA").c_str()).c_str());
	AppendMenu(hMenu_ConvertTXD_TXDGame, MF_STRING, 1653, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_Android_GTA").c_str()).c_str());
	AppendMenu(hMenu_ConvertTXD_TXDGame, MF_STRING, 1654, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", CLocalizationManager::getInstance()->getTranslatedFormattedText("PlatformedGame_XBOX_GTA").c_str()).c_str());

	AppendMenu(hMenu_ConvertTXD, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertTXD_TXDRWVersion, CLocalizationManager::getInstance()->getTranslatedTextW("TXDRWVersion").c_str());
	i = 0;
	for (auto pRWVersion : CRWManager::getInstance()->getVersionManager()->getEntries())
	{
		if (pRWVersion->getVersionType() != TYPE_MODEL)
		{
			continue;
		}

		string strString1 = CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(0, 2)) + "&" + CStringUtility::escapeMenuText((pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").substr(2));
		string strMenuText = CLocalizationManager::getInstance()->getTranslatedFormattedText("Menu_Convert_TXD_To", strString1.c_str());
		AppendMenu(hMenu_ConvertTXD_TXDRWVersion, MF_STRING, 1660 + i, CStringUtility::convertStdStringToStdWString(strMenuText).c_str());
		i++;
	}

	AppendMenu(hMenu_ConvertTXD, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertTXD_TXDTextureFormat, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_TXD_RasterFormat").c_str());
	i = 0;
	for (auto pRasterDataFormat : CImageManager::getInstance()->getRasterDataFormatManager()->getEntries())
	{
		AppendMenu(hMenu_ConvertTXD_TXDTextureFormat, MF_STRING, 2100 + i, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_TXD_To", pRasterDataFormat->getTextForMenu().c_str()).c_str());
		i++;
	}

	//AppendMenu(hMenu_ConvertTXD, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertTXD_WTDFileFormat, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_WTDFileFormat").c_str());
	//AppendMenu(hMenu_ConvertTXD_WTDFileFormat, MF_STRING, 2500, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Convert_WTD_TXD").c_str());

	AppendMenu(hMenu, MF_STRING, 1150, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_TextureList").c_str());
	AppendMenu(hMenu, MF_STRING, 1620, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_MissingTexture").c_str());

	// COL Entry
	AppendMenu(hMenu, MF_SEPARATOR, 1701, _T(""));

	AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu_ConvertCOL, CLocalizationManager::getInstance()->getTranslatedTextW("ConvertCollision").c_str());

	i = 0;
	for (auto pCOLVersion : CCOLManager::getInstance()->getVersionManager()->getEntries())
	{
		AppendMenu(hMenu_ConvertCOL, MF_STRING, 2400 + i, CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Menu_Convert_COL_COL", pCOLVersion->getText().c_str()).c_str());
		i++;
	}

	AppendMenu(hMenu, MF_STRING, 1680, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_CenterCOLMeshes").c_str());

	// show menu
	SetForegroundWindow(getKGM()->getDialog()->GetSafeHwnd());
	TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, xPos, yPos, 0, getKGM()->getDialog()->GetSafeHwnd(), NULL);
	PostMessage(getKGM()->getDialog()->GetSafeHwnd(), WM_NULL, 0, 0);

	// clean up
	//DestroyMenu(hMenu);
	*/
}