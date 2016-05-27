#pragma warning(disable : 4005)

#include "CTaskDispatchManager.h"
#include "CTaskManager.h"
#include "CKGM.h"
#include "Globals.h"
#include "GUI/CGUIUtility.h"
#include "String/CStringUtility.h"
#include "Vector/CVectorUtility.h"
#include "Path/CPathUtility.h"
#include "File/CFileUtility.h"
#include "Registry/CRegistryUtility.h"
#include "Window/Window Types/CEntryListWindow.h"
#include "Window/Window Types/Tabs/CEntryListWindowTab.h"
#include "IMG/CIMGManager.h"
#include "IMG/CIMGFormat.h"
#include "IMG/CIMGEntry.h"
#include "IDE/CIDEManager.h"
#include "IPL/CIPLManager.h"
#include "IDE/CIDEFormat.h"
#include "COL/CCOLManager.h"
#include "COL/CCOLFormat.h"
#include "Drag Drop/CDropTarget.h"
#include "TXD/CTXDManager.h"
#include "TXD/CTXDFormat.h"
#include "RW/Sections/CRWSection_TextureNative.h"
#include "DFF/CDFFManager.h"
#include "DFF/CDFFFormat.h"
#include "RW/CTextureEntry.h"
#include "DAT/Loader/CDATLoaderManager.h"
#include "DAT/Path/CDATPathManager.h" // todo - needed?
#include "DAT/Loader/CDATLoaderFormat.h"
#include "DAT/Loader/CDATLoaderEntry.h"
#include "BMP/CBMPManager.h"
#include "BMP/CBMPFormat.h"
#include "Engine/RW/CRWVersion.h"
#include "LST/CLSTManager.h"
#include "LST/CLSTFormat.h"
#include "LST/CLSTSection.h"
#include "LST/CLSTEntry.h"
#include "COL/CCOLEntry.h"
#include "Testing/CDebugger.h"
#include "IPL/CIPLFormat.h"
#include "Engine/RW/CRWManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Session/CSessionManager.h"
#include "CGUIManager.h"
#include "CLSTProcessingManager.h"
#include "CDumpManager.h"
#include "CSettingsManager.h"
#include "Sort/CSortManager.h"
#include "Sort/CSortPriority.h"
#include "CHTTPManager.h"
#include "DB/CDBManager.h"
#include "DB/CDBFormat.h"
#include "Image/CRasterDataFormat.h"
#include "CSearchEntry.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Entry Viewer/CEntryViewerManager.h"
#include "IDE/CIDEEntry.h"
#include "IDE/Entry/Data Entry/CIDEEntry_OBJS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TOBJ.h"
#include "IDE/Entry/Data Entry/CIDEEntry_ANIM.h"
#include "IDE/Entry/Data Entry/CIDEEntry_CARS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_HAND.h"
#include "IDE/Entry/Data Entry/CIDEEntry_HIER.h"
#include "IDE/Entry/Data Entry/CIDEEntry_PATH.h"
#include "IDE/Entry/Data Entry/CIDEEntry_PEDS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TXDP.h"
#include "IDE/Entry/Data Entry/CIDEEntry_WEAP.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Light.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Particle.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Ped.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_SunReflection.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Unknown1.h"
#include "IPL/CIPLEntry.h"
#include "IPL/Entry/Data Entry/CIPLEntry_INST.h"
#include "IPL/Entry/Data Entry/CIPLEntry_AUZO.h"
#include "IPL/Entry/Data Entry/CIPLEntry_CARS.h"
#include "IPL/Entry/Data Entry/CIPLEntry_CULL.h"
#include "IPL/Entry/Data Entry/CIPLEntry_ENEX.h"
#include "IPL/Entry/Data Entry/CIPLEntry_GRGE.h"
#include "IPL/Entry/Data Entry/CIPLEntry_JUMP.h"
#include "IPL/Entry/Data Entry/CIPLEntry_MULT.h"
#include "IPL/Entry/Data Entry/CIPLEntry_MZON.h"
#include "IPL/Entry/Data Entry/CIPLEntry_OCCL.h"
#include "IPL/Entry/Data Entry/CIPLEntry_PATH.h"
#include "IPL/Entry/Data Entry/CIPLEntry_PICK.h"
#include "IPL/Entry/Data Entry/CIPLEntry_TCYC.h"
#include "IPL/Entry/Data Entry/CIPLEntry_ZONE.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_CoverPoint.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_EnterExit.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_Escalator.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_Light.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_ParticleEffect.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_PedAttractor.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_SlotmachineWheel.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_StreetSign.h"
#include "RW/Entries/2d Effects/CRWEntry_2dEffect_SunGlare.h"
#include "Update/CUpdateManager.h"
#include "Update/CUpdateConnectionManager.h"
#include "Update/CUpdateConnection.h"
#include "buildnumber.h"
#include "COL/CCOLVersionManager.h"
#include "COL/CCOLVersion.h"
#include "Math/CMathUtility.h"
#include "RW/Sections/CRWSection_Geometry.h"
#include "RW/Sections/CRWSection_String.h"
#include "RW/Sections/CRWSection_Texture.h"
#include "RW/Sections/CRWSection_Material.h"
#include "RW/Sections/CRWSection_2dEffect.h"
#include "WDR/CWDRManager.h"
#include "WDR/CWDRFormat.h"
#include "WTD/CWTDManager.h"
#include "WTD/CWTDFormat.h"
#include "Texture/CIntermediateTextureFormat.h"
#include "DAT/Path/CDATPathFormat.h"
#include "Localization/CLocalizationManager.h"
#include "Sort/CSortPriorities.h"
#include "Entry Viewer/CTextureViewer.h"
#include "Entry Viewer/CCollisionViewer.h"
#include "CRenamedIMGEntry.h"
#include "RW/CTextureEntry.h"
#include "Game/eGame.h"
#include "CLastUsedValueManager.h"
#include <gdiplus.h>
#include <stdio.h>
#include <algorithm>

using namespace std;

void		CTaskDispatchManager::init(void)
{
}
void		CTaskDispatchManager::uninit(void)
{
}

void		CTaskDispatchManager::onRequestOpen(void)
{
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("OPEN"), "IMG,DIR");
	if (vecPaths.size() == 0)
	{
		return;
	}
	getKGM()->setLastUsedDirectory("OPEN", CPathUtility::getDirectory(vecPaths[0]));

	for (auto strPath : vecPaths)
	{
		onRequestOpen2(strPath);
	}
}
void		CTaskDispatchManager::onRequestOpen2(string strPath)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Open2_IMGPath(strPath);
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpen2");
	eIMGVersion eIMGVersionValue = CIMGManager::detectIMGVersion(strPath);
	/*
	todo
	if (uiFileResult == FILE_NOT_FOUND)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_32"), CLocalizationManager::getInstance()->getTranslatedText("FileNotFound"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestOpen2", true);
		return;
	}
	else if (uiFileResult == FILE_BLANK)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_33"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_33"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestOpen2", true);
		return;
	}
	else if (uiFileResult == FILE_UNKNOWN2)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_34"), CLocalizationManager::getInstance()->getTranslatedText("UnableToOpenIMG"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestOpen2", true);
		return;
	}
	*/
	
	if (eIMGVersionValue == IMG_1)
	{
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strPath)) == "IMG")
		{
			strPath = CPathUtility::replaceFileExtension(strPath, "DIR"); // as the user may have opened an IMG file
		}
	}
	else
	{
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strPath)) == "DIR")
		{
			strPath = CPathUtility::replaceFileExtension(strPath, "IMG"); // as the user may have opened a DIR file
		}
	}

	getKGM()->getEntryListWindow()->addTab(strPath, eIMGVersionValue);
	getKGM()->getTaskManager()->onTaskEnd("onRequestOpen2");
}
void		CTaskDispatchManager::onRequestClose(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestClose");
	CEntryListWindowTab *pWindowTab = getKGM()->getEntryListTab();
	if (!pWindowTab)
	{
		return;
	}

	if (getKGM()->getSettingsManager()->getSettingBool("RebuildConfirmationOnClose"))
	{
		getKGM()->getTaskManager()->onTaskPause();
		if (onRequestClose2(false) == true)
		{
			getKGM()->getTaskManager()->onTaskUnpause();
			getKGM()->getTaskManager()->onTaskEnd("onRequestClose", true);
			return;
		}
		getKGM()->getTaskManager()->onTaskUnpause();
	}

	getKGM()->getEntryListWindow()->removeTab(pWindowTab);

	getKGM()->getTaskManager()->onTaskEnd("onRequestClose");
}
void		CTaskDispatchManager::onRequestCloseAll(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestCloseAll");
	if (getKGM()->getSettingsManager()->getSettingBool("RebuildConfirmationOnClose"))
	{
		getKGM()->getTaskManager()->onTaskPause();
		if (onRequestClose2(true) == true)
		{
			getKGM()->getTaskManager()->onTaskUnpause();
			getKGM()->getTaskManager()->onTaskEnd("onRequestCloseAll", true);
			return;
		}
		getKGM()->getTaskManager()->onTaskUnpause();
	}

	while (getKGM()->getEntryListWindow()->getEntryCount() > 0) // todo - change to removeAllEntries or something assuming all stuff in removeTab() still gets ran somewhere
	{
		getKGM()->getEntryListWindow()->removeTab(getKGM()->getEntryListWindow()->getEntryByIndex(0));
	}

	getKGM()->getEntryListWindow()->setActiveTab(nullptr);
	getKGM()->getTaskManager()->onTaskEnd("onRequestCloseAll");
}
void		CTaskDispatchManager::onRequestExitTool(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExitTool");
	DestroyWindow(getKGM()->getActiveWindow()->getWindowHandle());
	getKGM()->getTaskManager()->onTaskEnd("onRequestExitTool");
}
void		CTaskDispatchManager::onRequestImportViaFiles(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestImportViaFiles");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFiles", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("IMPORT_FILES"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFiles", true);
		return;
	}
	getKGM()->setLastUsedDirectory("IMPORT_FILES", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecPaths.size());
	for (auto strPath : vecPaths)
	{
		getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strPath);
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_23", vecPaths.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntryNames"), true);
	vector<string> vecFileNames;
	for (auto strPath : vecPaths)
	{
		vecFileNames.push_back(strPath);
	}
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecFileNames, "\n"), true);

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFiles");
}
void		CTaskDispatchManager::onRequestRemoveSelected(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestRemoveSelected");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		return;
	}

	CListCtrl *pListControl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	vector<string> vecEntryNames;
	uint32 uiRemoveCount = 0;
	CIMGEntry *pIMGEntry;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);

		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		vecEntryNames.push_back(pIMGEntry->getEntryName());
		getKGM()->getEntryListTab()->removeEntry(pIMGEntry);
		pListControl->DeleteItem(nItem);

		pos = pListControl->GetFirstSelectedItemPosition();

		uiRemoveCount++;
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	if (pListControl->GetItemCount() == 0)
	{
		getKGM()->getEntryListTab()->readdAllEntriesToMainListView();
	}

	getKGM()->getEntryListTab()->searchText();

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_53", uiRemoveCount));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntryNames"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNames, "\n"), true);

	if (uiRemoveCount > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveSelected");
	*/
}
void		CTaskDispatchManager::onRequestRenameEntry(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestRenameEntry");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenameEntry", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenameEntry", true);
		return;
	}

	int nItem;
	CIMGEntry *pIMGEntry = nullptr;
	nItem = pListControl->GetNextSelectedItem(pos);
	pIMGEntry = ((CIMGEntry*)pListControl->GetItemData(nItem));

	pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenameEntry", true);
		return;
	}

	bool bMultipleEntries = pListControl->GetSelectedCount() > 1;
	string strOldName = pIMGEntry->getEntryName();
	getKGM()->getTaskManager()->onTaskPause();
	string strNewName = getKGM()->getGUIManager()->showTextInputDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_1_Title"), CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_TextInput_1_Message", pListControl->GetSelectedCount()), pIMGEntry->getEntryName());
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strNewName == "")
	{
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = ((CIMGEntry*)pListControl->GetItemData(nItem));

		pIMGEntry->setEntryName(strNewName);
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getEntryListTab()->onEntryChange(pIMGEntry);
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_54", strOldName.c_str(), strNewName.c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_55", strNewName.c_str(), getKGM()->getEntryListTab()->getIMGFile()->getEntryCountForName(strNewName)), true);

	getKGM()->getEntryListTab()->searchText();

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestRenameEntry");
	*/
}
void		CTaskDispatchManager::onRequestSelectAll(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSelectAll");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectAll", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());
	bool bNewSelectedState = true;
	if (pListControl->GetItemState(0, LVIS_SELECTED) == LVIS_SELECTED)
	{
		bNewSelectedState = false;
	}
	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		if (bNewSelectedState)
		{
			pListControl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			pListControl->SetSelectionMark(i);
		}
		else
		{
			pListControl->SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	pListControl->SetFocus();
	if(bNewSelectedState)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_56", pListControl->GetItemCount()));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_57", pListControl->GetItemCount()));
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestSelectAll");
	*/
}
void		CTaskDispatchManager::onRequestSelectInverse(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSelectInverse");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectInverse", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());
	uint32 uiSelectedEntryCount = 0;
	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		if (pListControl->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			pListControl->SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
		}
		else
		{
			uiSelectedEntryCount++;
			pListControl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			pListControl->SetSelectionMark(i);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	pListControl->SetFocus();
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_58", uiSelectedEntryCount, pListControl->GetItemCount()));
	getKGM()->getTaskManager()->onTaskEnd("onRequestSelectInverse");
	*/
}
void		CTaskDispatchManager::onRequestRebuild(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRebuild");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRebuild", true);
		return;
	}

	getKGM()->getEntryListTab()->rebuild();
	getKGM()->getEntryListWindow()->refreshActiveTab();

	getKGM()->getEntryListTab()->checkForUnknownRWVersionEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestRebuild");
}
void		CTaskDispatchManager::onRequestRebuildAs(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRebuildAs");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRebuildAs", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strIMGPath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("REBUILD_AS"), "IMG,DIR", "IMG.img");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strIMGPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRebuildAs", true);
		return;
	}
	getKGM()->setLastUsedDirectory("REBUILD_AS", strIMGPath);

	getKGM()->getEntryListTab()->rebuild(strIMGPath, false);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_59", CPathUtility::getFileName(strIMGPath).c_str()));
	getKGM()->getEntryListWindow()->refreshActiveTab();

	getKGM()->getEntryListTab()->checkForUnknownRWVersionEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestRebuildAs");
}
void		CTaskDispatchManager::onRequestRebuildAll(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRebuildAll");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRebuildAll", true);
		return;
	}

	vector<string> vecIMGPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecIMGPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());
		((CEntryListWindowTab*)pWindowTab)->rebuild("", false);
	}
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_1", getKGM()->getEntryListWindow()->getEntryCount()));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_2"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecIMGPaths, "\n"), true);

	getKGM()->getEntryListWindow()->refreshActiveTab();

	getKGM()->getEntryListTab()->checkForUnknownRWVersionEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestRebuildAll");
}
void		CTaskDispatchManager::onRequestConvertIMGVersion(eIMGVersion eIMGVersionValue)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Convert_IMGVersion(eIMGVersionValue);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertIMGVersion");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertIMGVersion", true);
		return;
	}

	// fetch data
	uint32 uiIMGEntryCount = getKGM()->getEntryListTab()->getIMGFile()->getEntryCount();
	uint32 ePreviousIMGVersion = getKGM()->getEntryListTab()->getIMGFile()->getIMGVersion();
	bool bPreviouslyEncrypted = getKGM()->getEntryListTab()->getIMGFile()->isEncrypted();

	// also convert entries to appropriate game version?
	bool bConvertEntries = false;
	if (ePreviousIMGVersion != IMG_FASTMAN92 && eIMGVersionValue != IMG_FASTMAN92)
	{
		bConvertEntries = getKGM()->getGUIManager()->showConfirmDialog("Also convert entries to appropriate game version?", "Convert Entries?");
	}

	// calculate progress bar tick count
	uint32 uiProgressMaxTicks = uiIMGEntryCount * 2;
	if (ePreviousIMGVersion == IMG_1)
	{
		uiProgressMaxTicks += 1;
	}
	if (getKGM()->getSettingsManager()->getSettingBool("RebuildOnConvert"))
	{
		uiProgressMaxTicks += uiIMGEntryCount;
	}
	if (bConvertEntries)
	{
		uiProgressMaxTicks += uiIMGEntryCount;
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiProgressMaxTicks);

	/*
	// remove compression from entries body data in version Fastman92 IMG
	if (ePreviousIMGVersion == IMG_FASTMAN92)
	{
		for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
		{
			if (pIMGEntry->isCompressed())
			{
				string strEntryNewData;
				switch(pIMGEntry->getCompressionAlgorithmId())
				{
					case IMGCOMPRESSION_ZLIB:
						strEntryNewData = CIMGManager::decompressZLib(pIMGEntry->getEntryData(), pIMGEntry->getFUncompressedSize());
					break;
					case IMGCOMPRESSION_LZ4:
						strEntryNewData = CIMGManager::decompressLZ4(pIMGEntry->getEntryData(), pIMGEntry->getFUncompressedSize());
					break;
				}
				pIMGEntry->setEntryData(strEntryNewData);
				getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
			}
		}
	}
	
	// add compression to entries body data in version Fastman92 IMG
	if (eIMGVersionValue == IMG_FASTMAN92)
	{
		for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
		{
			if (pIMGEntry->isCompressed())
			{
				string strEntryNewData;
				switch(pIMGEntry->getCompressionAlgorithmId())
				{
					case IMGCOMPRESSION_ZLIB:
						strEntryNewData = CIMGManager::compressZLib(pIMGEntry->getEntryData());
					break;
					case IMGCOMPRESSION_LZ4:
						strEntryNewData = CIMGManager::compressLZ4(pIMGEntry->getEntryData());
					break;
				}
				pIMGEntry->setEntryData(strEntryNewData);
				getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
			}
		}
	}
	*/

	// re-apply compression type to entry data
	if (ePreviousIMGVersion == IMG_FASTMAN92 && eIMGVersionValue != IMG_FASTMAN92)
	{
		for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
		{
			pIMGEntry->applyCompression(IMGCOMPRESSION_NONE);
		}
	}

	// fetch RW versions or resource types
	if (eIMGVersionValue == IMG_1 || eIMGVersionValue == IMG_2)
	{
		getKGM()->getEntryListTab()->getIMGFile()->unserializeRWVersions(); // todo - func name inconsitent with one below - RW / Rage
	}
	else
	{
		getKGM()->getEntryListTab()->getIMGFile()->unserializeResourceTypes();
	}

	// set new IMG version
	getKGM()->getEntryListTab()->getIMGFile()->setIMGVersion(eIMGVersionValue);

	// remove dir file?
	if (ePreviousIMGVersion == IMG_1)
	{
		CFileUtility::removeFile(CPathUtility::replaceFileExtension(getKGM()->getEntryListTab()->getIMGFile()->getFilePath(), "dir"));
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// check to convert entries aswell as IMG
	if (bConvertEntries)
	{
		// choose destination version for DFF, TXD and COL files
		ePlatformedGame eDestGame = PLATFORMED_GAME_UNKNOWN;
		if (eIMGVersionValue == IMG_1)
		{
			eDestGame = PLATFORMED_GAME_PC_GTA_VC;
		}
		else if (eIMGVersionValue == IMG_2)
		{
			eDestGame = PLATFORMED_GAME_PC_GTA_SA;
		}
		CRWVersion *pDestRWVersion = eDestGame == PLATFORMED_GAME_UNKNOWN ? nullptr : CRWManager::getInstance()->getVersionManager()->getRWVersionFromGame(eDestGame);

		eCOLVersion eDestCOLVersion = COL_UNKNOWN;
		if (eIMGVersionValue == IMG_1)
		{
			eDestCOLVersion = COL_2;
		}
		else if (eIMGVersionValue == IMG_2)
		{
			eDestCOLVersion = COL_3;
		}

		// convert entries to appropriate version
		for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
		{
			string strEntryExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			if (CPathUtility::isModelExtension(strEntryExtensionUpper))
			{
				if (eDestGame == PLATFORMED_GAME_UNKNOWN)
				{
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
				if (pDFFFile->doesHaveError())
				{
					pDFFFile->unload();
					delete pDFFFile;
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				pDFFFile->setRWVersion(pDestRWVersion);

				pIMGEntry->setEntryData(pDFFFile->storeViaMemory());
				getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
				
				pDFFFile->unload();
				delete pDFFFile;
			}
			else if (strEntryExtensionUpper == "TXD")
			{
				if (eDestGame == PLATFORMED_GAME_UNKNOWN)
				{
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
				if (pTXDFile->doesHaveError())
				{
					pTXDFile->unload();
					delete pTXDFile;
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				vector<string> vecMipmapsRemoved;
				pTXDFile->convertToGame(eDestGame, vecMipmapsRemoved);

				pIMGEntry->setEntryData(pTXDFile->serializeViaMemory());
				getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
				
				pTXDFile->unload();
				delete pTXDFile;
			}
			else if (strEntryExtensionUpper == "COL")
			{
				if (eDestCOLVersion == COL_UNKNOWN)
				{
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
				if (pCOLFile->doesHaveError())
				{
					pCOLFile->unload();
					delete pCOLFile;
					getKGM()->getTaskManager()->onTaskProgressTick();
					continue;
				}

				for (auto pCOLEntry : pCOLFile->getEntries())
				{
					pCOLEntry->setCOLVersion(CCOLManager::getInstance()->getVersionManager()->getEntryByVersionId(eDestCOLVersion));
				}

				pIMGEntry->setEntryData(pCOLFile->storeViaMemory());
				getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
				
				pCOLFile->unload();
				delete pCOLFile;
			}

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	// log
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_60", CIMGManager::getIMGVersionNameWithGames((eIMGVersion)ePreviousIMGVersion, bPreviouslyEncrypted).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, false).c_str()).c_str());

	// rebuild
	if (getKGM()->getSettingsManager()->getSettingBool("RebuildOnConvert"))
	{
		getKGM()->getEntryListTab()->rebuild();
	}

	// render
	getKGM()->getEntryListWindow()->refreshActiveTab();

	// other
	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	getKGM()->getEntryListTab()->checkForUnknownRWVersionEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertIMGVersion");
}
void		CTaskDispatchManager::onRequestConvertIMGVersionViaButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertIMGVersionViaButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertIMGVersionViaButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showConvertDialog("Convert IMG Version", CLocalizationManager::getInstance()->getTranslatedText("Convert"));
	getKGM()->getTaskManager()->onTaskUnpause(); 
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		return;
	}

	onRequestConvertIMGVersion((eIMGVersion)uiRadioButtonIndex);
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertIMGVersionViaButton");
}
void		CTaskDispatchManager::onRequestMerge(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestMerge");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMerge", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MERGE"), "IMG");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMerge", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MERGE", CPathUtility::getDirectory(vecPaths[0]));

	uint32 uiMergeEntryCount = 0;
	for (auto strPath : vecPaths)
	{
		uiMergeEntryCount += CIMGManager::getIMGEntryCount(strPath, CIMGManager::detectIMGVersion(strPath));
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiMergeEntryCount);

	uint32 uiImportedEntryCount = 0;
	vector<string> vecImportedEntryNames;
	string strExtendedLog = "";
	for (auto strPath : vecPaths)
	{
		uiImportedEntryCount += getKGM()->getEntryListTab()->merge(strPath, vecImportedEntryNames);

		strExtendedLog += "[" + CPathUtility::getFileName(strPath) + "]\n";
		strExtendedLog += CStringUtility::join(vecImportedEntryNames, "\n");

		vecImportedEntryNames.clear();
	}

	if (vecPaths.size() == 1)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_61", CPathUtility::getFileName(vecPaths[0]).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str(), uiImportedEntryCount));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_62", vecPaths.size(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str(), uiImportedEntryCount));
	}
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_63"), true);
	getKGM()->getEntryListTab()->log(strExtendedLog, true);

	getKGM()->getEntryListWindow()->refreshActiveTab();

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestMerge");
}
void		CTaskDispatchManager::onRequestSplitViaButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSplitViaButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showSplitViaDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaButton", true);
		return;
	}

	switch (uiRadioButtonIndex)
	{
	case 0:
		onRequestSplitSelectedEntries();
		break;
	case 1:
		onRequestSplitViaIDEFile();
		break;
	case 2:
		onRequestSplitViaTextLines();
		break;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaButton", true);
}
void		CTaskDispatchManager::onRequestSplitSelectedEntries(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSplitSelectedEntries");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitSelectedEntries", true);
		return;
	}

	if (getKGM()->getEntryListWindow()->getSelectedEntryCount() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitSelectedEntries", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("SPLIT_SELECTED"), "IMG,DIR", "IMG.img");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitSelectedEntries", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SPLIT_SELECTED", strPath);

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showConvertDialog("New IMG Version", CLocalizationManager::getInstance()->getTranslatedText("Save"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitSelectedEntries", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	bool bDeleteFromSource = getKGM()->getGUIManager()->showConfirmDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Message"), CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Title"));
	getKGM()->getTaskManager()->onTaskUnpause();

	eIMGVersion eIMGVersionValue = IMG_UNKNOWN;
	bool bIsEncrypted = false;
	switch (uiRadioButtonIndex)
	{
	case 0:
		eIMGVersionValue = IMG_1;
		break;
	case 1:
		eIMGVersionValue = IMG_2;
		break;
	case 2:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = true;
		break;
	case 3:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = false;
		break;
	}

	vector<string> vecSplitEntryNames;
	getKGM()->getEntryListTab()->splitSelectedEntries(strPath, eIMGVersionValue, bDeleteFromSource, vecSplitEntryNames);
	if(bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_66", getKGM()->getEntryListWindow()->getSelectedEntryCount(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_65", getKGM()->getEntryListWindow()->getSelectedEntryCount(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_67"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecSplitEntryNames, "\n"), true);

	if (bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}

	onRequestOpen2(strPath);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSplitSelectedEntries");
}
void		CTaskDispatchManager::onRequestSplitViaIDEFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSplitViaIDEFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaIDEFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("SPLIT_IDE"), "IMG,DIR", "IMG.img");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaIDEFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SPLIT_IDE", strPath);

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showConvertDialog("New IMG Version", CLocalizationManager::getInstance()->getTranslatedText("Save"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaIDEFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("SPLIT_IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaIDEFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SPLIT_IDE", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	bool bDeleteFromSource = getKGM()->getGUIManager()->showConfirmDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Message"), CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Title"));
	getKGM()->getTaskManager()->onTaskUnpause();
	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecSplitEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNamesWithoutExtension.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
			vecSplitEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}

	eIMGVersion eIMGVersionValue = IMG_UNKNOWN;
	bool bIsEncrypted = false;
	switch (uiRadioButtonIndex)
	{
	case 0:
		eIMGVersionValue = IMG_1;
		break;
	case 1:
		eIMGVersionValue = IMG_2;
		break;
	case 2:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = true;
		break;
	case 3:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = false;
		break;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size() * (bDeleteFromSource ? 2 : 1));
	getKGM()->getEntryListTab()->getIMGFile()->split(vecIMGEntries, strPath, eIMGVersionValue);
	
	if(bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_70", vecSplitEntryNames.size(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_69", vecSplitEntryNames.size(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_67"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecSplitEntryNames, "\n"), true);

	if (bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
		for (auto pIMGEntry : vecIMGEntries)
		{
			getKGM()->getEntryListTab()->removeEntry(pIMGEntry);

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	onRequestOpen2(strPath);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaIDEFile");
}
void		CTaskDispatchManager::onRequestSplitViaTextLines(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSplitViaTextLines");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("SPLIT_TEXTLINES"), "IMG,DIR", "IMG.img");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaTextLines", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SPLIT_TEXTLINES", strPath);

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showConvertDialog("New IMG Version", CLocalizationManager::getInstance()->getTranslatedText("Save"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	bool bDeleteFromSource = getKGM()->getGUIManager()->showConfirmDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Message"), CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_1_Title"));
	getKGM()->getTaskManager()->onTaskUnpause();

	vector<string> vecEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		vecEntryNames.push_back(strLine);
	}
	vecEntryNames = CVectorUtility::toUpperCase(vecEntryNames);

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecSplitEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNames.begin(), vecEntryNames.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNames.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
			vecSplitEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}

	eIMGVersion eIMGVersionValue = IMG_UNKNOWN;
	bool bIsEncrypted = false;
	switch (uiRadioButtonIndex)
	{
	case 0:
		eIMGVersionValue = IMG_1;
		break;
	case 1:
		eIMGVersionValue = IMG_2;
		break;
	case 2:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = true;
		break;
	case 3:
		eIMGVersionValue = IMG_3;
		bIsEncrypted = false;
		break;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size() * (bDeleteFromSource ? 2 : 1));
	getKGM()->getEntryListTab()->getIMGFile()->split(vecIMGEntries, strPath, eIMGVersionValue);

	if(bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_73", vecSplitEntryNames.size(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_72", vecSplitEntryNames.size(), CPathUtility::getFileName(strPath).c_str(), CIMGManager::getIMGVersionNameWithGames(eIMGVersionValue, bIsEncrypted).c_str(), CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str()));
	}
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_67"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecSplitEntryNames, "\n"), true);

	if (bDeleteFromSource)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
		for (auto pIMGEntry : vecIMGEntries)
		{
			getKGM()->getEntryListTab()->removeEntry(pIMGEntry);

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	onRequestOpen2(strPath);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSplitViaTextLines");
}
void		CTaskDispatchManager::onRequestReplace(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestReplace");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestReplace", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	string strEntryName;
	if (pos != NULL)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		CIMGEntry *pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		strEntryName = pIMGEntry->getEntryName();
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("REPLACE"), "", "", strEntryName);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestReplace", true);
		return;
	}
	getKGM()->setLastUsedDirectory("REPLACE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecUniqueFilePaths = CPathUtility::getUniqueFilePaths(vecPaths, getKGM()->getEntryListTab()->getIMGFile()->getEntryNames());
	vector<string> vecReplaceFilePaths = CVectorUtility::getUniqueEntries(vecPaths, vecUniqueFilePaths);
	uint32 uiReplaceEntryCount = vecReplaceFilePaths.size();

	bool bOverwriteFiles = false;
	if (getKGM()->getSettingsManager()->getSettingBool("AskBeforeOverwritingFiles"))
	{
		getKGM()->getTaskManager()->onTaskPause();
		bOverwriteFiles = getKGM()->getGUIManager()->showConfirmDialog("Replace " + CStringUtility::toString(uiReplaceEntryCount) + " entr" + (uiReplaceEntryCount == 1 ? "y" : "ies") + "?", CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_2_Title"));
		getKGM()->getTaskManager()->onTaskUnpause();

		if (!bOverwriteFiles)
		{
			vecPaths = vecUniqueFilePaths;
		}
	}
	else
	{
		//vector<string> vecA = CVectorUtility::getUniqueEntries(getKGM()->getEntryListTab()->getIMGFile()->getEntryNamesUpper(), CPathUtility::getFileNamesFromFilePaths(vecPaths));

		vector<string> vecNewReplacedFilePaths;
		for (auto strFilePath : vecReplaceFilePaths)
		{
			bool bReplaceEntry = true;
			CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getIMGFile()->getEntryByName(CPathUtility::getFileName(strFilePath));

			uint32
				uiExistingEntryFileCreationDate = pIMGEntry->getFileCreationDate(),
				uiNewEntryFileCreationDate = CFileUtility::getFileCreationDate(strFilePath);

			if (uiExistingEntryFileCreationDate < uiNewEntryFileCreationDate)
			{
				// the existing entry is older than the new entry

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteOlderFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}
			else if (uiExistingEntryFileCreationDate > uiNewEntryFileCreationDate)
			{
				// the existing entry is newer than the new entry

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteNewerFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}

			if (pIMGEntry->isProtectedEntry())
			{
				// the entry is protected

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteProtectedFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}

			if (bReplaceEntry)
			{
				vecNewReplacedFilePaths.push_back(strFilePath);
			}
		}
		vecPaths = CVectorUtility::combineVectors(vecUniqueFilePaths, vecNewReplacedFilePaths);
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecPaths.size());
	vector<string> vecReplacedEntryNames;
	getKGM()->getEntryListTab()->replace(vecPaths, vecReplacedEntryNames);

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("ReplacedEntries", vecReplacedEntryNames.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntriesForReplace"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecReplacedEntryNames, "\n"), true);

	getKGM()->getEntryListWindow()->refreshActiveTab();
	getKGM()->getTaskManager()->onTaskEnd("onRequestReplace");
	*/
}
void		CTaskDispatchManager::onRequestExportSelected(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportSelected");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportSelected", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_SELECTED"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportSelected", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_SELECTED", strPath);

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecExportedEntryNames;

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry = nullptr;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportSelected", true);
		return;
	}
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		vecIMGEntries.push_back(pIMGEntry);
		vecExportedEntryNames.push_back(pIMGEntry->getEntryName());

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_76", vecIMGEntries.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_77"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecExportedEntryNames, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportSelected");
	*/
}
void		CTaskDispatchManager::onRequestSearchText(void) // from search box
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSearchText");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSearchText", true);
		return;
	}

	CString strSearchText2;
	((CEdit*)getKGM()->getDialog()->GetDlgItem(24))->GetWindowTextW(strSearchText2);
	string strSearchText = CStringUtility::convertCStringToStdString(strSearchText2);
	if (strSearchText == "")
	{
		CListCtrl *pListControlSearch = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(22);
		pListControlSearch->DeleteAllItems();
		((CStatic*)getKGM()->getDialog()->GetDlgItem(0))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", 0).c_str());

		getKGM()->getTaskManager()->onTaskEnd("onRequestSearchText", true);
		return;
	}

	bool bSearchInAllTabs = ((CButton*)getKGM()->getDialog()->GetDlgItem(46))->GetCheck() == BST_CHECKED;
	if(bSearchInAllTabs)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_79", strSearchText.c_str()), bSearchInAllTabs);
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_78", strSearchText.c_str()), bSearchInAllTabs);
	}

	getKGM()->getEntryListTab()->setSearchText(strSearchText);
	getKGM()->getEntryListTab()->searchText();

	if (bSearchInAllTabs)
	{
		getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_3", getKGM()->getEntryListWindow()->getSearchHitCount(), getKGM()->getEntryListWindow()->getSearchFileCount()), true);
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_3", getKGM()->getEntryListWindow()->getSearchHitCount(), getKGM()->getEntryListWindow()->getSearchFileCount()), true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestSearchText");
	*/
}
void		CTaskDispatchManager::onRequestSearchSelection(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSearchSelection");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSearchSelection", true);
		return;
	}

	CListCtrl *pListControlSearch = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(22));
	CListCtrl *pListControlMain = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	CSearchEntry *pSearchEntry;
	POSITION pos = pListControlSearch->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSearchSelection", true);
		return;
	}
	int nItem2 = -1;
	while (pos)
	{
		int nItem = pListControlSearch->GetNextSelectedItem(pos);
		pSearchEntry = (CSearchEntry*)pListControlSearch->GetItemData(nItem);

		if (pSearchEntry->getWindowTab() != getKGM()->getEntryListTab())
		{
			getKGM()->getEntryListWindow()->setActiveTab(pSearchEntry->getWindowTab());
		}
		nItem2 = getKGM()->getEntryListWindow()->getMainListControlItemByEntry(pSearchEntry->getIMGEntry());
		pListControlMain->SetItemState(nItem2, LVIS_SELECTED, LVIS_SELECTED);
		pListControlMain->SetSelectionMark(nItem2);
	}
	//pListControlMain->SetFocus();

	if (nItem2 != -1)
	{
		CRect ItemRect;
		pListControlMain->GetSubItemRect(1, 1, LVIR_BOUNDS, ItemRect);
		uint32 uiHeight = ItemRect.bottom - ItemRect.top;

		uint32 uiCurrentScroll = (int)pListControlMain->GetScrollPos(SB_VERT);

		pListControlMain->Scroll(CSize(0, (nItem2 - uiCurrentScroll) * uiHeight));
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestSearchSelection");
	*/
}
void		CTaskDispatchManager::onRequestFilter(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestFilter");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFilter", true);
		return;
	}

	if (getKGM()->getEntryListTab()->isRestoringFilterOptions())
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFilter", true);
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getKGM()->getEntryListTab()->getIMGFile()->getEntryCount());

	getKGM()->getEntryListTab()->storeFilterOptions();
	getKGM()->getEntryListTab()->readdAllEntriesToMainListView();
	getKGM()->getTaskManager()->onTaskEnd("onRequestFilter");
}
void		CTaskDispatchManager::onRequestFind(bool bFindInAllOpenedFiles) // from menu
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestFind");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFind", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strSearchText = getKGM()->getGUIManager()->showTextInputDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_2_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_2_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	
	if (strSearchText == "")
	{
		CListCtrl *pListControlSearch = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(22);
		pListControlSearch->DeleteAllItems();
		((CStatic*)getKGM()->getDialog()->GetDlgItem(0))->SetWindowTextW(CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Window_Main_Text_SearchResult_ActiveTab", 0).c_str());

		getKGM()->getTaskManager()->onTaskEnd("onRequestFind", true);
		return;
	}

	((CEdit*)getKGM()->getDialog()->GetDlgItem(24))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(strSearchText).c_str());

	if (bFindInAllOpenedFiles)
	{
		((CButton*)getKGM()->getDialog()->GetDlgItem(46))->SetCheck(BST_CHECKED);
	}

	getKGM()->getEntryListTab()->setSearchText(strSearchText);
	getKGM()->getEntryListTab()->searchText();
	getKGM()->getTaskManager()->onTaskEnd("onRequestFind");
	*/
}
void		CTaskDispatchManager::onRequestExportViaButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportViaButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showExportViaDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaButton", true);
		return;
	}

	switch (uiRadioButtonIndex)
	{
	case 0:
		onRequestExportSelected();
		break;
	case 1:
		onRequestExportViaIDEFile();
		break;
	case 2:
		onRequestExportViaTextLines();
		break;
	case 3:
		onRequestExportViaIPLFile();
		break;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaButton");
}
void		CTaskDispatchManager::onRequestExportViaIDEFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportViaIDEFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIDEFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("EXPORT_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIDEFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("EXPORT_IDE__IDE", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_IDE__DESTINATION"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIDEFile", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_IDE__DESTINATION", strPath);

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecExportedEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNamesWithoutExtension.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
			vecExportedEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size());
	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_80", vecIMGEntries.size(), getKGM()->getEntryListTab()->getIMGFile()->getEntryCount()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_77"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecExportedEntryNames, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIDEFile");
}
void		CTaskDispatchManager::onRequestExportViaTextLines(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportViaTextLines");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_TEXTLINES"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaTextLines", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_TEXTLINES", strPath);

	vector<string> vecEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		vecEntryNames.push_back(strLine);
	}
	vecEntryNames = CVectorUtility::toUpperCase(vecEntryNames);

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecExportedEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNames.begin(), vecEntryNames.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNames.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
			vecExportedEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size());
	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_81", vecIMGEntries.size(), vecEntryNames.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_77"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecExportedEntryNames, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaTextLines");
}
void		CTaskDispatchManager::onRequestSortEntries(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSortEntries");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortEntries", true);
		return;
	}

	getKGM()->getEntryListTab()->sortEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestSortEntries");
}
void		CTaskDispatchManager::onRequestSortButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSortButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<int> vecSortOptions = getKGM()->getGUIManager()->showSortOptionsDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecSortOptions[10] == -1)
	{
		return;
	}

	bool bCancel = false;
	for (uint32 i = 0; i < 10; i++)
	{
		for (uint32 i2 = 0; i2 < 9; i2++)
		{
			// todo CheckMenuItem(getKGM()->m_hMenu_Entry_Sort, 1200 + (20 * i) + i2, MF_UNCHECKED);
		}

		if (vecSortOptions[i] != -1)
		{
			// todo CheckMenuItem(getKGM()->m_hMenu_Entry_Sort, 1200 + (20 * i) + vecSortOptions[i], MF_CHECKED);
		}

		uint32 uiPriority = i;
		// todo CMenu *pSortMenu = nullptr; // todo CMenu::FromHandle(getKGM()->m_hMenu_Entry_Sort);
		uint32 wp = 1200 + (20 * i) + vecSortOptions[i];

		if (vecSortOptions[i] == 6) // sort by IDE file
		{
			getKGM()->getTaskManager()->onTaskPause();
			vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("SORT_IDE"), "IDE", false);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (vecPaths.size() == 0)
			{
				bCancel = true;
			}
			else
			{
				getKGM()->setLastUsedDirectory("SORT_IDE", CPathUtility::getDirectory(vecPaths[0]));

				CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(vecPaths[0]);
				if(!pIDEFile->doesHaveError())
				{
					vector<string> vecModelNames = pIDEFile->getModelNames();
					vector<string> vecTextureNames = pIDEFile->getTXDNames();
					vector<string> vecEntryNames = CVectorUtility::toUpperCase(CVectorUtility::combineVectors(vecModelNames, vecTextureNames));
					getKGM()->getSortManager()->getSortPriorities()->getEntryByIndex((uint16)uiPriority)->setData(vecEntryNames);

					// todo pSortMenu->ModifyMenuW(LOWORD(wp), 0, LOWORD(wp), CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Sort_ByText_WithFilename", "IDE file", CPathUtility::getFileName(pIDEFile->getFilePath()).c_str()).c_str());
				}
				pIDEFile->unload();
				delete pIDEFile;
			}
		}
		else if (vecSortOptions[i] == 7) // sort by COL file
		{
			getKGM()->getTaskManager()->onTaskPause();
			vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("SORT_COL"), "COL", false);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (vecPaths.size() == 0)
			{
				bCancel = true;
			}
			else
			{
				getKGM()->setLastUsedDirectory("SORT_COL", CPathUtility::getDirectory(vecPaths[0]));

				CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(vecPaths[0]);
				if(!pCOLFile->doesHaveError())
				{
					vector<string> vecEntryNames = CVectorUtility::toUpperCase(pCOLFile->getModelNames());
					getKGM()->getSortManager()->getSortPriorities()->getEntryByIndex((uint16)uiPriority)->setData(vecEntryNames);

					// todo pSortMenu->ModifyMenuW(LOWORD(wp), 0, LOWORD(wp), CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Sort_ByText_WithFilename", "COL file", CPathUtility::getFileName(pCOLFile->getFilePath()).c_str()).c_str());
				}
				pCOLFile->unload();
				delete pCOLFile;
			}
		}
		else if (vecSortOptions[i] == 8) // sort by file extensions
		{
			getKGM()->getTaskManager()->onTaskPause();
			string strText = getKGM()->getGUIManager()->showTextInputDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_3_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_4_Message"));
			getKGM()->getTaskManager()->onTaskUnpause();
			if (strText == "")
			{
				bCancel = true;
			}
			else
			{
				vector<string> vecFileExtensions = CStringUtility::split(strText, ",");
				for (uint32 i = 0; i < vecFileExtensions.size(); i++)
				{
					vecFileExtensions[i] = CStringUtility::toUpperCase(CStringUtility::trim(vecFileExtensions[i]));
					if (vecFileExtensions[i][0] == '.')
					{
						vecFileExtensions[i] = vecFileExtensions[i].substr(1);
					}
				}
				getKGM()->getSortManager()->getSortPriorities()->getEntryByIndex((uint16)uiPriority)->setData(vecFileExtensions);
				// todo pSortMenu->ModifyMenuW(LOWORD(wp), 0, LOWORD(wp), CLocalizationManager::getInstance()->getTranslatedFormattedTextW("Sort_ByText_WithFilename", CLocalizationManager::getInstance()->getTranslatedTextW("Sort_Extensions").c_str(), CStringUtility::join(vecFileExtensions, ", ").c_str()).c_str());
			}
		}

		if (!bCancel)
		{
			// todo CheckMenuItem(getKGM()->m_hMenu_Entry_Sort, LOWORD(wp), MF_CHECKED);
		}
	}

	if (bCancel)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortButton", true);
		return;
	}

	getKGM()->getEntryListTab()->sortEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestSortButton");
}
void		CTaskDispatchManager::onRequestRemoveViaIDEFile(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestRemoveViaIDEFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaIDEFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("REMOVE_IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaIDEFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("REMOVE_IDE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	vector<CIMGEntry*> vecIMGEntries;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNamesWithoutExtension.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getKGM()->getEntryListTab()->getIMGFile()->getEntryCount());

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	vector<string> vecRemovedEntryNames;
	for (auto pIMGEntry : vecIMGEntries)
	{
		int nItem = getKGM()->getEntryListTab()->getIMGFile()->getIndexByEntry(pIMGEntry);
		pListControl->DeleteItem(nItem);

		vecRemovedEntryNames.push_back(pIMGEntry->getEntryName());
		getKGM()->getEntryListTab()->removeEntry(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	if (pListControl->GetItemCount() == 0)
	{
		getKGM()->getEntryListTab()->readdAllEntriesToMainListView();
	}

	getKGM()->getEntryListTab()->searchText();

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_82", vecIMGEntries.size(), vecEntryNamesWithoutExtension.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_83"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecRemovedEntryNames, "\n"), true);

	if (vecIMGEntries.size() > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaIDEFile");
	*/
}
void		CTaskDispatchManager::onRequestRemoveViaTextLines(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestRemoveViaTextLines");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_4_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaTextLines", true);
		return;
	}

	vector<string> vecEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		vecEntryNames.push_back(CPathUtility::removeFileExtension(strLine));
	}
	vecEntryNames = CVectorUtility::toUpperCase(vecEntryNames);

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecEntryNames.size());

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	uint32 uiRemoveCount = 0;
	vector<string> vecRemovedEntryNames;
	for (auto strEntryName : vecEntryNames)
	{
		CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getIMGFile()->getEntryByNameWithoutExtension(strEntryName);
		if (pIMGEntry != nullptr)
		{
			int nItem = getKGM()->getEntryListTab()->getIMGFile()->getIndexByEntry(pIMGEntry);
			pListControl->DeleteItem(nItem);

			vecRemovedEntryNames.push_back(pIMGEntry->getEntryName());

			getKGM()->getEntryListTab()->removeEntry(pIMGEntry);

			uiRemoveCount++;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	if (pListControl->GetItemCount() == 0)
	{
		getKGM()->getEntryListTab()->readdAllEntriesToMainListView();
	}

	getKGM()->getEntryListTab()->searchText();

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_82", uiRemoveCount, vecEntryNames.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_83"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecRemovedEntryNames, "\n"), true);

	if (uiRemoveCount > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaTextLines");
	*/
}
void		CTaskDispatchManager::onRequestRemoveViaButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRemoveViaButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showRemoveViaDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaButton", true);
		return;
	}

	switch (uiRadioButtonIndex)
	{
	case 0:
		onRequestRemoveSelected();
		break;
	case 1:
		onRequestRemoveViaIDEFile();
		break;
	case 2:
		onRequestRemoveViaTextLines();
		break;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveViaButton");
}
void		CTaskDispatchManager::onRequestImportViaButton(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestImportViaButton");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaButton", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	uint32 uiRadioButtonIndex = getKGM()->getGUIManager()->showImportViaDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (uiRadioButtonIndex == 0xFFFFFFFF)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaButton", true);
		return;
	}

	switch (uiRadioButtonIndex)
	{
	case 0:
		onRequestImportViaFiles();
		break;
	case 1:
		onRequestImportViaIDEFile();
		break;
	case 2:
		onRequestImportViaTextLines();
		break;
	case 3:
		onRequestImportViaFolder();
		break;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaButton");
}
void		CTaskDispatchManager::onRequestImportViaIDEFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestImportViaIDEFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaIDEFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("IMPORT_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaIDEFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("IMPORT_IDE__IDE", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	string strFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_13"), getKGM()->getLastUsedDirectory("IMPORT_IDE__SOURCE"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaIDEFile", true);
		return;
	}
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
	getKGM()->setLastUsedDirectory("IMPORT_IDE__SOURCE", strFolderPath);

	vector<string> vecEntryNamesInAllFiles;
	for (auto strPath : vecPaths)
	{
		CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strPath);
		vector<string> vecEntryNames;
		if(!pIDEFile->doesHaveError())
		{
			vector<string> vecModelNames = pIDEFile->getModelNames();
			for (string& strModelName : vecModelNames)
			{
				strModelName += ".DFF";
			}
			
			vector<string> vecTextureNames = pIDEFile->getTXDNames();
			for (string& strTextureName : vecTextureNames)
			{
				strTextureName += ".TXD";
			}

			vecEntryNames = CVectorUtility::combineVectors(vecModelNames, vecTextureNames);
		}
		pIDEFile->unload();
		delete pIDEFile;

		vecEntryNamesInAllFiles = CVectorUtility::combineVectors(vecEntryNamesInAllFiles, CVectorUtility::toUpperCase(vecEntryNames));
	}
	vecEntryNamesInAllFiles = CVectorUtility::removeDuplicates(vecEntryNamesInAllFiles);

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecEntryNamesInAllFiles.size());
	uint32 uiImportCount = 0;
	vector<string> vecImportedEntryNames;
	for (auto strEntryName : vecEntryNamesInAllFiles)
	{
		string strEntryPath = strFolderPath + strEntryName;
		if (CFileUtility::doesFileExist(strEntryPath))
		{
			uiImportCount++;
			getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strEntryPath);
			vecImportedEntryNames.push_back(strEntryName);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_84", uiImportCount, vecEntryNamesInAllFiles.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntriesForImport"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecImportedEntryNames, "\n"), true);

	if (uiImportCount > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaIDEFile");
}
void		CTaskDispatchManager::onRequestImportViaTextLines(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestImportViaTextLines");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_5_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaTextLines", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_13"), getKGM()->getLastUsedDirectory("IMPORT_TEXTLINES"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaTextLines", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("IMPORT_TEXTLINES", strPath);

	vector<string> vecEntryNamesWithoutExtension, vecImportedEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		strLine = CPathUtility::removeFileExtension(strLine);
		vecEntryNamesWithoutExtension.push_back(strLine);
	}
	vecEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension));

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecEntryNamesWithoutExtension.size());
	uint32 uiImportCount = 0;
	for (auto strEntryNameWithoutExtension : vecEntryNamesWithoutExtension)
	{
		string strFileName = CFileUtility::getFileNameFromNameWithoutExtension(strPath, strEntryNameWithoutExtension);
		if (strFileName != "")
		{
			uiImportCount++;
			getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strPath + strFileName);
			vecImportedEntryNames.push_back(strFileName);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_84", uiImportCount, vecEntryNamesWithoutExtension.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntriesForImport"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecImportedEntryNames, "\n"), true);

	if (uiImportCount > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaTextLines");
}
void		CTaskDispatchManager::onRequestNew(eIMGVersion eIMGVersion)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_New_IMGVersion(eIMGVersion);
	getKGM()->getTaskManager()->onTaskBegin("onRequestNew");
	//TCHAR szCurrentDirectory[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, szCurrentDirectory);

	//string strFilePath = CPathUtility::addSlashToEnd(CStringUtility::convertStdWStringToStdString(szCurrentDirectory)) + "Untitled.img";
	//strFilePath = CFileUtility::getNextIncrementingFileName(strFilePath);

	/*
	getKGM()->getTaskManager()->onTaskPause();
	string strFilePath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("NEW"), "IMG,DIR", "IMG.img");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strFilePath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestNew", true);
		return;
	}
	getKGM()->setLastUsedDirectory("NEW", strFilePath);
	*/

	string strFilePath = getKGM()->getInstallationMeta().getLocalAppPath() + "New/IMG/New.img";
	strFilePath = CFileUtility::getNextIncrementingFileName(strFilePath);
	CFileUtility::createFoldersForPath(strFilePath);
	strFilePath = CStringUtility::replace(strFilePath, "/", "\\");
	getKGM()->getEntryListWindow()->addBlankTab(strFilePath, eIMGVersion);
	getKGM()->getTaskManager()->onTaskEnd("onRequestNew");
}
void		CTaskDispatchManager::onRequestStats(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestStats");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestStats", true);
		return;
	}

	unordered_map<uint32, uint32> umapStatsRWVersions;
	unordered_map<string, uint32> umapStatsExtensions;

	for (CIMGEntry *pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (pIMGEntry->getRWVersion() != nullptr)
		{
			if (pIMGEntry->isCOLFile())
			{
			}
			else
			{
				if (umapStatsRWVersions.count(pIMGEntry->getRWVersion()->getVersionCC()) == 0) // crashes when calling getVersionCC()
				{
					umapStatsRWVersions.insert(std::pair<uint32, uint32>(pIMGEntry->getRWVersion()->getVersionCC(), 1));
				}
				else
				{
					umapStatsRWVersions[pIMGEntry->getRWVersion()->getVersionCC()]++;
				}
			}
		}

		string strExtension = CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
		if (umapStatsExtensions.count(strExtension) == 0)
		{
			umapStatsExtensions.insert(std::pair<string, uint32>(strExtension, 1));
		}
		else
		{
			umapStatsExtensions[strExtension]++;
		}
	}

	unordered_map<uint32, vector<string>> umapVersionNames1 = CRWManager::getInstance()->getVersionManager()->getVersionNames();
	unordered_map<uint32, string> umapVersionNames2;
	for(auto it : umapVersionNames1)
	{
		uint32 uiVersionCC = it.first;
		vector<string> vecVersionNames3 = it.second;
		string strVersionName = vecVersionNames3[0];
		string strLocalizationKey = vecVersionNames3[1];
		
		umapVersionNames2[uiVersionCC] = strVersionName + " (" + CLocalizationManager::getInstance()->getTranslatedText(strLocalizationKey) + ")";
	}
	
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showStatsDialog(umapStatsRWVersions, umapStatsExtensions, umapVersionNames2);
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestStats");
}
void		CTaskDispatchManager::onRequestNameCase(uint8 ucCaseType, uint8 ucFilenameType)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_NameCase_CaseType(ucCaseType);
	getKGM()->getLastUsedValueManager()->setLastUsedValue_NameCase_FilenameType(ucFilenameType);
	getKGM()->getTaskManager()->onTaskBegin("onRequestNameCase");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestNameCase", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestNameCase", true);
		return;
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (ucCaseType == 0)
		{
			if (ucFilenameType == 0)
			{
				pIMGEntry->setEntryName(CStringUtility::toLowerCase(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 1)
			{
				pIMGEntry->setEntryName(CStringUtility::toLowerCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName())) + "." + CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 2)
			{
				pIMGEntry->setEntryName(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "." + CStringUtility::toLowerCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
			}
		}
		else if (ucCaseType == 1)
		{
			if (ucFilenameType == 0)
			{
				pIMGEntry->setEntryName(CStringUtility::toUpperCase(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 1)
			{
				pIMGEntry->setEntryName(CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName())) + "." + CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 2)
			{
				pIMGEntry->setEntryName(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "." + CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
			}
		}
		else if (ucCaseType == 2)
		{
			if (ucFilenameType == 0)
			{
				pIMGEntry->setEntryName(CStringUtility::toTitleCase(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 1)
			{
				pIMGEntry->setEntryName(CStringUtility::toTitleCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName())) + "." + CPathUtility::getFileExtension(pIMGEntry->getEntryName()));
			}
			else if (ucFilenameType == 2)
			{
				pIMGEntry->setEntryName(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()) + "." + CStringUtility::toTitleCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
			}
		}

		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->searchText();

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestNameCase");
	*/
}
void		CTaskDispatchManager::onRequestCopyEntryData(eIMGEntryProperty eIMGEntryProperty)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Copy_IMGEntryProperty(eIMGEntryProperty);
	getKGM()->getTaskManager()->onTaskBegin("onRequestCopyEntryData");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCopyEntryData", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry;
	vector<string> vecCopyLines;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCopyEntryData", true);
		return;
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		switch (eIMGEntryProperty)
		{
		case IMG_ENTRY_ID:
			vecCopyLines.push_back(CStringUtility::toString(getKGM()->getEntryListTab()->getIMGFile()->getIndexByEntry(pIMGEntry) + 1));
			break;
		case IMG_ENTRY_NAME:
			vecCopyLines.push_back(pIMGEntry->getEntryName());
			break;
		case IMG_ENTRY_OFFSET:
			vecCopyLines.push_back(CStringUtility::toString(pIMGEntry->getEntryOffsetInSectors() * 2048));
			break;
		case IMG_ENTRY_RESOURCETYPE:
			vecCopyLines.push_back(pIMGEntry->getRageResourceType()->getResourceName());
			break;
		case IMG_ENTRY_RWVERSION:
			vecCopyLines.push_back(pIMGEntry->getRWVersion()->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pIMGEntry->getRWVersion()->getLocalizationKey()) + ")");
			break;
		case IMG_ENTRY_SIZE:
			vecCopyLines.push_back(CStringUtility::toString(pIMGEntry->getEntrySize()));
			break;
		case IMG_ENTRY_TYPE:
			vecCopyLines.push_back(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())));
			break;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	CStringUtility::setClipboardText(CStringUtility::join(vecCopyLines, "\r\n"));
	getKGM()->getTaskManager()->onTaskEnd("onRequestCopyEntryData");
	*/
}
void		CTaskDispatchManager::onRequestShift(uint8 ucDirection)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Shift_Direction(ucDirection);
	getKGM()->getTaskManager()->onTaskBegin("onRequestShift");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestShift", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry = nullptr;
	int nItem;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestShift", true);
		return;
	}
	while (pos)
	{
		nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		break;
	}

	if (ucDirection == 0) // up
	{
		if (nItem == 0)
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestShift", true);
			return;
		}
	}
	else if (ucDirection == 1) // down
	{
		if (nItem == (pListControl->GetItemCount() - 1))
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestShift", true);
			return;
		}
	}

	CIMGEntry *pIMGEntry2;
	uint32 uiIMGEntry1Index = nItem;
	uint32 uiIMGEntry2Index;
	if (ucDirection == 0) // up
	{
		uiIMGEntry2Index = nItem - 1;
	}
	else if (ucDirection == 1) // down
	{
		uiIMGEntry2Index = nItem + 1;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(4);

	pIMGEntry2 = getKGM()->getEntryListTab()->getIMGFile()->getEntryByIndex(uiIMGEntry2Index);
	getKGM()->getEntryListTab()->getIMGFile()->swapEntries(pIMGEntry, pIMGEntry2);
	getKGM()->getTaskManager()->onTaskProgressTick();

	pListControl->SetItemData(uiIMGEntry1Index, (DWORD)pIMGEntry2);
	pListControl->SetItemData(uiIMGEntry2Index, (DWORD)pIMGEntry);
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry2);
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestShift");
	*/
}
void		CTaskDispatchManager::onRequestQuickExport(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestQuickExport");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestQuickExport", true);
		return;
	}

	if (getKGM()->getSettingsManager()->getSettingString("QuickExportPath") == "")
	{
		getKGM()->getTaskManager()->onTaskPause();
		string strQuickExportPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_1"), getKGM()->getLastUsedDirectory("QUICK_EXPORT"));
		getKGM()->getTaskManager()->onTaskUnpause();
	
		if (strQuickExportPath == "")
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestQuickExport", true);
			return;
		}
		
		getKGM()->setLastUsedDirectory("QUICK_EXPORT", strQuickExportPath);
		getKGM()->getSettingsManager()->setSettingString("QuickExportPath", strQuickExportPath);
	}

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecExportedEntryNames;
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	CIMGEntry *pIMGEntry = nullptr;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestQuickExport", true);
		return;
	}
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);
		vecIMGEntries.push_back(pIMGEntry);
		vecExportedEntryNames.push_back(pIMGEntry->getEntryName());

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntries, getKGM()->getSettingsManager()->getSettingString("QuickExportPath"));
	if (vecIMGEntries.size() == 1)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_86", vecIMGEntries[0]->getEntryName().c_str(), vecIMGEntries[0]->getEntrySize()));
	}
	else
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_87", vecIMGEntries.size()));
	}
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_88"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecExportedEntryNames, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestQuickExport");
	*/
}
void		CTaskDispatchManager::onRequestSelectViaFileExtension(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSelectViaFileExtension");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaFileExtension", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strText = getKGM()->getGUIManager()->showTextInputDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_4_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_4_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strText == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaFileExtension", true);
		return;
	}

	vector<string> vecFileExtensions = CStringUtility::split(strText, ",");
	for (uint32 i = 0; i < vecFileExtensions.size(); i++)
	{
		vecFileExtensions[i] = CStringUtility::toUpperCase(CStringUtility::trim(vecFileExtensions[i]));
		if (vecFileExtensions[i].c_str()[0] == '.')
		{
			vecFileExtensions[i] = vecFileExtensions[i].substr(1);
		}
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	uint32 uiSelectedEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());
	for (uint32 i = 0; i < (uint32)pListControl->GetItemCount(); i++)
	{
		if (std::find(vecFileExtensions.begin(), vecFileExtensions.end(), CStringUtility::toUpperCase(CPathUtility::getFileExtension(getKGM()->getEntryListTab()->getIMGFile()->getEntryByIndex(i)->getEntryName()))) != vecFileExtensions.end())
		{
			uiSelectedEntryCount++;
			pListControl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			pListControl->SetSelectionMark(i);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_89", uiSelectedEntryCount));

	pListControl->SetFocus();
	getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaFileExtension");
	*/
}
void		CTaskDispatchManager::onRequestSelectViaRWVersion(CRWVersion *pRWVersion)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Select_RWVersion(pRWVersion);
	getKGM()->getTaskManager()->onTaskBegin("onRequestSelectViaRWVersion");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaRWVersion", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	uint32 uiSelectedEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());
	for (uint32 i = 0; i < (uint32)pListControl->GetItemCount(); i++)
	{
		CRWVersion *pEntryRWVersion = getKGM()->getEntryListTab()->getIMGFile()->getEntryByIndex(i)->getRWVersion();
		if (pRWVersion == pEntryRWVersion || (pRWVersion->getVersionCC() == 0x0 && pEntryRWVersion == nullptr))
		{
			uiSelectedEntryCount++;
			pListControl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			pListControl->SetSelectionMark(i);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_90", uiSelectedEntryCount));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_91", (pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").c_str()), true);

	pListControl->SetFocus();
	getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaRWVersion");
	*/
}
void		CTaskDispatchManager::onRequestVersion(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestVersion");
	CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_36", getKGM()->getBuildMeta().getCurrentVersionString().c_str(), BUILDNUMBER_STR), CLocalizationManager::getInstance()->getTranslatedText("Version"), MB_OK);
	getKGM()->getTaskManager()->onTaskEnd("onRequestVersion");
}
void		CTaskDispatchManager::onRequestTextureList(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestTextureList");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestTextureList", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	CIMGEntry *pIMGEntry = nullptr;
	int nItem;
	vector<string> vecTextureNames;
	uint32 uiEntryCount = 0;
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestTextureList", true);
		return;
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		string strFileData = pIMGEntry->getEntryData();
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) == "TXD")
		{
			//CDebugger::log(pIMGEntry->m_strFileName);
			CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strFileData);
			if(!pTXDFile->doesHaveError())
			{
				for (auto pTexture : pTXDFile->getTextures())
				{
					if (pTexture->doesHaveDiffuse())
					{
						vecTextureNames.push_back(pTexture->getDiffuseName());
					}
					if (pTexture->doesHaveAlpha())
					{
						vecTextureNames.push_back(pTexture->getAlphaName());
					}
				}
			}
			pTXDFile->unload();
			delete pTXDFile;
			uiEntryCount++;
		}
		else if (CPathUtility::isModelExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()))))
		{
			CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(strFileData);
			if(!pDFFFile->doesHaveError())
			{
				for (auto pTextureEntry : pDFFFile->getTextureEntries())
				{
					if (pTextureEntry->doesHaveDiffuse())
					{
						vecTextureNames.push_back(pTextureEntry->getDiffuseName());
					}
					if (pTextureEntry->doesHaveAlpha())
					{
						vecTextureNames.push_back(pTextureEntry->getAlphaName());
					}
				}
			}
			pDFFFile->unload();
			delete pDFFFile;
			uiEntryCount++;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getTaskManager()->onTaskPause();
	CTextureListDialogData *pTextureListDialogData = getKGM()->getGUIManager()->showTextureListDialog("Texture List", "Showing " + CStringUtility::toString(vecTextureNames.size()) + " texture name" + (vecTextureNames.size() == 1 ? "" : "s") + " for " + CStringUtility::toString(uiEntryCount) + " IMG entr" + (uiEntryCount == 1 ? "y" : "ies") + ".", "Texture Name", vecTextureNames);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (pTextureListDialogData->m_bSaveTexturesFormat2)
	{
		string strData = "";
		strData += "[TXDList]\n";
		strData += "Count=" + CStringUtility::toString(vecTextureNames.size()) + "\n\n";

		for (uint32 i = 0, j = vecTextureNames.size(); i < j; i++)
		{
			strData += "[Texture" + CStringUtility::toString(i + 1) + "]\n";
			strData += "path=" + vecTextureNames[i] + "\n\n";
		}

		getKGM()->getTaskManager()->onTaskPause();
		string strFilePath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("SAVE_TEXTURE_LIST"), "TXT", CLocalizationManager::getInstance()->getTranslatedText("SaveFilePopup_2_InitialFilename"));
		getKGM()->getTaskManager()->onTaskUnpause();
		if (strFilePath == "")
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestTextureList", true);
			return;
		}
		getKGM()->setLastUsedDirectory("SAVE_TEXTURE_LIST", strFilePath);

		CFileUtility::storeFile(strFilePath, strData, true, false);
	}
	delete pTextureListDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestTextureList");
	*/
}
void		CTaskDispatchManager::onRequestAssociateIMGExtension(void)
{
	//TCHAR szExePath[MAX_PATH];
	//GetModuleFileName(NULL, szExePath, MAX_PATH);

	//CGUIUtility::showMessage(NULL, szExePath, "A", MB_OK);

	//CRegistryUtility::assoicateFileExtension("img", CStringUtility::convertStdWStringToStdString(szExePath));
}
string		CTaskDispatchManager::onRequestSaveLog(bool bActiveTab, bool bNormalFormat)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_SaveLog_ActiveTab(bActiveTab);
	getKGM()->getLastUsedValueManager()->setLastUsedValue_SaveLog_NormalFormat(bNormalFormat);
	getKGM()->getTaskManager()->onTaskBegin("onRequestSaveLog");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSaveLog", true);
		return "";
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strSaveFilePath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("SAVE_LOG"), "TXT", CLocalizationManager::getInstance()->getTranslatedText("SaveFilePopup_8_InitialFilename"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strSaveFilePath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSaveLog", true);
		return "";
	}
	getKGM()->setLastUsedDirectory("SAVE_LOG", strSaveFilePath);

	if (bActiveTab)
	{
		if (bNormalFormat)
		{
			CFileUtility::storeFile(strSaveFilePath, CStringUtility::join(getKGM()->getEntryListTab()->getLogLinesBasic(), "\n"), false, false);
		}
		else
		{
			CFileUtility::storeFile(strSaveFilePath, CStringUtility::join(getKGM()->getEntryListTab()->getLogLinesExtended(), "\n"), false, false);
		}
	}
	else
	{
		string strLogData = "";
		if (bNormalFormat)
		{
			for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
			{
				strLogData += "[[" + ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath() + "]]\n" + CStringUtility::join(((CEntryListWindowTab*)pWindowTab)->getLogLinesBasic(), "\n") + "\n\n";
			}
		}
		else
		{
			for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
			{
				strLogData += "[[" + ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath() + "]]\n" + CStringUtility::join(((CEntryListWindowTab*)pWindowTab)->getLogLinesExtended(), "\n") + "\n\n";
			}
		}
		CFileUtility::storeFile(strSaveFilePath, strLogData, false, false);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestSaveLog");

	return strSaveFilePath;
}
void		CTaskDispatchManager::onRequestSaveSession(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSaveSession");
	if (getKGM()->getEntryListWindow()->getEntryCount() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSaveSession", true);
		return;
	}

	string strSessionName;
	bool bSemiColonFound;
	do
	{
		getKGM()->getTaskManager()->onTaskPause();
		strSessionName = getKGM()->getGUIManager()->showTextInputDialog(CLocalizationManager::getInstance()->getTranslatedText("SessionName"), CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_TextInput_5_Message", getKGM()->getEntryListWindow()->getEntryCount()));
		getKGM()->getTaskManager()->onTaskUnpause();
		if (strSessionName == "")
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestSaveSession", true);
			return;
		}

		bSemiColonFound = CStringUtility::isIn(strSessionName, ";");
		if (bSemiColonFound)
		{
			getKGM()->getTaskManager()->onTaskPause();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_37"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_37"), MB_OK);
			getKGM()->getTaskManager()->onTaskUnpause();
		}
	} while (bSemiColonFound);

	vector<string> vecPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());
	}

	getKGM()->getSessionManager()->addSession(strSessionName, vecPaths);
	getKGM()->getSessionManager()->loadSessions();

	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_4", getKGM()->getEntryListWindow()->getEntryCount(), strSessionName));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_5"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecPaths, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSaveSession");
	*/
}
void		CTaskDispatchManager::onRequestOrphanDFFEntriesNotInCOL(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanDFFEntriesNotInCOL");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInCOL", true);
		return;
	}

	// fetch DFF model names
	/*
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecDFFPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_DFF_COL__DFF"), "DFF,BSP");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecDFFPaths.size() == 0)
	{
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInCOL", true);
	return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_DFF_COL__DFF", CPathUtility::getDirectory(vecDFFPaths[0]));
	*/


	vector<string> vecDFFEntryNamesWithoutExtension;
	/*
	todo
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		CIMGEntry *pIMGEntry = (CIMGEntry*)pListControl->GetItemData(i);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) == "DFF")
		{
			vecDFFEntryNamesWithoutExtension.push_back(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		}
	}
	*/

	// choose COL files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecCOLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_DFF_COL__COL"), "COL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecCOLPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInCOL", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_DFF_COL__COL", CPathUtility::getDirectory(vecCOLPaths[0]));

	// fetch COL model names
	vector<string> vecCOLEntryNamesWithoutExtension;
	for (auto strCOLPath : vecCOLPaths)
	{
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strCOLPath);
		if (!pCOLFile->doesHaveError())
		{
			vector<string> vecModelNames = pCOLFile->getModelNames();
			vecCOLEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecCOLEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecModelNames));
		}
		pCOLFile->unload();
		delete pCOLFile;
	}
	vecCOLEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(vecCOLEntryNamesWithoutExtension);

	vector<string>
		vecEntryNamesMissingFromCOL,
		vecEntryNamesMissingFromCOLUpper;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFEntryNamesWithoutExtension.size());
	for (auto strDFFEntryNameWithoutExtension : vecDFFEntryNamesWithoutExtension)
	{
		if (std::find(vecCOLEntryNamesWithoutExtension.begin(), vecCOLEntryNamesWithoutExtension.end(), strDFFEntryNameWithoutExtension) == vecCOLEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromCOL.push_back(strDFFEntryNameWithoutExtension);
			vecEntryNamesMissingFromCOLUpper.push_back(CStringUtility::toUpperCase(strDFFEntryNameWithoutExtension));
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	if (getKGM()->getEntryListWindow()->getEntryCount() > 0)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_125", vecEntryNamesMissingFromCOL.size()));
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromCOL, "\n"), true);
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_125", vecEntryNamesMissingFromCOL.size()));
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CStringUtility::join(vecEntryNamesMissingFromCOL, "\n"), true);
	}

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromCOL, "DFF Entries missing from COL:");
	getKGM()->getTaskManager()->onTaskUnpause();

	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInCOL");
}
void		CTaskDispatchManager::onRequestOrphanIDEEntriesNotInCOL(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanIDEEntriesNotInCOL");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInCOL", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IDE_COL__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInCOL", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IDE_COL__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecCOLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IDE_COL__COL"), "COL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecCOLPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInCOL", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IDE_COL__COL", CPathUtility::getDirectory(vecCOLPaths[0]));

	vector<string> vecIDEEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);
	vecIDEEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecIDEEntryNamesWithoutExtension);

	vector<string> vecCOLEntryNamesWithoutExtension;
	for (auto strCOLPath : vecCOLPaths)
	{
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strCOLPath);
		if (pCOLFile->doesHaveError())
		{
			pCOLFile->unload();
			delete pCOLFile;
			continue;
		}
		vector<string> vecModelNames = pCOLFile->getModelNames();
		vecCOLEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecCOLEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecModelNames));
		pCOLFile->unload();
		delete pCOLFile;
	}
	vecCOLEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(vecCOLEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromCOL;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIDEEntryNamesWithoutExtension.size());
	for (auto strIDEEntryNameWithoutExtension : vecIDEEntryNamesWithoutExtension)
	{
		if (std::find(vecCOLEntryNamesWithoutExtension.begin(), vecCOLEntryNamesWithoutExtension.end(), strIDEEntryNameWithoutExtension) == vecCOLEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromCOL.push_back(strIDEEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_112", vecEntryNamesMissingFromCOL.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromCOL, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	bool bImportEntries = getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromCOL, "IDE Entries missing from COL:", "Import into IMG");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (bImportEntries)
	{
		getKGM()->getTaskManager()->onTaskPause();
		string strFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_17"), getKGM()->getLastUsedDirectory("ORPHAN_IDE_COL__IMPORT"));
		getKGM()->getTaskManager()->onTaskUnpause();
		if (strFolderPath == "")
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInCOL", true);
			return;
		}
		strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
		getKGM()->setLastUsedDirectory("ORPHAN_IDE_COL__IMPORT", strFolderPath);

		uint32 uiImportedFileCount = 0;
		for (auto strEntryNameMissingFromCOL : vecEntryNamesMissingFromCOL)
		{
			string strEntryFilePath = strFolderPath + CFileUtility::getFileNameFromNameWithoutExtension(strFolderPath, strEntryNameMissingFromCOL);
			if (CFileUtility::doesFileExist(strEntryFilePath))
			{
				getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strEntryFilePath);
				uiImportedFileCount++;
			}
		}

		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_84", uiImportedFileCount, vecEntryNamesMissingFromCOL.size()));

		if (uiImportedFileCount > 0)
		{
			getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
		}
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInCOL");
}
void		CTaskDispatchManager::onRequestOrphanDFFEntriesNotInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanDFFEntriesNotInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInIDE", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_DFF_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_DFF_IDE__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecDFFPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_DFF_IDE__DFF"), "DFF");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecDFFPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_DFF_IDE__DFF", CPathUtility::getDirectory(vecDFFPaths[0]));

	vector<string> vecIDEEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);
	vecIDEEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecIDEEntryNamesWithoutExtension);

	vector<string> vecDFFEntryNamesWithoutExtension;
	for (auto strDFFPath : vecDFFPaths)
	{
		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaFile(strDFFPath);
		if (!pDFFFile->doesHaveError())
		{
			vector<string> vecTextureNames = pDFFFile->getTextureNames();
			vecDFFEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecDFFEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecTextureNames));
		}
		pDFFFile->unload();
		delete pDFFFile;
	}
	vecDFFEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(vecDFFEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFEntryNamesWithoutExtension.size());
	for (auto strDFFEntryNameWithoutExtension : vecDFFEntryNamesWithoutExtension)
	{
		if (std::find(vecIDEEntryNamesWithoutExtension.begin(), vecIDEEntryNamesWithoutExtension.end(), strDFFEntryNameWithoutExtension) == vecIDEEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromIDE.push_back(strDFFEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_126", vecEntryNamesMissingFromIDE.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIDE, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIDE, "DFF Entries missing from IDE:");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanDFFEntriesNotInIDE");
}
void		CTaskDispatchManager::onRequestOrphanCOLEntriesNotInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanCOLEntriesNotInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanCOLEntriesNotInIDE", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_COL_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanCOLEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_COL_IDE__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecCOLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_COL_IDE__COL"), "COL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecCOLPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanCOLEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_COL_IDE__COL", CPathUtility::getDirectory(vecCOLPaths[0]));

	vector<string> vecIDEEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);
	vecIDEEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecIDEEntryNamesWithoutExtension);

	vector<string> vecCOLEntryNamesWithoutExtension;
	for (auto strCOLPath : vecCOLPaths)
	{
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strCOLPath);
		if (!pCOLFile->doesHaveError())
		{
			vector<string> vecModelNames = pCOLFile->getModelNames();
			vecCOLEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecCOLEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecModelNames));
		}
		pCOLFile->unload();
		delete pCOLFile;
	}
	vecCOLEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(vecCOLEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecCOLEntryNamesWithoutExtension.size());
	for (auto strCOLEntryNameWithoutExtension : vecCOLEntryNamesWithoutExtension)
	{
		if (std::find(vecIDEEntryNamesWithoutExtension.begin(), vecIDEEntryNamesWithoutExtension.end(), strCOLEntryNameWithoutExtension) == vecIDEEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromIDE.push_back(strCOLEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_113", vecEntryNamesMissingFromIDE.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIDE, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIDE, "COL Entries missing from IDE:");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanCOLEntriesNotInIDE");
}
void		CTaskDispatchManager::onRequestOrphanIMGEntriesNotInIDE(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanIMGEntriesNotInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIMGEntriesNotInIDE", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IMG_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIMGEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IMG_IDE__IDE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getKGM()->getEntryListTab()->getIMGFile()->getEntries().size());
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()))) == vecEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromIDE.push_back(pIMGEntry->getEntryName());
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_92", vecEntryNamesMissingFromIDE.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIDE, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	bool bRemoveEntries = getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIDE, "IMG Entries missing from IDE:", "Remove from IMG");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (bRemoveEntries)
	{
		CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
		for (auto strEntryNameMissingFromIDE : vecEntryNamesMissingFromIDE)
		{
			CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getIMGFile()->getEntryByName(strEntryNameMissingFromIDE);
			int nItem = getKGM()->getEntryListTab()->getIMGFile()->getIndexByEntry(pIMGEntry);
			pListControl->DeleteItem(nItem);

			getKGM()->getEntryListTab()->removeEntry(pIMGEntry);
		}

		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_53", vecEntryNamesMissingFromIDE.size()));

		if (vecEntryNamesMissingFromIDE.size() > 0)
		{
			getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
		}

		getKGM()->getEntryListTab()->searchText();
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIMGEntriesNotInIDE");
	*/
}
void		CTaskDispatchManager::onRequestOrphanIPLEntriesNotInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanIPLEntriesNotInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIPLEntriesNotInIDE", true);
		return;
	}

	// IDE files input
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IPL_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIPLEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IPL_IDE__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	// IPL files input
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIPLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IPL_IDE__IPL"), "IPL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIPLPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIPLEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IPL_IDE__IPL", CPathUtility::getDirectory(vecIPLPaths[0]));

	// fetch DFF names in IDE files
	vector<string> vecDFFNamesWithoutExtensionInIDE = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);
	vecDFFNamesWithoutExtensionInIDE = CVectorUtility::toUpperCase(vecDFFNamesWithoutExtensionInIDE);
	unordered_map<string, bool> umapDFFNamesWithoutExtensionInIDE = CVectorUtility::convertVectorToUnorderedMap(vecDFFNamesWithoutExtensionInIDE);
	vecDFFNamesWithoutExtensionInIDE.clear();

	// fetch DFF names in IPL files
	vector<string> vecDFFNamesWithoutExtensionInIPL;
	for (string& strIPLPath : vecIPLPaths)
	{
		CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strIPLPath);

		if (!pIPLFile->doesHaveError())
		{
			CVectorUtility::addToVector(vecDFFNamesWithoutExtensionInIPL, pIPLFile->getModelNames());
		}

		pIPLFile->unload();
		delete pIPLFile;
	}

	// find DFF names in IPL files but not found in IDE files
	vector<string> vecEntryNamesMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFNamesWithoutExtensionInIPL.size());
	for (auto strDFFEntryNameWithoutExtension : vecDFFNamesWithoutExtensionInIPL)
	{
		string strDFFEntryNameWithoutExtensionUpper = CStringUtility::toUpperCase(strDFFEntryNameWithoutExtension);
		if (umapDFFNamesWithoutExtensionInIDE.count(strDFFEntryNameWithoutExtensionUpper) == 0)
		{
			vecEntryNamesMissingFromIDE.push_back(strDFFEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_141", vecEntryNamesMissingFromIDE.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIDE, "\n"), true);

	// popup
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIDE, "IPL Entries missing from IDE:");
	getKGM()->getTaskManager()->onTaskUnpause();

	// end
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIPLEntriesNotInIDE");
}
void		CTaskDispatchManager::onRequestOrphanTXDEntriesNotInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanTXDEntriesNotInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanTXDEntriesNotInIDE", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_TXD_IDE__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanTXDEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_TXD_IDE__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecTXDPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_TXD_IDE__TXD"), "TXD");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecTXDPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanTXDEntriesNotInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_TXD_IDE__TXD", CPathUtility::getDirectory(vecTXDPaths[0]));

	vector<string> vecIDEEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, false, true);
	vecIDEEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecIDEEntryNamesWithoutExtension);

	vector<string> vecTXDEntryNamesWithoutExtension;
	for (auto strTXDPath : vecTXDPaths)
	{
		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaFile(strTXDPath);
		if (!pTXDFile->doesHaveError())
		{
			vector<string> vecTextureNames = pTXDFile->getTextureNames();
			vecTXDEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecTXDEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecTextureNames));
		}
		pTXDFile->unload();
		delete pTXDFile;
	}
	vecTXDEntryNamesWithoutExtension = CVectorUtility::removeDuplicates(vecTXDEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecTXDEntryNamesWithoutExtension.size());
	for (auto strTXDEntryNameWithoutExtension : vecTXDEntryNamesWithoutExtension)
	{
		if (std::find(vecIDEEntryNamesWithoutExtension.begin(), vecIDEEntryNamesWithoutExtension.end(), strTXDEntryNameWithoutExtension) == vecIDEEntryNamesWithoutExtension.end())
		{
			vecEntryNamesMissingFromIDE.push_back(strTXDEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_140", vecEntryNamesMissingFromIDE.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIDE, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIDE, "TXD Entries missing from IDE:");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanTXDEntriesNotInIDE");
}
void		CTaskDispatchManager::onRequestOrphanIDEEntriesNotInIMG(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOrphanIDEEntriesNotInIMG");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInIMG", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("ORPHAN_IDE_IMG__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInIMG", true);
		return;
	}
	getKGM()->setLastUsedDirectory("ORPHAN_IDE_IMG__IDE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	vector<string> vecEntryNamesMissingFromIMG;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecEntryNamesWithoutExtension.size());
	for (auto strEntryNameWithoutExtension : vecEntryNamesWithoutExtension)
	{
		CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getIMGFile()->getEntryByNameWithoutExtension(strEntryNameWithoutExtension);
		if (pIMGEntry == nullptr)
		{
			vecEntryNamesMissingFromIMG.push_back(strEntryNameWithoutExtension);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_94", vecEntryNamesMissingFromIMG.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_93"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNamesMissingFromIMG, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	bool bImportEntries = getKGM()->getGUIManager()->showOrphanEntriesDialog(vecEntryNamesMissingFromIMG, "IDE Entries missing from IMG:", "Import into IMG");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (bImportEntries)
	{
		getKGM()->getTaskManager()->onTaskPause();
		string strFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_17"), getKGM()->getLastUsedDirectory("ORPHAN_IDE_IMG__IMPORT"));
		getKGM()->getTaskManager()->onTaskUnpause();
		if (strFolderPath == "")
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInIMG", true);
			return;
		}
		strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
		getKGM()->setLastUsedDirectory("ORPHAN_IDE_IMG__IMPORT", strFolderPath);

		uint32 uiImportedFileCount = 0;
		for (auto strEntryNameMissingFromIMG : vecEntryNamesMissingFromIMG)
		{
			string strEntryFilePath = strFolderPath + CFileUtility::getFileNameFromNameWithoutExtension(strFolderPath, strEntryNameMissingFromIMG);
			if (CFileUtility::doesFileExist(strEntryFilePath))
			{
				getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strEntryFilePath);
				uiImportedFileCount++;
			}
		}

		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_84", uiImportedFileCount, vecEntryNamesMissingFromIMG.size()));

		if (uiImportedFileCount > 0)
		{
			getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
		}
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestOrphanIDEEntriesNotInIMG");
}
void		CTaskDispatchManager::onRequestSettings(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSettings");
	getKGM()->getTaskManager()->onTaskPause();
	bool bSave = getKGM()->getGUIManager()->showSettingsDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (!bSave)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSettings", true);
		return;
	}

	getKGM()->getSettingsManager()->reloadSettings();
	
	getKGM()->getTaskManager()->onTaskEnd("onRequestSettings");
	if (getKGM()->getSettingsManager()->getSettingBool("NewLanguageApplied"))
	{
		exit(EXIT_SUCCESS);
	}
}
void		CTaskDispatchManager::onRequestReopen(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestReopen");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestReopen", true);
		return;
	}

	string strIMGPath = getKGM()->getEntryListTab()->getIMGFile()->getFilePath();
	if (onRequestClose2(false) == true)
	{
		// cancelled
		getKGM()->getTaskManager()->onTaskEnd("onRequestReopen", true);
		return;
	}
	getKGM()->getEntryListWindow()->removeTab(getKGM()->getEntryListTab());
	onRequestOpen2(strIMGPath);
	getKGM()->getTaskManager()->onTaskEnd("onRequestReopen");
}
void		CTaskDispatchManager::onRequestConvertDFFToRWVersion(CRWVersion *pRWVersion)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ConvertDFF_RWVersion(pRWVersion);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertDFFToRWVersion");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	CDFFConversionDialogData *pDFFConversionDialogData = getKGM()->getGUIManager()->showDFFConversionDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (!pDFFConversionDialogData->m_bConvert) // cancel button
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
		delete pDFFConversionDialogData;
		return;
	}

	bool bConvert2DFXFromIIIOrVCToSA = false;
	unordered_map<string, vector<CIDEEntry*>> umapIDEEntriesByModelName;
	vector<CIDEFormat*> veCIDEFormats;
	bool bSelectedDFFsContainIIIOrVC = false;

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
		delete pDFFConversionDialogData;
		return;
	}
	CIMGEntry *pIMGEntry = nullptr;

	////////////////
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (!CPathUtility::isModelExtension(CPathUtility::getFileExtension(pIMGEntry->getEntryName())))
		{
			continue;
		}

		if (pIMGEntry != nullptr && (pIMGEntry->getRWVersion()->doesGameUseVersion(PLATFORMED_GAME_PC_GTA_III) || pIMGEntry->getRWVersion()->doesGameUseVersion(PLATFORMED_GAME_PC_GTA_VC)))
		{
			bSelectedDFFsContainIIIOrVC = true;
			break;
		}
	}
	//////////

	if (bSelectedDFFsContainIIIOrVC && pRWVersion->doesGameUseVersion(PLATFORMED_GAME_PC_GTA_SA))
	{
		// The selected DFFs in the active IMG tab contains at least 1 DFF IMG entry with a RW version of III or VC, and the target RW version to convert to is SA.
		vector<uint32> vecExtendedLogLines_MissingObjectIds;
		bool bDidCancel = false;
		bConvert2DFXFromIIIOrVCToSA = getKGM()->getGUIManager()->showConfirmDialog("Do you want to also convert 2DFX sections in DFF files from GTA III/VC format to GTA SA format? (Requires IDE input)", "Convert 2DFX Too?", bDidCancel);
		if (bDidCancel)
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
			delete pDFFConversionDialogData;
			return;
		}

		if (bConvert2DFXFromIIIOrVCToSA)
		{
			getKGM()->getTaskManager()->onTaskPause();
			vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("CONVERT_DFF_RWVERSION_IDE"), "IDE");
			getKGM()->getTaskManager()->onTaskUnpause();
			if (vecIDEPaths.size() == 0)
			{
				getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
				return;
			}
			getKGM()->setLastUsedDirectory("CONVERT_DFF_RWVERSION_IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

			for (string strIDEPath : vecIDEPaths)
			{
				CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strIDEPath);
				if (pIDEFile->doesHaveError())
				{
					pIDEFile->unload();
					delete pIDEFile;
					continue;
				}
				veCIDEFormats.push_back(pIDEFile);

				unordered_map<uint32, std::string> umapIDEModelNamesByObjectId;
				for (auto it : pIDEFile->getSectionEntries())
				{
					for (CIDEEntry *pIDEEntry : it.second)
					{
						if (pIDEEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
						{
							CIDEEntry_Data *pIDEEntry_Data = (CIDEEntry_Data*)pIDEEntry;
							uint32 uiObjectId = pIDEEntry_Data->getObjectId();
							if (uiObjectId != -1)
							{
								string strModelName = pIDEEntry_Data->getModelName();
								if (strModelName != "")
								{
									umapIDEModelNamesByObjectId[uiObjectId] = strModelName;
								}
							}
						}
					}
				}

				for (CIDEEntry_2DFX_Light *pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_2DFX_Light>(IDE_SECTION_2DFX, _2DFX_LIGHT))
				{
					if (umapIDEModelNamesByObjectId.count(pIDEEntry->getObjectId()) == 0)
					{
						vecExtendedLogLines_MissingObjectIds.push_back(pIDEEntry->getObjectId());
					}
					else
					{
						string strIDEModelName = umapIDEModelNamesByObjectId[pIDEEntry->getObjectId()];
						umapIDEEntriesByModelName[CStringUtility::toUpperCase(strIDEModelName)].push_back(pIDEEntry);
					}
				}
				for (CIDEEntry_2DFX_Particle *pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_2DFX_Particle>(IDE_SECTION_2DFX, _2DFX_PARTICLE))
				{
					if (umapIDEModelNamesByObjectId.count(pIDEEntry->getObjectId()) == 0)
					{
						vecExtendedLogLines_MissingObjectIds.push_back(pIDEEntry->getObjectId());
					}
					else
					{
						string strIDEModelName = umapIDEModelNamesByObjectId[pIDEEntry->getObjectId()];
						umapIDEEntriesByModelName[CStringUtility::toUpperCase(strIDEModelName)].push_back(pIDEEntry);
					}
				}
				for (CIDEEntry_2DFX_Ped *pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_2DFX_Ped>(IDE_SECTION_2DFX, _2DFX_PED))
				{
					if (umapIDEModelNamesByObjectId.count(pIDEEntry->getObjectId()) == 0)
					{
						vecExtendedLogLines_MissingObjectIds.push_back(pIDEEntry->getObjectId());
					}
					else
					{
						string strIDEModelName = umapIDEModelNamesByObjectId[pIDEEntry->getObjectId()];
						umapIDEEntriesByModelName[CStringUtility::toUpperCase(strIDEModelName)].push_back(pIDEEntry);
					}
				}
				for (CIDEEntry_2DFX_Unknown1 *pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_2DFX_Unknown1>(IDE_SECTION_2DFX, _2DFX_UNKNOWN_1))
				{
					if (umapIDEModelNamesByObjectId.count(pIDEEntry->getObjectId()) == 0)
					{
						vecExtendedLogLines_MissingObjectIds.push_back(pIDEEntry->getObjectId());
					}
					else
					{
						string strIDEModelName = umapIDEModelNamesByObjectId[pIDEEntry->getObjectId()];
						umapIDEEntriesByModelName[CStringUtility::toUpperCase(strIDEModelName)].push_back(pIDEEntry);
					}
				}
				for (CIDEEntry_2DFX_SunReflection *pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_2DFX_SunReflection>(IDE_SECTION_2DFX, _2DFX_SUN_REFLECTION))
				{
					if (umapIDEModelNamesByObjectId.count(pIDEEntry->getObjectId()) == 0)
					{
						vecExtendedLogLines_MissingObjectIds.push_back(pIDEEntry->getObjectId());
					}
					else
					{
						string strIDEModelName = umapIDEModelNamesByObjectId[pIDEEntry->getObjectId()];
						umapIDEEntriesByModelName[CStringUtility::toUpperCase(strIDEModelName)].push_back(pIDEEntry);
					}
				}

			}
		}

		getKGM()->getEntryListTab()->log("[Convert 2DFX from IDE (GTA III/VC) to DFF (GTA SA)] IDE 2DFX object IDs entries not having an IDE entry linked by object ID:", true);
		vector<string> vecExtendedLogLines_MissingObjectIds2;
		for (uint32 uiValue : vecExtendedLogLines_MissingObjectIds)
		{
			vecExtendedLogLines_MissingObjectIds2.push_back(CStringUtility::toString(uiValue));
		}
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecExtendedLogLines_MissingObjectIds2, "\n"), true);
		vecExtendedLogLines_MissingObjectIds.clear();
		vecExtendedLogLines_MissingObjectIds2.clear();
	}

	pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion", true);
		delete pDFFConversionDialogData;
		for (auto pIDEFile : veCIDEFormats)
		{
			pIDEFile->unload();
			delete pIDEFile;
		}
		return;
	}

	pIMGEntry = nullptr;
	vector<string> vecConvertedDFFEntryNames;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (!CPathUtility::isModelExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()))))
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		//CDebugger::log("Converting DFF: " + pIMGEntry->m_strFileName);
		string strEntryData = pIMGEntry->getEntryData();
		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(strEntryData);
		if (pDFFFile->doesHaveError())
		{
			//CDebugger::log("CORRUPT REASON: " + pDFFFile->getCorruptReason());
			pDFFFile->unload();
			delete pDFFFile;
			continue;
		}

		vecConvertedDFFEntryNames.push_back(pIMGEntry->getEntryName());

		pDFFFile->setRWVersion(pRWVersion);

		if (pDFFConversionDialogData->m_ucPrelightningOption == 0) // keep prelightning
		{
		}
		else if (pDFFConversionDialogData->m_ucPrelightningOption == 1) // remove prelightning
		{
			pDFFFile->removePrelightning();
		}
		else if (pDFFConversionDialogData->m_ucPrelightningOption == 2) // adjust prelightning
		{
			if (pDFFConversionDialogData->m_ucAdjustPrelightningOption == 0) // fixed colour
			{
				pDFFFile->setPrelightningColour(
					pDFFConversionDialogData->m_ssAdjustColourValues[0],
					pDFFConversionDialogData->m_ssAdjustColourValues[1],
					pDFFConversionDialogData->m_ssAdjustColourValues[2],
					pDFFConversionDialogData->m_ssAdjustColourValues[3]
				);
			}
			else if (pDFFConversionDialogData->m_ucAdjustPrelightningOption == 1) // colour offset
			{
				pDFFFile->applyPrelightningColourOffset(
					pDFFConversionDialogData->m_ssAdjustColourValues[0],
					pDFFConversionDialogData->m_ssAdjustColourValues[1],
					pDFFConversionDialogData->m_ssAdjustColourValues[2],
					pDFFConversionDialogData->m_ssAdjustColourValues[3]
				);
			}
		}

		if (bConvert2DFXFromIIIOrVCToSA && pIMGEntry->getRWVersion() != nullptr && (pIMGEntry->getRWVersion()->doesGameUseVersion(PLATFORMED_GAME_PC_GTA_III) || pIMGEntry->getRWVersion()->doesGameUseVersion(PLATFORMED_GAME_PC_GTA_VC)))
		{
			// CIDEEntry_2DFX = umapIDEModelNames[modelName]->getSectionsByType(IDE_SECTION_2DFX)[0];

			string strModelNameUpper = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
			int a;
			if (umapIDEEntriesByModelName.count(strModelNameUpper) == 0)
			{
				// Coudn't find DFF model name (from IMG entry name) in IDE OBJS/TOBJ with any corresponding 2DFX entries.
			}
			else
			{
				vector<C2dEffect*> vec2dEffects;
				C2dEffect *p2dEffect;
				for (CIDEEntry *pIDEEntry : umapIDEEntriesByModelName[strModelNameUpper])
				{
					switch (((CIDEEntry_2DFX*)pIDEEntry)->get2DFXType())
					{
					case _2DFX_LIGHT:
					{
						CRWEntry_2dEffect_Light					*pDFFEntry_2dEffect_Light = new CRWEntry_2dEffect_Light;
						CIDEEntry_2DFX_Light					*pIDEEntry_2dEffect_Light = (CIDEEntry_2DFX_Light*) pIDEEntry;
						p2dEffect = pDFFEntry_2dEffect_Light;

						uint32 uiPackedColour =
							(pIDEEntry_2dEffect_Light->getColour().m_x << 24) |
							(pIDEEntry_2dEffect_Light->getColour().m_y << 16) |
							(pIDEEntry_2dEffect_Light->getColour().m_z << 8) |
							0xFF;

						uint32 uiNewCoronaShowMode = 0;
						uint8 ucFlags1 = 0;
						uint32 ucFlags2 = 0;
						switch (pIDEEntry_2dEffect_Light->getFlare())
						{
						case 0: // constantly lit
							uiNewCoronaShowMode = 0;
							ucFlags1 |= 32 | 64;
							break;
						case 1: // constantly lit at night
							uiNewCoronaShowMode = 0;
							ucFlags1 |= 64;
							break;


						case 2: // occasional flicker all time
						case 10: // random flashing
						case 4: // ~1 second flashes
						case 6: // ~2 seconds flashes
						case 8: // ~3 seconds flashes
							uiNewCoronaShowMode = 1;
							ucFlags1 |= 32 | 64;
							break;
						case 3: // occasional flicker at night
						case 5: // ~1 second flashes at night
						case 7: // ~2 seconds flashes at night
						case 9: // ~3 seconds flashes at night
						case 11: // random flicker at night
							uiNewCoronaShowMode = 1;
							ucFlags1 |= 64;
							break;


						case 12: // traffic light
							uiNewCoronaShowMode = 7;
							break;
						}

						uint32 ucFogType = 0; // 0, 1 or 2 (0 = none)
						if (pIDEEntry_2dEffect_Light->getIDEFlag() & 2)
						{
							ucFogType = 1;
							ucFlags1 |= 2;
						}
						if (pIDEEntry_2dEffect_Light->getIDEFlag() & 4)
						{
							ucFogType = 2;
							ucFlags1 |= 4;
						}

						bool bUpdateZPosAboveGround = true;
						if (pIDEEntry_2dEffect_Light->getIDEFlag() & 1) // bit 0 (value 1) = disable z-test for corona
						{
							bUpdateZPosAboveGround = false;
						}
						ucFlags2 |= bUpdateZPosAboveGround ? 4 : 0;

						CVector3D vecPosition = { pIDEEntry_2dEffect_Light->getPosition().m_x, pIDEEntry_2dEffect_Light->getPosition().m_y, pIDEEntry_2dEffect_Light->getPosition().m_z };

						pDFFEntry_2dEffect_Light->set2DFXType(_2DFX_LIGHT);
						pDFFEntry_2dEffect_Light->setDataSize(76);
						pDFFEntry_2dEffect_Light->setPosition(vecPosition);

						CVector3ui8 vecLookDirection;
						vecLookDirection.m_x = 0;
						vecLookDirection.m_y = 0;
						vecLookDirection.m_z = 0;

						CVector2ui8 vecPadding;
						vecPadding.m_x = 0;
						vecPadding.m_y = 0;

						pDFFEntry_2dEffect_Light->setCoronaFarClip(pIDEEntry_2dEffect_Light->getViewDistance());
						pDFFEntry_2dEffect_Light->setCoronaSize(pIDEEntry_2dEffect_Light->getCoronaSize());
						pDFFEntry_2dEffect_Light->setPointlightRange(pIDEEntry_2dEffect_Light->getOuterRange());
						pDFFEntry_2dEffect_Light->setShadowSize(1.0f); // ?
						pDFFEntry_2dEffect_Light->setCoronaTexName(pIDEEntry_2dEffect_Light->getCoronaTexture());
						pDFFEntry_2dEffect_Light->setShadowTexName(pIDEEntry_2dEffect_Light->getShadowTexture());
						pDFFEntry_2dEffect_Light->setCoronaEnableReflection(1); // ?
						pDFFEntry_2dEffect_Light->setCoronaFlareType(pIDEEntry_2dEffect_Light->getFlare());
						pDFFEntry_2dEffect_Light->setCoronaShowMode(uiNewCoronaShowMode);
						pDFFEntry_2dEffect_Light->setFlags1(ucFlags1); //pIDEEntry_2dEffect_Light->m_uiIDEFlag; // flags need converting..
						pDFFEntry_2dEffect_Light->setFlags2(ucFlags2); //pIDEEntry_2dEffect_Light->m_uiIDEFlag; // flags need converting..
						pDFFEntry_2dEffect_Light->setLookDirection(vecLookDirection); // only used if m_uiDataSize == 80, instead of 76
						pDFFEntry_2dEffect_Light->setPadding(vecPadding);
						pDFFEntry_2dEffect_Light->setShadowColorMultiplier(40); // ?
						pDFFEntry_2dEffect_Light->setShadowZDistance(10); // guessed //pIDEEntry_2dEffect_Light->m_fViewDistance > 255.0f ? 255.0f : (uint8) pIDEEntry_2dEffect_Light->m_fViewDistance; // is this correct?
						pDFFEntry_2dEffect_Light->setColor(uiPackedColour);
						break;
					}
					case _2DFX_PARTICLE:
					{
						CRWEntry_2dEffect_ParticleEffect		*pDFFEntry_2dEffect_Particle = new CRWEntry_2dEffect_ParticleEffect;
						CIDEEntry_2DFX_Particle					*pIDEEntry_2dEffect_Particle = (CIDEEntry_2DFX_Particle*)pIDEEntry;
						p2dEffect = pDFFEntry_2dEffect_Particle;

						CVector3D vecPosition = { pIDEEntry_2dEffect_Particle->getPosition().m_x, pIDEEntry_2dEffect_Particle->getPosition().m_y, pIDEEntry_2dEffect_Particle->getPosition().m_z };

						pDFFEntry_2dEffect_Particle->set2DFXType(_2DFX_PARTICLE);
						pDFFEntry_2dEffect_Particle->setDataSize(24);
						pDFFEntry_2dEffect_Particle->setPosition(vecPosition);

						//pDFFEntry_2dEffect_Particle->setParticleEffectName(pIDEEntry_2dEffect_Particle->getParticleType()); // needs convert from ID to string.. // todo
						break;
					}
					case _2DFX_PED:
					{
						CRWEntry_2dEffect_PedAttractor			*pDFFEntry_2DEffect_Ped = new CRWEntry_2dEffect_PedAttractor;
						CIDEEntry_2DFX_Ped						*pIDEEntry_2dEffect_Ped = (CIDEEntry_2DFX_Ped*)pIDEEntry;
						p2dEffect = pDFFEntry_2DEffect_Ped;

						CVector3D vecPosition = { pIDEEntry_2dEffect_Ped->getPosition().m_x, pIDEEntry_2dEffect_Ped->getPosition().m_y, pIDEEntry_2dEffect_Ped->getPosition().m_z };

						pDFFEntry_2DEffect_Ped->set2DFXType(_2DFX_PED_ATTRACTOR);
						pDFFEntry_2DEffect_Ped->setDataSize(56);
						pDFFEntry_2DEffect_Ped->setPosition(vecPosition);

						string strExternalScriptName = "";
						CVector3D
							vecRotation1 = { pIDEEntry_2dEffect_Ped->getPedRotation().m_x, pIDEEntry_2dEffect_Ped->getPedRotation().m_y, pIDEEntry_2dEffect_Ped->getPedRotation().m_z },
							vecRotation2 = { 0.0f, 0.0f, 0.0f },
							vecRotation3 = { 0.0f, 0.0f, 0.0f };

						pDFFEntry_2DEffect_Ped->setPedExistingProbability(pIDEEntry_2dEffect_Ped->getBehaviour()); // is this correct?
						pDFFEntry_2DEffect_Ped->setType(0); // ?
						pDFFEntry_2DEffect_Ped->setExternalScriptName(strExternalScriptName); // ?
						pDFFEntry_2DEffect_Ped->setNotUsed1(0);
						pDFFEntry_2DEffect_Ped->setNotUsed2(0);
						pDFFEntry_2DEffect_Ped->setUnknown1(0);
						pDFFEntry_2DEffect_Ped->setUnknown2(0);
						pDFFEntry_2DEffect_Ped->setRotation(0, vecRotation1);
						pDFFEntry_2DEffect_Ped->setRotation(1, vecRotation2);
						pDFFEntry_2DEffect_Ped->setRotation(2, vecRotation3);
						break;
					}
					case _2DFX_UNKNOWN_1:
						continue;
					case _2DFX_SUN_REFLECTION:
					{
						CRWEntry_2dEffect_SunGlare						*pDFFEntry_2DEffect_SunGlare = new CRWEntry_2dEffect_SunGlare;
						CIDEEntry_2DFX_SunReflection					*pIDEEntry_2dEffect_SunGlare = (CIDEEntry_2DFX_SunReflection*)pIDEEntry;
						p2dEffect = pDFFEntry_2DEffect_SunGlare;

						CVector3D vecPosition = { pIDEEntry_2dEffect_SunGlare->getPosition().m_x, pIDEEntry_2dEffect_SunGlare->getPosition().m_y, pIDEEntry_2dEffect_SunGlare->getPosition().m_z };

						pDFFEntry_2DEffect_SunGlare->set2DFXType(_2DFX_SUN_GLARE);
						pDFFEntry_2DEffect_SunGlare->setDataSize(0);
						pDFFEntry_2DEffect_SunGlare->setPosition(vecPosition);
						break;
					}
					default:
						continue;
					}
					vec2dEffects.push_back(p2dEffect);
				}
				pDFFFile->set2dEffects(vec2dEffects);
			}
		}

		string strFileData = pDFFFile->storeViaMemory();
		pDFFFile->unload();
		delete pDFFFile;

		pIMGEntry->setEntrySize(strFileData.length());
		pIMGEntry->setRWVersion(pRWVersion);
		
		pIMGEntry->setEntryData(strFileData);

		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
	}
	
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_95", vecConvertedDFFEntryNames.size(), (pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_96"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecConvertedDFFEntryNames, "\n"), true);

	if (pIMGEntry != nullptr)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}

	getKGM()->getEntryListTab()->checkForUnknownRWVersionEntries();

	delete pDFFConversionDialogData;
	for (auto pIDEFile : veCIDEFormats)
	{
		pIDEFile->unload();
		delete pIDEFile;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFToRWVersion");
	*/
}
void		CTaskDispatchManager::onRequestMissingTextures(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestMissingTextures");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures", true);
		return;
	}

	vector<CIMGEntry*> vecSelectedDFFs;
	vector<string> vecSelectedDFFsFilenames;
	CIMGEntry *pIMGEntry = nullptr;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		getKGM()->getTaskManager()->onTaskProgressTick();
		if (!CPathUtility::isModelExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()))))
		{
			continue;
		}

		vecSelectedDFFs.push_back(pIMGEntry);
		vecSelectedDFFsFilenames.push_back(pIMGEntry->getEntryName());
	}
	if (pIMGEntry == nullptr)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_38"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_38"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSING_TEXTURES_IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSING_TEXTURES_IDE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecTextureNames;
	for (auto strPath : vecPaths)
	{
		CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strPath);
		if(!pIDEFile->doesHaveError())
		{
			vecTextureNames = CVectorUtility::combineVectors(vecTextureNames, pIDEFile->getTXDNamesFromModelNames(vecSelectedDFFsFilenames));
		}
		pIDEFile->unload();
		delete pIDEFile;
	}

	vector<string> vecDFFTexturesMissingFromTXD;
	for (uint32 i = 0; i < vecSelectedDFFs.size(); i++)
	{
		CIMGEntry *pIMGEntry2 = getKGM()->getEntryListTab()->getIMGFile()->getEntryByNameWithoutExtension(vecTextureNames[i]);
		string strTXDContent;
		if (pIMGEntry2 == nullptr)
		{
			getKGM()->getTaskManager()->onTaskPause();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_39", vecSelectedDFFsFilenames[i]), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_39"), MB_OK);
			vector<string> vecPaths2 = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSING_TEXTURES_TXD"), "TXD", false);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (vecPaths2.size() == 0)
			{
				getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures", true);
				return;
			}
			getKGM()->setLastUsedDirectory("MISSING_TEXTURES_TXD", CPathUtility::getDirectory(vecPaths2[0]));

			strTXDContent = CFileUtility::getFileContent(vecPaths2[0]);
		}
		else
		{
			strTXDContent = pIMGEntry2->getEntryData();
		}

		string strDFFContent = vecSelectedDFFs[i]->getEntryData();

		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(strDFFContent);
		vector<string> vecDFFTextureNames;
		if(!pDFFFile->doesHaveError())
		{
			vecDFFTextureNames = pDFFFile->getTextureNames();
		}
		pDFFFile->unload();
		delete pDFFFile;

		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strTXDContent);
		vector<string> vecTXDTextureNames;
		if(!pTXDFile->doesHaveError())
		{
			vecTXDTextureNames = pTXDFile->getTextureNames();
		}
		pTXDFile->unload();
		delete pTXDFile;

		vecTXDTextureNames = CVectorUtility::toUpperCase(vecTXDTextureNames);
		for (auto strDFFTexture : vecDFFTextureNames)
		{
			if (std::find(vecTXDTextureNames.begin(), vecTXDTextureNames.end(), CStringUtility::toUpperCase(strDFFTexture)) == vecTXDTextureNames.end())
			{
				vecDFFTexturesMissingFromTXD.push_back(strDFFTexture);
			}
		}
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_97", vecDFFTexturesMissingFromTXD.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_98"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecDFFTexturesMissingFromTXD, "\n"), true);

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog("Missing Textures", "Textures missing:", "Texture Name", vecDFFTexturesMissingFromTXD, CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_4_InitialFilename", CPathUtility::replaceFileExtension(CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()), "txt").c_str()), "MISSINGTEXTURES");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestMissingTextures");
	*/
}
bool		CTaskDispatchManager::onRequestClose2(bool bCloseAll)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_Close2_CloseAll(bCloseAll);
	getKGM()->getTaskManager()->onTaskBegin("onRequestClose2");
	string strText = "";
	if (bCloseAll)
	{
		uint32 uiModifiedSinceRebuildCount = 0;
		for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
		{
			if (((CEntryListWindowTab*)pWindowTab)->getIMGModifiedSinceRebuild())
			{
				uiModifiedSinceRebuildCount++;
			}
		}
		if (uiModifiedSinceRebuildCount > 0)
		{
			strText = CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_Confirm_3_Message", uiModifiedSinceRebuildCount);
		}
	}
	else
	{
		if (getKGM()->getEntryListTab()->getIMGModifiedSinceRebuild())
		{
			strText = CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_Confirm_4_Message", CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str());
		}
	}

	if (strText == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestClose2", true);
		return false;
	}

	bool bDidCancel = false;
	getKGM()->getTaskManager()->onTaskPause();
	bool bResult = getKGM()->getGUIManager()->showConfirmDialog(strText, CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_3_Title"), bDidCancel);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (bDidCancel)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestClose2", true);
		return true;
	}
	if (!bResult)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestClose2", true);
		return false;
	}

	if (bCloseAll)
	{
		for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
		{
			if (((CEntryListWindowTab*)pWindowTab)->getIMGModifiedSinceRebuild())
			{
				((CEntryListWindowTab*)pWindowTab)->rebuild();
			}
		}
	}
	else
	{
		getKGM()->getEntryListTab()->rebuild();
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestClose2");
	return true;
}
void		CTaskDispatchManager::onRequestReplaceAllFromFolder(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestReplaceAllFromFolder");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestReplaceAllFromFolder", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_14"), getKGM()->getLastUsedDirectory("REPLACE_ALL_FOLDER"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestReplaceAllFromFolder", true);
		return;
	}
	strFolderPath = CPathUtility::addSlashToEnd(strFolderPath);
	getKGM()->setLastUsedDirectory("REPLACE_ALL_FOLDER", strFolderPath);

	vector<string> vecFileNames = CFileUtility::getFileNames(strFolderPath);
	vector<string> vecFilePaths;
	for (uint32 i = 0; i < vecFileNames.size(); i++)
	{
		vecFilePaths[i] = strFolderPath + vecFileNames[i];
	}

	vector<string> vecUniqueFilePaths = CPathUtility::getUniqueFilePaths(vecFilePaths, getKGM()->getEntryListTab()->getIMGFile()->getEntryNames());
	vector<string> vecReplaceFilePaths = CVectorUtility::getUniqueEntries(vecFilePaths, vecUniqueFilePaths);
	uint32 uiReplaceEntryCount = vecReplaceFilePaths.size();

	bool bOverwriteFiles = false;
	if (getKGM()->getSettingsManager()->getSettingBool("AskBeforeOverwritingFiles"))
	{
		getKGM()->getTaskManager()->onTaskPause();
		bOverwriteFiles = getKGM()->getGUIManager()->showConfirmDialog(CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_Confirm_2_Message", uiReplaceEntryCount), CLocalizationManager::getInstance()->getTranslatedText("Window_Confirm_2_Title"));
		getKGM()->getTaskManager()->onTaskUnpause();

		if (!bOverwriteFiles)
		{
			vecFilePaths = vecUniqueFilePaths;
		}
	}
	else
	{
		//vector<string> vecA = CVectorUtility::getUniqueEntries(getKGM()->getEntryListTab()->getIMGFile()->getEntryNamesUpper(), CPathUtility::getFileNamesFromFilePaths(vecPaths));

		vector<string> vecNewReplacedFilePaths;
		for (auto strFilePath : vecReplaceFilePaths)
		{
			bool bReplaceEntry = true;
			CIMGEntry *pIMGEntry = getKGM()->getEntryListTab()->getIMGFile()->getEntryByName(CPathUtility::getFileName(strFilePath));

			uint32
				uiExistingEntryFileCreationDate = pIMGEntry->getFileCreationDate(),
				uiNewEntryFileCreationDate = CFileUtility::getFileCreationDate(strFilePath);

			if (uiExistingEntryFileCreationDate < uiNewEntryFileCreationDate)
			{
				// the existing entry is older than the new entry

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteOlderFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}
			else if (uiExistingEntryFileCreationDate > uiNewEntryFileCreationDate)
			{
				// the existing entry is newer than the new entry

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteNewerFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}

			if (pIMGEntry->isProtectedEntry())
			{
				// the entry is protected

				if (getKGM()->getSettingsManager()->getSettingBool("OverwriteProtectedFiles"))
				{
					bReplaceEntry = true;
				}
				else
				{
					bReplaceEntry = false;
				}
			}

			if (bReplaceEntry)
			{
				vecNewReplacedFilePaths.push_back(strFilePath);
			}
		}
		vecFilePaths = CVectorUtility::combineVectors(vecUniqueFilePaths, vecNewReplacedFilePaths);
	}

	vector<string> vecReplacedEntryNames;
	getKGM()->getEntryListTab()->replace(vecFilePaths, vecReplacedEntryNames);

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("ReplacedEntries", vecReplacedEntryNames.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntriesForReplace"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecReplacedEntryNames, "\n"), true);

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestReplaceAllFromFolder");
}
void		CTaskDispatchManager::onRequestExportAllEntriesFromAllTabs(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportAllEntriesFromAllTabs");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabs", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_ALL_ALL"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabs", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_ALL_ALL", strPath);

	uint32 uiTotalEntryCount = 0;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		uiTotalEntryCount += ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTotalEntryCount);

	vector<string> vecIMGPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecIMGPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());

		((CEntryListWindowTab*)pWindowTab)->getIMGFile()->exportMultiple(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntries(), strPath);
		//pWindowTab->log("Exported all " + CStringUtility::toString(pWindowTab->getIMGFile()->m_vecEntries.size()) + " entr" + (pWindowTab->getIMGFile()->m_vecEntries.size() == 1 ? "y" : "ies") + ".");
	}

	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_6", uiTotalEntryCount, getKGM()->getEntryListWindow()->getEntryCount()));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_7"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecIMGPaths, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabs");
}
void		CTaskDispatchManager::onRequestExportEntriesViaIDEFileFromAllTabs(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportEntriesViaIDEFileFromAllTabs");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaIDEFileFromAllTabs", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("EXPORT_IDE_ALL__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause(); 
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaIDEFileFromAllTabs", true);
		return;
	}
	getKGM()->setLastUsedDirectory("EXPORT_IDE_ALL__IDE", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_IDE_ALL__DEST"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaIDEFileFromAllTabs", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_IDE_ALL__DEST", strPath);

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);
	
	uint32 uiTotalEntryCount = 0;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		uiTotalEntryCount += ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTotalEntryCount);

	uint32 uiTotalEntryExportedCount = 0;
	vector<string> vecIMGPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecIMGPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());

		vector<CIMGEntry*> vecIMGEntries;
		for (auto pIMGEntry : ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntries())
		{
			string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
			auto it = std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), strEntryNameWithoutExtension);
			if (it != vecEntryNamesWithoutExtension.end())
			{
				uiTotalEntryExportedCount++;
				vecIMGEntries.push_back(pIMGEntry);
			}

			getKGM()->getTaskManager()->onTaskProgressTick();
		}

		((CEntryListWindowTab*)pWindowTab)->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	}

	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_108", uiTotalEntryExportedCount, getKGM()->getEntryListWindow()->getEntryCount()));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_7"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecIMGPaths, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaIDEFileFromAllTabs");
}
void		CTaskDispatchManager::onRequestExportEntriesViaTextLinesFromAllTabs(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportEntriesViaTextLinesFromAllTabs");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaTextLinesFromAllTabs", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_3_Message"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaTextLinesFromAllTabs", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_TEXTLINES_ALL"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaTextLinesFromAllTabs", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_TEXTLINES_ALL", strPath);

	vector<string> vecEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		vecEntryNames.push_back(strLine);
	}
	vecEntryNames = CVectorUtility::toUpperCase(vecEntryNames);

	uint32 uiTotalEntryCount = 0;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		uiTotalEntryCount += ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTotalEntryCount);

	uint32 uiTotalEntryExportedCount = 0;
	vector<string> vecIMGPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecIMGPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());

		vector<CIMGEntry*> vecIMGEntries;
		for (auto pIMGEntry : ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntries())
		{
			string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
			auto it = std::find(vecEntryNames.begin(), vecEntryNames.end(), strEntryNameWithoutExtension);
			if (it != vecEntryNames.end())
			{
				uiTotalEntryExportedCount++;
				vecIMGEntries.push_back(pIMGEntry);
			}

			getKGM()->getTaskManager()->onTaskProgressTick();
		}

		((CEntryListWindowTab*)pWindowTab)->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	}

	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_8", uiTotalEntryExportedCount, getKGM()->getEntryListWindow()->getEntryCount()));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_7"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecIMGPaths, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportEntriesViaTextLinesFromAllTabs");
}
void		CTaskDispatchManager::onRequestImportViaFolder(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestImportViaFolder");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFolder", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_15"), getKGM()->getLastUsedDirectory("IMPORT_FOLDER"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFolder", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("IMPORT_FOLDER", strPath);

	vector<string> vecFileNames = CFileUtility::getFileNames(strPath);
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecFileNames.size());
	for (auto strFileName : vecFileNames)
	{
		getKGM()->getEntryListTab()->addOrReplaceEntryViaFileAndSettings(strPath + strFileName);
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_99", vecFileNames.size(), CPathUtility::getFolderName(strPath).c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_100"), true);
	getKGM()->getEntryListTab()->log(strPath, true);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("EntriesForImport"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecFileNames, "\n"), true);

	if (vecFileNames.size() > 0)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestImportViaFolder");
}
void		CTaskDispatchManager::onRequestDuplicateEntries(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestDuplicateEntries");

	// show window
	getKGM()->getTaskManager()->onTaskPause();
	CDuplicateEntriesDialogData *pDuplicateEntriesDialogData = getKGM()->getGUIManager()->showDuplicateEntriesDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	if (!pDuplicateEntriesDialogData->m_bCheck)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestDuplicateEntries", true);
		return;
	}

	// choose entries
	vector<CIMGFormat*> veCIMGFormats;
	if (pDuplicateEntriesDialogData->m_ucEntriesType == 0) // all entries in active tab
	{
		if (getKGM()->getEntryListTab() == nullptr)
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestDuplicateEntries", true);
			return;
		}

		veCIMGFormats.push_back(getKGM()->getEntryListTab()->getIMGFile());
	}
	else if (pDuplicateEntriesDialogData->m_ucEntriesType == 1) // selected entries in active tab
	{
		if (getKGM()->getEntryListTab() == nullptr)
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestDuplicateEntries", true);
			return;
		}

		veCIMGFormats.push_back(getKGM()->getEntryListTab()->getIMGFile());
	}
	else if (pDuplicateEntriesDialogData->m_ucEntriesType == 2) // all entries in all tabs
	{
		if (getKGM()->getEntryListTab() == nullptr)
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestDuplicateEntries", true);
			return;
		}

		veCIMGFormats = getKGM()->getEntryListWindow()->getAllMainWindowTabsIMGFiles();
	}
	else if (pDuplicateEntriesDialogData->m_ucEntriesType == 3) // DAT file
	{
		ePlatformedGame ePlatformedGameValue = PLATFORMED_GAME_UNKNOWN;
		switch (pDuplicateEntriesDialogData->m_uiDATGameIndex)
		{
		case 0: // GTA III
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_III;
			break;
		case 1: // GTA VC
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_VC;
			break;
		case 2: // GTA SA
			ePlatformedGameValue = PLATFORMED_GAME_PC_GTA_SA;
			break;
		case 3: // SOL
			ePlatformedGameValue = PLATFORMED_GAME_PC_SOL;
			break;
		case 4: // Other
			break;
		}
		string strDATPath = pDuplicateEntriesDialogData->m_strDATGameDirectoryPath + CDATLoaderManager::getDefaultGameDATSubPath(ePlatformedGameValue);

		CDATLoaderFormat *pDATFile = CDATLoaderManager::getInstance()->parseViaFile(strDATPath);
		if (!pDATFile->doesHaveError())
		{
			veCIMGFormats = pDATFile->parseIMGFiles(pDuplicateEntriesDialogData->m_strDATGameDirectoryPath);
		}
		pDATFile->unload();
		delete pDATFile;

		vector<string> vecGameIMGPaths = CIMGManager::getDefaultGameIMGSubPaths(ePlatformedGameValue);

		for (auto strIMGRelativePath : vecGameIMGPaths)
		{
			string strIMGPath = pDuplicateEntriesDialogData->m_strDATGameDirectoryPath + strIMGRelativePath;
			if (CFileUtility::doesFileExist(strIMGPath))
			{
				CIMGFormat *pIMGFile = CIMGManager::getInstance()->parseViaFile(strIMGPath);
				if(!pIMGFile->doesHaveError())
				{
					veCIMGFormats.push_back(pIMGFile);
				}
			}
		}
	}

	// max progress tick
	uint32 uiTickCount = 0;
	for (auto pIMGFile : veCIMGFormats)
	{
		uiTickCount += pIMGFile->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTickCount);

	// fetch selected entries
	vector<CIMGEntry*> vecSelectedIMGEntries;
	if (pDuplicateEntriesDialogData->m_ucEntriesType == 1) // selected entries
	{
		vecSelectedIMGEntries = getKGM()->getEntryListTab()->getSelectedEntries();
	}

	// find duplicate entries
	unordered_map < string, vector<CIMGEntry*> >
		umapIMGEntries;
	vector<string>
		vecEntryDuplicateNames;
	for (auto pIMGFile : veCIMGFormats)
	{
		// choose IMG entries
		vector<CIMGEntry*> vecIMGEntries;
		if (pDuplicateEntriesDialogData->m_ucEntriesType == 1) // selected entries
		{
			vecIMGEntries = vecSelectedIMGEntries;
		}
		else
		{
			vecIMGEntries = pIMGFile->getEntries();
		}

		// store IMG entry name for checking
		for (auto pIMGEntry : vecIMGEntries)
		{
			umapIMGEntries[CStringUtility::toUpperCase(pIMGEntry->getEntryName())].push_back(pIMGEntry);

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}
	for (auto it : umapIMGEntries)
	{
		vector<CIMGEntry*>& vecEntries = it.second;
		if (vecEntries.size() > 1)
		{
			vector<string> vecEntryIMGFileNames;
			for (auto pIMGEntry : vecEntries)
			{
				vecEntryIMGFileNames.push_back(CPathUtility::getFileName(pIMGEntry->getIMGFile()->getFilePath()));
			}
			vecEntryDuplicateNames.push_back(vecEntries[0]->getEntryName() + " (" + CStringUtility::join(vecEntryIMGFileNames, ", ") + ")");
		}
	}

	umapIMGEntries.clear();

	// log
	if (getKGM()->getEntryListTab())
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_102", vecEntryDuplicateNames.size()));
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_103"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryDuplicateNames, "\n"), true);
	}

	// results window
	string strSaveFileName;
	if (pDuplicateEntriesDialogData->m_ucEntriesType == 3) // DAT file
	{
		strSaveFileName = CLocalizationManager::getInstance()->getTranslatedText("SaveFilePopup_5_InitialFilename");
	}
	else
	{
		strSaveFileName = CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_6_InitialFilename", CPathUtility::replaceFileExtension(CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()), "txt").c_str());
	}
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog(CLocalizationManager::getInstance()->getTranslatedText("DuplicateEntries"), "Showing " + CStringUtility::toString(vecEntryDuplicateNames.size()) + " duplicate entr" + (vecEntryDuplicateNames.size() == 1 ? "y" : "ies") + ".", CLocalizationManager::getInstance()->getTranslatedText("Window_OrphanEntries_EntryName"), vecEntryDuplicateNames, strSaveFileName, "DUPLICATEENTRIES");
	getKGM()->getTaskManager()->onTaskUnpause();

	// clean up
	vecEntryDuplicateNames.clear();
	if (pDuplicateEntriesDialogData->m_ucEntriesType == 3) // DAT file
	{
		for (auto pIMGFile : veCIMGFormats)
		{
			pIMGFile->unload();
			delete pIMGFile;
		}
	}
	delete pDuplicateEntriesDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestDuplicateEntries");
}
void		CTaskDispatchManager::onRequestExportAllEntriesFromAllTabsIntoMultipleFolders(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportAllEntriesFromAllTabsIntoMultipleFolders");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabsIntoMultipleFolders", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_16"), getKGM()->getLastUsedDirectory("EXPORT_ALL_FOLDERS"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabsIntoMultipleFolders", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_ALL_FOLDERS", strPath);

	uint32 uiTotalEntryCount = 0;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		uiTotalEntryCount += ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntryCount();
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(uiTotalEntryCount);

	vector<string> vecIMGPaths;
	for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
	{
		vecIMGPaths.push_back(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath());

		((CEntryListWindowTab*)pWindowTab)->getIMGFile()->exportMultiple(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntries(), strPath + CPathUtility::getFileName(((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getFilePath()) + "/");
	}

	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedFormattedText("LogAllTabs_9", uiTotalEntryCount, getKGM()->getEntryListWindow()->getEntries()));
	getKGM()->getEntryListWindow()->logAllTabs(CLocalizationManager::getInstance()->getTranslatedText("LogAllTabs_7"), true);
	getKGM()->getEntryListWindow()->logAllTabs(CStringUtility::join(vecIMGPaths, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportAllEntriesFromAllTabsIntoMultipleFolders");
}
void		CTaskDispatchManager::onRequestOpenLast(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpenLast");
	uint32 uiRecentlyOpenedCount = CRegistryUtility::getSoftwareValueInt("KGM\\RecentlyOpened", "Count");
	if (uiRecentlyOpenedCount > 0)
	{
		string strIMGPath = CRegistryUtility::getSoftwareValueString("KGM\\RecentlyOpened", "Data_" + CStringUtility::toString(uiRecentlyOpenedCount));
		onRequestOpen2(strIMGPath);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestOpenLast");
}
void		CTaskDispatchManager::onRequestConvertTXDToGame(ePlatformedGame ePlatformedGame)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ConvertTXD_Game(ePlatformedGame);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertTXDToGame");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToGame", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToGame", true);
		return;
	}

	CIMGEntry *pIMGEntry = nullptr;
	uint32 uiConvertedTXDCount = 0;
	vector<string>
		vecConvertedTXDNames,
		vecMipmapsRemoved;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) != "TXD")
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pTXDFile->doesHaveError())
		{
			pTXDFile->unload();
			delete pTXDFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		uiConvertedTXDCount++;
		vecConvertedTXDNames.push_back(pIMGEntry->getEntryName());

		pTXDFile->convertToGame(ePlatformedGame, vecMipmapsRemoved);
		CRWVersion *pRWVersion = pTXDFile->getRWVersion();
		string strFileData = pTXDFile->serializeViaMemory();
		pTXDFile->unload();
		delete pTXDFile;
		
		pIMGEntry->setEntrySize(strFileData.length());
		pIMGEntry->setRWVersion(pRWVersion);
		
		pIMGEntry->setEntryData(strFileData);

		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
		
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_104", uiConvertedTXDCount, CRWManager::getInstance()->getGameName(ePlatformedGame).c_str(), vecMipmapsRemoved.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_105"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecConvertedTXDNames, "\n"), true);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("MipmapsRemoved"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecMipmapsRemoved, "\n"), true);

	if (pIMGEntry != nullptr)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToGame");
	*/
}
void		CTaskDispatchManager::onRequestConvertTXDToRWVersion(CRWVersion *pRWVersion)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ConvertTXD_RWVersion(pRWVersion);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertTXDToRWVersion");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToRWVersion", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToRWVersion", true);
		return;
	}

	CIMGEntry *pIMGEntry = nullptr;
	vector<string> vecConvertedTXDEntryNames;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) != "TXD")
		{
			continue;
		}

		string strEntryData = pIMGEntry->getEntryData();
		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strEntryData);
		if (pTXDFile->doesHaveError())
		{
			pTXDFile->unload();
			delete pTXDFile;
			continue;
		}

		vecConvertedTXDEntryNames.push_back(pIMGEntry->getEntryName());

		pTXDFile->setRWVersion(pRWVersion);
		string strFileData = pTXDFile->serializeViaMemory();
		uint32 uiFileSize = strFileData.length();
		pTXDFile->unload();
		delete pTXDFile;

		pIMGEntry->setEntrySize(uiFileSize);
		pIMGEntry->setRWVersion(pRWVersion);
		
		pIMGEntry->setEntryData(strFileData);
		
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_107", vecConvertedTXDEntryNames.size(), (pRWVersion->getVersionName() + " (" + CLocalizationManager::getInstance()->getTranslatedText(pRWVersion->getLocalizationKey()) + ")").c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_105"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecConvertedTXDEntryNames, "\n"), true);

	if (pIMGEntry != nullptr)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToRWVersion");
	*/
}
void		CTaskDispatchManager::onRequestDump(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestDump");
	getKGM()->getDumpManager()->process();
	getKGM()->getTaskManager()->onTaskEnd("onRequestDump");
}
void		CTaskDispatchManager::onRequestSessionManager(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSessionManager");
	CSessionManagerDialogData *pSessionManagerDialogData = nullptr;
	bool bReopenWindow;
	do
	{
		vector<string> vecSessionsData;
		uint32 uiSessionCount = CRegistryUtility::getSoftwareValueInt("KGM\\Sessions", "Count");
		for (int32 i = uiSessionCount; i >= 1; i--)
		{
			string strIMGPaths = CRegistryUtility::getSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(i));
			vecSessionsData.push_back(strIMGPaths);
		}

		getKGM()->getTaskManager()->onTaskPause();
		pSessionManagerDialogData = getKGM()->getGUIManager()->showSessionManagerDialog(vecSessionsData);
		getKGM()->getTaskManager()->onTaskUnpause();
		bReopenWindow = pSessionManagerDialogData->m_vecSessionsToRemove.size() > 0 || pSessionManagerDialogData->m_vecSessionsToAdd.size() > 0 || pSessionManagerDialogData->m_strSessionNameToUpdate != "";

		for (auto strSessionName : pSessionManagerDialogData->m_vecSessionsToRemove)
		{
			/*
			char zz[1024];
			sprintf_s(zz, "strSessionName: %s", strSessionName.c_str());
			CDebugger::log(zz);
			*/
			getKGM()->getSessionManager()->removeSession(getKGM()->getSessionManager()->getSessionByName(strSessionName));
		}
		for (auto strSessionData : pSessionManagerDialogData->m_vecSessionsToAdd)
		{
			deque<string> deqIMGPaths = CVectorUtility::convertVectorToDeque(CStringUtility::split(strSessionData, "; "));
			string strSessionName = deqIMGPaths[0];
			deqIMGPaths.pop_front();

			vector<string> vecIMGPaths;
			for (auto strIMGPath : deqIMGPaths)
			{
				vecIMGPaths.push_back(strIMGPath);
			}

			getKGM()->getSessionManager()->addSession(strSessionName, vecIMGPaths);
		}
		if (pSessionManagerDialogData->m_strSessionNameToUpdate != "")
		{
			//pSessionManagerDialogData->m_strSessionDataToUpdate
			deque<string> deqIMGPaths = CVectorUtility::convertVectorToDeque(CStringUtility::split(pSessionManagerDialogData->m_strSessionDataToUpdate, "; "));
			string strNewSessionName = deqIMGPaths[0];
			deqIMGPaths.pop_front();

			vector<string> vecIMGPaths;
			for (auto strIMGPath : deqIMGPaths)
			{
				vecIMGPaths.push_back(strIMGPath);
			}

			getKGM()->getSessionManager()->getSessionByName(pSessionManagerDialogData->m_strSessionNameToUpdate)->setName(strNewSessionName);
			getKGM()->getSessionManager()->getSessionByName(pSessionManagerDialogData->m_strSessionNameToUpdate)->setPaths(vecIMGPaths);
		}

		delete pSessionManagerDialogData;
		getKGM()->getSessionManager()->loadSessions();
	} while (bReopenWindow);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSessionManager");
}
void		CTaskDispatchManager::onRequestWebsite(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestWebsite");
	//ShellExecute(NULL, L"open", L"http://imgfactory.mvec.io/", NULL, NULL, SW_SHOWNORMAL);
	ShellExecute(NULL, L"open", L"http://kgm.io/", NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskEnd("onRequestWebsite");
}
void		CTaskDispatchManager::onRequestOpenLogBasic(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpenLogBasic");
	ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath") + CStringUtility::getDateTextForFolder() + " / " + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Basic")).c_str(), NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskEnd("onRequestOpenLogBasic");
}
void		CTaskDispatchManager::onRequestOpenLogExtended(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpenLogExtended");
	ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath") + CStringUtility::getDateTextForFolder() + " / " + CLocalizationManager::getInstance()->getTranslatedText("LogFilename_Extended")).c_str(), NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskEnd("onRequestOpenLogExtended");
}
void		CTaskDispatchManager::onRequestOpenLogFolder(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpenLogFolder");
	ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(getKGM()->getSettingsManager()->getSettingString("AutomaticLoggingPath")).c_str(), NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskEnd("onRequestOpenLogFolder");
}
void		CTaskDispatchManager::onRequestProcessLSTFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestProcessLSTFile");
	/*
	getKGM()->getTaskManager()->onTaskPause();
	string strGameDirectoryPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_3"), getKGM()->getLastUsedDirectory("PROCESS_LST__GAME"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strGameDirectoryPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestProcessLSTFile", true);
		return;
	}
	strGameDirectoryPath = CPathUtility::addSlashToEnd(strGameDirectoryPath);
	getKGM()->setLastUsedDirectory("PROCESS_LST__GAME", strGameDirectoryPath);
	*/

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("PROCESS_LST__LST"), "LST", false);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestProcessLSTFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("PROCESS_LST__LST", CPathUtility::getDirectory(vecPaths[0]));

	CLSTFormat *pLSTFile = CLSTManager::getInstance()->parseViaFile(vecPaths[0]);
	if(!pLSTFile->doesHaveError())
	{
		getKGM()->getLSTProcessingManager()->process(pLSTFile);
	}
	
	pLSTFile->unload();
	delete pLSTFile;
	
	getKGM()->getTaskManager()->onTaskEnd("onRequestProcessLSTFile");
}
void		CTaskDispatchManager::onRequestSelectViaIDE(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSelectViaIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaIDE", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("SELECT_IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SELECT_IDE", CPathUtility::getDirectory(vecPaths[0]));

	vector<string> vecEntryNamesWithoutExtension = CIDEManager::getIDEEntryNamesWithoutExtension(vecPaths);
	vecEntryNamesWithoutExtension = CVectorUtility::toUpperCase(vecEntryNamesWithoutExtension);

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	bool bSelectEntry;
	uint32 uiSelectedEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());
	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		CIMGEntry *pIMGEntry = (CIMGEntry*)pListControl->GetItemData(i);

		if (std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()))) != vecEntryNamesWithoutExtension.end())
		{
			bSelectEntry = true;
		}
		else
		{
			bSelectEntry = false;
		}

		if (bSelectEntry)
		{
			uiSelectedEntryCount++;
			pListControl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			pListControl->SetSelectionMark(i);
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	getKGM()->getEntryListTab()->log("Selected " + CStringUtility::toString(uiSelectedEntryCount) + " entr" + (uiSelectedEntryCount == 1 ? "y" : "ies") + " (vie IDE file).");

	pListControl->SetFocus();
	getKGM()->getTaskManager()->onTaskEnd("onRequestSelectViaIDE");
	*/
}
void		CTaskDispatchManager::onRequestExportViaIPLFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportViaIPLFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIPLFile", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("EXPORT_IPL__IPL"), "IPL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIPLFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("EXPORT_IPL__IPL", CPathUtility::getDirectory(vecPaths[0]));

	getKGM()->getTaskManager()->onTaskPause();
	string strPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORT_IPL__DESTINATION"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIPLFile", true);
		return;
	}
	strPath = CPathUtility::addSlashToEnd(strPath);
	getKGM()->setLastUsedDirectory("EXPORT_IPL__DESTINATION", strPath);

	vector<string> vecEntryNamesWithoutExtension;
	for (auto strPath : vecPaths)
	{
		CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strPath);
		if(!pIPLFile->doesHaveError())
		{
			vector<string> vecModelNames = pIPLFile->getModelNames();
			vecEntryNamesWithoutExtension = CVectorUtility::combineVectors(vecEntryNamesWithoutExtension, CVectorUtility::toUpperCase(vecModelNames));
		}
		pIPLFile->unload();
		delete pIPLFile;
	}

	vector<CIMGEntry*> vecIMGEntries;
	vector<string> vecIMGEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		auto it = std::find(vecEntryNamesWithoutExtension.begin(), vecEntryNamesWithoutExtension.end(), strEntryNameWithoutExtension);
		if (it != vecEntryNamesWithoutExtension.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
			vecIMGEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}
	
	for (auto pIMGEntry : vecIMGEntries)
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_109", pIMGEntry->getEntryName().c_str()), true);
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size());
	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntries, strPath);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_110", vecIMGEntries.size(), getKGM()->getEntryListTab()->getIMGFile()->getEntryCount()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_77"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecIMGEntryNames, "\n"), true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaIPLFile");
}
void		CTaskDispatchManager::onRequestRenameIMG(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestRenameIMG");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenameIMG", true);
		return;
	}

	string strCurrentIMGFileName = CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath());
	getKGM()->getTaskManager()->onTaskPause();
	string strNewIMGFileName = getKGM()->getGUIManager()->showTextInputDialog("Rename IMG", CLocalizationManager::getInstance()->getTranslatedText("Window_TextInput_6_Message"), strCurrentIMGFileName);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strNewIMGFileName == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenameIMG", true);
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(5);

	string strFolderPath = CPathUtility::addSlashToEnd(CPathUtility::removeFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()));
	CFileUtility::renameFile(strFolderPath + strCurrentIMGFileName, strFolderPath + strNewIMGFileName);
	getKGM()->getTaskManager()->onTaskProgressTick();

	if (getKGM()->getEntryListTab()->getIMGFile()->getIMGVersion() == IMG_1)
	{
		CFileUtility::renameFile(CPathUtility::replaceFileExtension(strFolderPath + strCurrentIMGFileName, "dir"), CPathUtility::replaceFileExtension(strFolderPath + strNewIMGFileName, "dir"));
	}
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getEntryListTab()->getIMGFile()->setFilePath(strFolderPath + strNewIMGFileName);
	getKGM()->getTaskManager()->onTaskProgressTick();

	((CEdit*)getKGM()->getDialog()->GetDlgItem(38))->SetWindowTextW(CStringUtility::convertStdStringToStdWString(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()).c_str());
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getEntryListTab()->searchText();
	getKGM()->getTaskManager()->onTaskProgressTick();

	wstring wstrNewIMGFileName = CStringUtility::convertStdStringToStdWString(strNewIMGFileName);
	TCITEM ltag;
	ltag.mask = TCIF_TEXT;
	ltag.pszText = &wstrNewIMGFileName[0];
	((CTabCtrl*)getKGM()->getDialog()->GetDlgItem(1))->SetItem(getKGM()->getEntryListTab()->getIndex(), &ltag);

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_111", strCurrentIMGFileName.c_str(), strNewIMGFileName.c_str()));
	getKGM()->getTaskManager()->onTaskEnd("onRequestRenameIMG");
	*/
}
void		CTaskDispatchManager::onRequestUpdate(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestUpdate");

	//vector<CUpdateConnection*> vecUpdateConnections;
	if (getKGM()->getBuildMeta().isAlphaBuild())
	{
		// alpha version
		string strFileContent;
		CUpdateConnection *pActiveUpdateConnection = nullptr;
		for (auto pUpdateConnection : getKGM()->getUpdateManager()->getUpdateConnectionManager()->getEntries())
		{
			if (!pUpdateConnection->isAlpha())
			{
				continue;
			}

			pActiveUpdateConnection = pUpdateConnection;
			strFileContent = CHTTPManager::getInstance()->getFileContent(pUpdateConnection->getLatestVersionURL());
			if (strFileContent == "")
			{
				continue;
			}

			break;
		}
		if (strFileContent == "")
		{
			getKGM()->getTaskManager()->onTaskPause();
			uint32 uiMirrorCount = getKGM()->getUpdateManager()->getUpdateConnectionManager()->getEntryCount();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_40", uiMirrorCount), CLocalizationManager::getInstance()->getTranslatedText("UnableToCheckForUpdates"), MB_OK);
			getKGM()->getTaskManager()->onTaskUnpause();
			getKGM()->getTaskManager()->onTaskEnd("onRequestUpdate", true);
			return;
		}

		vector<string> vecData = CStringUtility::split(strFileContent, "\n");
		string strLatestVersion = vecData[0];
		string strLatestVersionFileName = vecData[1];
		string strLatestBuildNumber = vecData[2];

		uint32 uiLatestBuildNumber = CStringUtility::toUint32(strLatestBuildNumber);

		if (uiLatestBuildNumber > BUILDNUMBER)
		{
			getKGM()->getTaskManager()->onTaskPause();
			uint32 uiResult = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_41", strLatestVersion.c_str(), strLatestBuildNumber.c_str(), getKGM()->getBuildMeta().getCurrentVersionString().c_str(), BUILDNUMBER_STR), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_41"), MB_OKCANCEL);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (uiResult == IDOK)
			{
				string strNewProgramData = CHTTPManager::getInstance()->getFileContent(pActiveUpdateConnection->getDownloadFolderURL() + strLatestVersionFileName);

				TCHAR szFilename[MAX_PATH];
				GetModuleFileName(NULL, szFilename, MAX_PATH);
				string strRunningProgramPath = CStringUtility::convertStdWStringToStdString(szFilename);

				//CFileUtility::overwriteLockedFileViaContent(strRunningProgramPath, strLatestVersionFileName, strNewProgramData);

				string strLockedFileDirectory = CPathUtility::getDirectory(strRunningProgramPath);
				string strNewProgramPath = strLockedFileDirectory + strLatestVersionFileName;
				//rename(strRunningProgramPath.c_str(), (strLockedFileDirectory + "Temp File").c_str());
				strNewProgramPath = CFileUtility::getNextIncrementingFileName(strNewProgramPath);
				CFileUtility::storeFile(strNewProgramPath, strNewProgramData, false, true);

				// delete previous version's exe file
				if (getKGM()->getSettingsManager()->getSettingBool("RemoveOldVersionOnUpdate"))
				{
					TCHAR szModuleName[MAX_PATH];
					GetModuleFileName(NULL, szModuleName, MAX_PATH);
					string strExePath = CStringUtility::convertStdWStringToStdString(szModuleName);
					if (strNewProgramPath != strExePath)
					{
						CRegistryUtility::setSoftwareValueString("KGM\\InternalSettings", "DeletePreviousVersionOnNextLaunch", strExePath);
					}
				}

				getKGM()->getTaskManager()->onTaskPause();
				uint32 uiResult2 = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_42"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_42"), MB_OKCANCEL);
				getKGM()->getTaskManager()->onTaskUnpause();
				if (uiResult2 == IDOK)
				{
					ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strNewProgramPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
				}

				if (uiResult2 == IDOK)
				{
					getKGM()->getTaskManager()->onTaskEnd("onRequestUpdate");
					ExitProcess(0);
				}
			}
		}
		else
		{
			getKGM()->getTaskManager()->onTaskPause();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_43", getKGM()->getBuildMeta().getCurrentVersionString().c_str(), BUILDNUMBER_STR), CLocalizationManager::getInstance()->getTranslatedText("UpToDate"), MB_OK);
			getKGM()->getTaskManager()->onTaskUnpause();
		}
	}
	else
	{
		// non-alpha version
		string strFileContent;
		CUpdateConnection *pActiveUpdateConnection = nullptr;
		for (auto pUpdateConnection : getKGM()->getUpdateManager()->getUpdateConnectionManager()->getEntries())
		{
			if (pUpdateConnection->isAlpha())
			{
				continue;
			}

			pActiveUpdateConnection = pUpdateConnection;
			strFileContent = CHTTPManager::getInstance()->getFileContent(pUpdateConnection->getLatestVersionURL());
			if (strFileContent == "")
			{
				continue;
			}

			break;
		}
		if (strFileContent == "")
		{
			getKGM()->getTaskManager()->onTaskPause();
			uint32 uiMirrorCount = getKGM()->getUpdateManager()->getUpdateConnectionManager()->getEntryCount();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_44", uiMirrorCount), CLocalizationManager::getInstance()->getTranslatedText("UnableToCheckForUpdates"), MB_OK);
			getKGM()->getTaskManager()->onTaskUnpause();
			getKGM()->getTaskManager()->onTaskEnd("onRequestUpdate", true);
			return;
		}

		vector<string> vecData = CStringUtility::split(CHTTPManager::getInstance()->getFileContent(pActiveUpdateConnection->getLatestVersionURL()), "\n");
		string strLatestVersion = vecData[0];
		string strLatestVersionFileName = vecData[1];

		float32 fLatestVersion = CStringUtility::toFloat32(vecData[0]);

		if (fLatestVersion > getKGM()->getBuildMeta().getCurrentVersion())
		{
			getKGM()->getTaskManager()->onTaskPause();
			uint32 uiResult = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_45", strLatestVersion.c_str(), getKGM()->getBuildMeta().getCurrentVersionString().c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_45"), MB_OKCANCEL);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (uiResult == IDOK)
			{
				string strNewProgramData = CHTTPManager::getInstance()->getFileContent(pActiveUpdateConnection->getDownloadFolderURL() + strLatestVersionFileName);

				TCHAR szFilename[MAX_PATH];
				GetModuleFileName(NULL, szFilename, MAX_PATH);
				string strRunningProgramPath = CStringUtility::convertStdWStringToStdString(szFilename);

				//CFileUtility::overwriteLockedFileViaContent(strRunningProgramPath, strLatestVersionFileName, strNewProgramData);

				string strLockedFileDirectory = CPathUtility::getDirectory(strRunningProgramPath);
				string strNewProgramPath = strLockedFileDirectory + strLatestVersionFileName;
				//rename(strRunningProgramPath.c_str(), (strLockedFileDirectory + "Temp File").c_str());
				CFileUtility::storeFile(strNewProgramPath, strNewProgramData, false, true);

				// delete previous version's exe file
				if (getKGM()->getSettingsManager()->getSettingBool("RemoveOldVersionOnUpdate"))
				{
					TCHAR szModuleName[MAX_PATH];
					GetModuleFileName(NULL, szModuleName, MAX_PATH);
					string strExePath = CStringUtility::convertStdWStringToStdString(szModuleName);
					if (strNewProgramPath != strExePath)
					{
						CRegistryUtility::setSoftwareValueString("KGM\\InternalSettings", "DeletePreviousVersionOnNextLaunch", strExePath);
					}
				}

				getKGM()->getTaskManager()->onTaskPause();
				uint32 uiResult2 = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_42"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_42"), MB_OKCANCEL);
				getKGM()->getTaskManager()->onTaskUnpause();
				if (uiResult2 == IDOK)
				{
					ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strNewProgramPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
				}

				if (uiResult2 == IDOK)
				{
					getKGM()->getTaskManager()->onTaskEnd("onRequestUpdate");
					ExitProcess(0);
				}
			}
		}
		else
		{
			getKGM()->getTaskManager()->onTaskPause();
			CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_46", getKGM()->getBuildMeta().getCurrentVersionString().c_str()), CLocalizationManager::getInstance()->getTranslatedText("UpToDate"), MB_OK);
			getKGM()->getTaskManager()->onTaskUnpause();
		}
	}

	getKGM()->getTaskManager()->onTaskEnd("onRequestUpdate");
}
void		CTaskDispatchManager::onRequestAutoUpdate(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestAutoUpdate");
	vector<string> vecData = CStringUtility::split(CHTTPManager::getInstance()->getFileContent("http://updater.imgfactory.mvec.io/latest-version.txt"), "\n");
	if (vecData.size() == 0)
	{
		// e.g. no network connection
		getKGM()->getTaskManager()->onTaskEnd("onRequestAutoUpdate", true);
		return;
	}
	string strLatestVersion = vecData[0];
	string strLatestVersionFileName = vecData[1];

	float32 fLatestVersion = CStringUtility::toFloat32(vecData[0]);

	if (fLatestVersion > getKGM()->getBuildMeta().getCurrentVersion())
	{
		getKGM()->getTaskManager()->onTaskPause();
		uint32 uiResult = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_45", strLatestVersion.c_str(), getKGM()->getBuildMeta().getCurrentVersionString().c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_45"), MB_OKCANCEL);
		getKGM()->getTaskManager()->onTaskUnpause();
		if (uiResult == IDOK)
		{
			string strNewProgramData = CHTTPManager::getInstance()->getFileContent("http://updater.imgfactory.mvec.io/versions/" + strLatestVersionFileName);

			TCHAR szFilename[MAX_PATH];
			GetModuleFileName(NULL, szFilename, MAX_PATH);
			string strRunningProgramPath = CStringUtility::convertStdWStringToStdString(szFilename);

			//CFileUtility::overwriteLockedFileViaContent(strRunningProgramPath, strLatestVersionFileName, strNewProgramData);

			string strLockedFileDirectory = CPathUtility::getDirectory(strRunningProgramPath);
			string strNewProgramPath = strLockedFileDirectory + strLatestVersionFileName;
			//rename(strRunningProgramPath.c_str(), (strLockedFileDirectory + "Temp File").c_str());
			CFileUtility::storeFile(strNewProgramPath, strNewProgramData, false, true);

			// delete previous version's exe file
			if (getKGM()->getSettingsManager()->getSettingBool("RemoveOldVersionOnUpdate"))
			{
				TCHAR szModuleName[MAX_PATH];
				GetModuleFileName(NULL, szModuleName, MAX_PATH);
				string strExePath = CStringUtility::convertStdWStringToStdString(szModuleName);
				CRegistryUtility::setSoftwareValueString("KGM\\InternalSettings", "DeletePreviousVersionOnNextLaunch", strExePath);
			}

			getKGM()->getTaskManager()->onTaskPause();
			uint32 uiResult2 = CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_42"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_42"), MB_OKCANCEL);
			getKGM()->getTaskManager()->onTaskUnpause();
			if (uiResult2 == IDOK)
			{
				ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strNewProgramPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
			}

			// close the currently open program
			if (uiResult2 == IDOK)
			{
				getKGM()->getTaskManager()->onTaskEnd("onRequestAutoUpdate");
				ExitProcess(0);
			}
		}
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestAutoUpdate");
}
void		CTaskDispatchManager::onRequestSaveIMGSignature(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSaveIMGSignature");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSaveIMGSignature", true);
		return;
	}

	CIMGFormat *pIMGFile = getKGM()->getEntryListTab()->getIMGFile();
	CDBFormat *pDBFile = CDBManager::getInstance()->createDBFileFromIMGFile(pIMGFile);
	//CDBFormat *pDBFile = getKGM()->getEntryListTab()->m_pDBFile;
	string strDBPath = CPathUtility::replaceFileExtension(pIMGFile->getFilePath(), "db");
	pDBFile->storeViaFile(strDBPath);

	if (getKGM()->getEntryListTab()->m_pDBFile != nullptr)
	{
		delete getKGM()->getEntryListTab()->m_pDBFile;
	}
	getKGM()->getEntryListTab()->m_pDBFile = pDBFile;
	getKGM()->getEntryListTab()->loadProtectedEntryStates();
	getKGM()->getEntryListTab()->readdAllEntriesToMainListView();

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_114", CPathUtility::getFileName(strDBPath).c_str()));
	getKGM()->getTaskManager()->onTaskEnd("onRequestSaveIMGSignature");
}
void		CTaskDispatchManager::onRequestVerifyIMGSignature(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestVerifyIMGSignature");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestVerifyIMGSignature", true);
		return;
	}

	CIMGFormat *pIMGFile = getKGM()->getEntryListTab()->getIMGFile();
	CDBFormat *pDBFileForIMGTab = CDBManager::getInstance()->createDBFileFromIMGFile(pIMGFile);

	string strDBPath = CPathUtility::replaceFileExtension(pIMGFile->getFilePath(), "db");
	CDBFormat *pDBFileForIMGFile = CDBManager::getInstance()->parseViaFile(strDBPath);
	
	if(pDBFileForIMGFile->doesHaveError())
	{
		delete pDBFileForIMGTab;
		delete pDBFileForIMGFile;
		getKGM()->getTaskManager()->onTaskEnd("onRequestVerifyIMGSignature", true);
		return;
	}
	
	if (!CFileUtility::doesFileExist(strDBPath))
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_54", CPathUtility::getFileName(strDBPath).c_str(), CPathUtility::getDirectory(strDBPath).c_str()), CLocalizationManager::getInstance()->getTranslatedText("FileNotFound"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	else if (CDBManager::getInstance()->compareDBFiles(pDBFileForIMGTab, pDBFileForIMGFile))
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_55", CPathUtility::getFileName(pIMGFile->getFilePath()).c_str(), CPathUtility::getFileName(strDBPath).c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_55"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	else
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_56", CPathUtility::getFileName(pIMGFile->getFilePath()).c_str(), CPathUtility::getFileName(strDBPath).c_str()), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_56"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
	}

	delete pDBFileForIMGTab;
	delete pDBFileForIMGFile;
	getKGM()->getTaskManager()->onTaskEnd("onRequestVerifyIMGSignature");
}
void		CTaskDispatchManager::onRequestCompareIMG(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestCompareIMG");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("COMPAREIMG"), "IMG", false);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}
	getKGM()->setLastUsedDirectory("COMPAREIMG", CPathUtility::getDirectory(vecPaths[0]));

	CIMGFormat *pIMGFile1 = getKGM()->getEntryListTab()->getIMGFile();

	eIMGVersion eIMGVersionValue = CIMGManager::detectIMGVersion(vecPaths[0]);
	/*
	todo
	if (uiFileResult == FILE_NOT_FOUND)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_32"), CLocalizationManager::getInstance()->getTranslatedText("FileNotFound"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}
	else if (uiFileResult == FILE_BLANK)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_33"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_33"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}
	else if (uiFileResult == FILE_UNKNOWN2)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_34"), CLocalizationManager::getInstance()->getTranslatedText("UnableToOpenIMG"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}
	*/
	
	CIMGFormat *pIMGFile2 = CIMGManager::getInstance()->parseViaFile(vecPaths[0]/* todo ?? -, (eIMGVersion)uiFileResult */);
	if (pIMGFile2->doesHaveError())
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedFormattedText("TextPopup_47", CPathUtility::getFileName(vecPaths[0]).c_str()), CLocalizationManager::getInstance()->getTranslatedText("UnableToOpenIMG"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
		
		pIMGFile2->unload();
		delete pIMGFile2;
		
		getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG", true);
		return;
	}

	vector<string>
		vecEntriesInFile1NotInFile2,
		vecEntriesInFile2NotInFile1,
		vecEntriesInBothFiles;

	for (auto pIMGEntry : pIMGFile1->getEntries())
	{
		if (pIMGFile2->getEntryByName(pIMGEntry->getEntryName()) == nullptr)
		{
			vecEntriesInFile1NotInFile2.push_back(pIMGEntry->getEntryName());
		}
		else
		{
			vecEntriesInBothFiles.push_back(pIMGEntry->getEntryName());
		}
	}
	for (auto pIMGEntry : pIMGFile2->getEntries())
	{
		if (pIMGFile1->getEntryByName(pIMGEntry->getEntryName()) == nullptr)
		{
			vecEntriesInFile2NotInFile1.push_back(pIMGEntry->getEntryName());
		}
	}
	vecEntriesInFile1NotInFile2 = CVectorUtility::removeDuplicates(vecEntriesInFile1NotInFile2);
	vecEntriesInFile2NotInFile1 = CVectorUtility::removeDuplicates(vecEntriesInFile2NotInFile1);
	vecEntriesInBothFiles = CVectorUtility::removeDuplicates(vecEntriesInBothFiles);

	vector<string> vecLines;
	uint32
		uiEntryCountInFile1NotInFile2,
		uiEntryCountInFile2NotInFile1,
		uiEntryCountInBothFiles;
	if (vecEntriesInFile1NotInFile2.size() > 0)
	{
		vecLines.push_back("Entries in " + CPathUtility::getFileName(pIMGFile1->getFilePath()) + " not in " + CPathUtility::getFileName(pIMGFile2->getFilePath()) + ":");
		for (auto strEntryName : vecEntriesInFile1NotInFile2)
		{
			vecLines.push_back(strEntryName);
		}
	}
	uiEntryCountInFile1NotInFile2 = vecEntriesInFile1NotInFile2.size();
	vecEntriesInFile1NotInFile2.clear();
	if (vecEntriesInFile2NotInFile1.size() > 0)
	{
		vecLines.push_back("Entries in " + CPathUtility::getFileName(pIMGFile2->getFilePath()) + " not in " + CPathUtility::getFileName(pIMGFile1->getFilePath()) + ":");
		for (auto strEntryName : vecEntriesInFile2NotInFile1)
		{
			vecLines.push_back(strEntryName);
		}
	}
	uiEntryCountInFile2NotInFile1 = vecEntriesInFile2NotInFile1.size();
	vecEntriesInFile2NotInFile1.clear();

	if (vecEntriesInBothFiles.size() > 0)
	{
		vecLines.push_back("Entries in " + CPathUtility::getFileName(pIMGFile1->getFilePath()) + " also in " + CPathUtility::getFileName(pIMGFile2->getFilePath()) + ":");
		for (auto strEntryName : vecEntriesInBothFiles)
		{
			vecLines.push_back(strEntryName);
		}
	}
	uiEntryCountInBothFiles = vecEntriesInBothFiles.size();
	vecEntriesInBothFiles.clear();

	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog(
		"Compare IMG",
		CStringUtility::toString(uiEntryCountInFile1NotInFile2) + " entr" + (uiEntryCountInFile1NotInFile2 == 1 ? "y" : "ies") + " not in " + CPathUtility::getFileName(pIMGFile2->getFilePath()) + ", "
		+ CStringUtility::toString(uiEntryCountInFile2NotInFile1) + " entr" + (uiEntryCountInFile2NotInFile1 == 1 ? "y" : "ies") + " not in " + CPathUtility::getFileName(pIMGFile1->getFilePath()) + ", "
		+ CStringUtility::toString(uiEntryCountInBothFiles) + " entr" + (uiEntryCountInBothFiles == 1 ? "y" : "ies") + " in both files",
		CLocalizationManager::getInstance()->getTranslatedText("Window_OrphanEntries_EntryName"),
		vecLines,
		CLocalizationManager::getInstance()->getTranslatedText("SaveFilePopup_7_InitialFilename"),
		"COMPAREIMG__SAVE"
	);
	getKGM()->getTaskManager()->onTaskUnpause();

	pIMGFile2->unload();
	delete pIMGFile2;
	getKGM()->getTaskManager()->onTaskEnd("onRequestCompareIMG");
}

void			CTaskDispatchManager::onRequestConvertTXDToTextureFormat(CRasterDataFormat *pRasterDataFormat)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ConvertTXD_TextureFormat(pRasterDataFormat);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertTXDToTextureFormat");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToTextureFormat", true);
		return;
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToTextureFormat", true);
		return;
	}

	CIMGEntry *pIMGEntry = nullptr;
	vector<string>
		vecConvertedTXDEntryNames,
		vecMipmapsRemoved;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) != "TXD")
		{
			continue;
		}

		string strEntryData = pIMGEntry->getEntryData();
		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strEntryData);
		if (pTXDFile->doesHaveError())
		{
			pTXDFile->unload();
			delete pTXDFile;
			continue;
		}

		vecConvertedTXDEntryNames.push_back(pIMGEntry->getEntryName());

		//pTXDFile->convertToTextureFormat(pTextureFormat, vecMipmapsRemoved);
		pTXDFile->convertToRasterDataFormat(pRasterDataFormat->getRasterDataFormatId(), vecMipmapsRemoved);

		string strFileData = pTXDFile->serializeViaMemory();
		uint32 uiFileSize = strFileData.length();
		pTXDFile->unload();
		delete pTXDFile;

		pIMGEntry->setEntrySize(uiFileSize);
		pIMGEntry->setEntryData(strFileData);

		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_115", vecConvertedTXDEntryNames.size(), CLocalizationManager::getInstance()->getTranslatedText(pRasterDataFormat->getLocalizationKey()).c_str(), vecMipmapsRemoved.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_105"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecConvertedTXDEntryNames, "\n"), true);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("MipmapsRemoved"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecMipmapsRemoved, "\n"), true);

	if (pIMGEntry != nullptr)
	{
		getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertTXDToTextureFormat");
	*/
}

void			CTaskDispatchManager::onRequestClearLogs(bool bAllTabs)
{
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ClearLogs_AllTabs(bAllTabs);
	getKGM()->getTaskManager()->onTaskBegin("onRequestClearLogs");
	vector<CEntryListWindowTab*> vecEntryListWindowTabs;
	if (bAllTabs)
	{
		for (auto pEntryListWindowTab : getKGM()->getEntryListWindow()->getEntries())
		{
			vecEntryListWindowTabs.push_back((CEntryListWindowTab*)pEntryListWindowTab);
		}
	}
	else
	{
		vecEntryListWindowTabs.push_back(getKGM()->getEntryListTab());
	}

	for (auto pWindowTab : vecEntryListWindowTabs)
	{
		pWindowTab->clearLogs();
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestClearLogs");
}

void			CTaskDispatchManager::onRequestValidateAllDFFInActiveTab(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestValidateAllDFFInActiveTab");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestValidateAllDFFInActiveTab", true);
		return;
	}

	vector<string> vecCorruptDFFEntryLines;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (CPathUtility::isModelExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()))))
		{
			CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
			if (pDFFFile->doesHaveError())
			{
				vecCorruptDFFEntryLines.push_back(pIMGEntry->getEntryName() + " - " + pDFFFile->getErrorReason());
			}
			pDFFFile->unload();
			delete pDFFFile;
		}
	}

	if (vecCorruptDFFEntryLines.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_48"), CLocalizationManager::getInstance()->getTranslatedText("DFFValidationComplete"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	else
	{
		getKGM()->getTaskManager()->onTaskPause();
		getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("DFFValidationComplete"), CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_TextArea_6_Message", vecCorruptDFFEntryLines.size()), CStringUtility::join(vecCorruptDFFEntryLines, "\n"));
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestValidateAllDFFInActiveTab");
}

void			CTaskDispatchManager::onRequestValidateAllTXDInActiveTab(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestValidateAllTXDInActiveTab");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestValidateAllTXDInActiveTab", true);
		return;
	}

	vector<string> vecCorruptTXDEntryLines;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) == "TXD")
		{
			string strTXDData = pIMGEntry->getEntryData();
			if (!CTXDFormat::isTXDSizeValid(strTXDData.size()))
			{
				vecCorruptTXDEntryLines.push_back(pIMGEntry->getEntryName() + " - TXD size not valid");
			}
			else
			{
				CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaMemory(strTXDData);
				if (pTXDFile->doesHaveError())
				{
					vecCorruptTXDEntryLines.push_back(pIMGEntry->getEntryName() + " - Failed to parse");
				}
				else
				{
					if (!CTXDFormat::isTextureCountValid(pTXDFile->getTextures().size(), pTXDFile->getGames()))
					{
						vecCorruptTXDEntryLines.push_back(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_InvalidTextureCount", pIMGEntry->getEntryName(), pTXDFile->getTextures().size()));
					}
					else
					{
						uint32 uiTextureIndex = 0;
						for (auto pTexture : pTXDFile->getTextures())
						{
							if (!CTXDFormat::isTextureResolutionValid(pTexture->getImageSize().m_x, pTexture->getImageSize().m_y, pTXDFile->getGames()))
							{
								vecCorruptTXDEntryLines.push_back(pIMGEntry->getEntryName() + " - Invalid texture resolution: " + pTexture->getDiffuseName() + " (" + CStringUtility::toString(pTexture->getImageSize().m_x) + " x " + CStringUtility::toString(pTexture->getImageSize().m_y) + ")");
								break;
							}

							if (!CTXDFormat::isTextureNameValid(pTexture->getDiffuseName()) || !CTXDFormat::isTextureNameValid(pTexture->getAlphaName(), true))
							{
								vecCorruptTXDEntryLines.push_back(pIMGEntry->getEntryName() + " - Invalid texture name: #" + CStringUtility::toString(uiTextureIndex + 1));
								break;
							}

							uiTextureIndex++;
						}
					}
				}
				pTXDFile->unload();
				delete pTXDFile;
			}
		}
	}

	if (vecCorruptTXDEntryLines.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskPause();
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_49"), CLocalizationManager::getInstance()->getTranslatedText("TXDValidationComplete"), MB_OK);
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	else
	{
		getKGM()->getTaskManager()->onTaskPause();
		getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("TXDValidationComplete"), CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_TextArea_7_Message", vecCorruptTXDEntryLines.size()), CStringUtility::join(vecCorruptTXDEntryLines, "\n"));
		getKGM()->getTaskManager()->onTaskUnpause();
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestValidateAllTXDInActiveTab");
}

void			CTaskDispatchManager::onRequestCredits(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestCredits");
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showCreditsDialog();
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestCredits");
}

void			CTaskDispatchManager::onRequestEntryViewer(bool bDontOpenWindow)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestEntryViewer");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
		return;
	}

	// fetch selected IMG entry
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
		return;
	}
	int nItem = pListControl->GetNextSelectedItem(pos);
	CIMGEntry *pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

	// check the entry type
	if (pIMGEntry->isTextureFile())
	{
		CTextureViewer *pTextureViewer = getKGM()->getEntryViewerManager()->getTextureViewer();

		if (bDontOpenWindow && !pTextureViewer->isWindowOpen())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
			return;
		}

		pTextureViewer->reset();
		pTextureViewer->setIMGEntry(pIMGEntry);
		pTextureViewer->setWindowTitle(CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_TextureViewer_TitleWithEntryName", pIMGEntry->getEntryName().c_str()));
		pTextureViewer->prepareRenderData();

		if (pTextureViewer->isEntityDataCorrupt())
		{
			if (!bDontOpenWindow)
			{
				CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText(pIMGEntry->isTXDFile() ? "TXDReadError" : "WTDReadError"), CLocalizationManager::getInstance()->getTranslatedText("TextPopup_Title52"), MB_OK);
			}
			getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
			return;
		}

		if (pTextureViewer->isWindowOpen())
		{
			pTextureViewer->applyWindowTitle();
		}
		else
		{
			if (!bDontOpenWindow)
			{
				pTextureViewer->loadThreadAndWindow();
			}
		}

		pTextureViewer->forceRender();
	}
	*/



	/*
	else if (pIMGEntry->isCOLFile())
	{
		CCollisionViewer *pCollisionViewer = getKGM()->getEntryViewerManager()->getCollisionViewer();

		if (pCollisionViewer->isWindowOpening())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
			return;
		}

		if (bDontOpenWindow && !pCollisionViewer->isWindowOpen())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
			return;
		}

		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pCOLFile->doesHaveError())
		{
			if (!bDontOpenWindow)
			{
				CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("COLReadError"), CLocalizationManager::getInstance()->getTranslatedText("TextPopup_Title52"), MB_OK);
			}
			pCOLFile->unload();
			delete pCOLFile;
			getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
			return;
		}

		if (pCollisionViewer->isWindowOpen())
		{
			if(!bDontOpenWindow)
			{
				pCollisionViewer->unloadThreadAndWindow();
			}
		}

		pCollisionViewer->setEntryChanging(true);
		pCollisionViewer->reset();
		pCollisionViewer->setCOLFile(pCOLFile);
		pCollisionViewer->setWindowTitle(CLocalizationManager::getInstance()->getTranslatedFormattedText("Window_CollisionViewer_TitleWithEntryName", pIMGEntry->getEntryName().c_str()));
		//pCollisionViewer->setCOLFileName(pIMGEntry->getEntryName());
		pCollisionViewer->prepareRenderData();
		pCollisionViewer->setEntryChanging(false);

		if (pCollisionViewer->isWindowOpen())
		{
			pCollisionViewer->applyWindowTitle();
		}
		else
		{
			if (!bDontOpenWindow)
			{
				pCollisionViewer->loadThreadAndWindow();
			}
		}
	}
	*/


	/*
	todo
	else if (pIMGEntry->isModelFile())
	{
	}
	else
	{
		// entry viewer does not support the type of file
		getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer", true);
		return;
	}

	getKGM()->getTaskManager()->onTaskEnd("onRequestEntryViewer");
	*/
}

void			CTaskDispatchManager::onRequestRenamer(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRenamer");
	getKGM()->getTaskManager()->onTaskPause();
	CRenamerDialogData *pRenamerDialogData = getKGM()->getGUIManager()->showRenamerDialog();
	getKGM()->getTaskManager()->onTaskUnpause();

	if (!pRenamerDialogData->m_bRename)
	{
		delete pRenamerDialogData;
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenamer", true);
		return;
	}

	// ensure a tab is open
	if (getKGM()->getEntryListTab() == nullptr)
	{
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_50"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_50"), MB_OK);
		delete pRenamerDialogData;
		getKGM()->getTaskManager()->onTaskEnd("onRequestRenamer", true);
		return;
	}

	// choose entries to rename
	vector<CIMGEntry*> vecIMGEntries;
	if (pRenamerDialogData->m_ucEntriesType == 0) // all entries
	{
		vecIMGEntries = getKGM()->getEntryListTab()->getIMGFile()->getEntries();
	}
	else if (pRenamerDialogData->m_ucEntriesType == 1) // selected entries
	{
		vecIMGEntries = getKGM()->getEntryListTab()->getSelectedEntries();
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size() * 2); // x1 for the main loop, and x1 for the refreshing the main list view display

	vector<CRenamedIMGEntry*> vecIMGEntriesWithNewNames;

	// loop around entries
	for (auto pIMGEntry : vecIMGEntries)
	{
		// skip LODs if "update LOD names to match new name" is enabled, as matching LOD entries will be updated anyway
		if (pRenamerDialogData->m_bUpdateLODNamesToMatch)
		{
			if (CStringUtility::toUpperCase(pIMGEntry->getEntryName().substr(0, 3)) == "LOD")
			{
				getKGM()->getTaskManager()->onTaskProgressTick();
				continue;
			}
		}

		string strEntryPreviousName = pIMGEntry->getEntryName();

		// character adding/replacing
		if (pRenamerDialogData->m_ucCharReplacementType == 0) // don't replace characters
		{
			if (pRenamerDialogData->m_strCharAdding_Front != "")
			{
				pIMGEntry->setEntryName(pRenamerDialogData->m_strCharAdding_Front + pIMGEntry->getEntryName());
			}

			if (pRenamerDialogData->m_strCharAdding_End != "")
			{
				pIMGEntry->setEntryName(pIMGEntry->getEntryName() + pRenamerDialogData->m_strCharAdding_End);
			}

			if (pRenamerDialogData->m_strCharAdding_AtPosition != "")
			{
				uint32
					uiPosition = pRenamerDialogData->m_uiCharAddingAtPosition_Position;
				if (uiPosition <= pIMGEntry->getEntryName().length())
				{
					string
						strNameBefore = pIMGEntry->getEntryName().substr(0, uiPosition),
						strNameAfter = pIMGEntry->getEntryName().substr(uiPosition);
					pIMGEntry->setEntryName(strNameBefore + pRenamerDialogData->m_strCharAdding_AtPosition + strNameAfter);
				}
			}
		}
		else if (pRenamerDialogData->m_ucCharReplacementType == 1) // always replace characters
		{
			if (pRenamerDialogData->m_strCharAdding_Front != "")
			{
				string strNewEntryName = pRenamerDialogData->m_strCharAdding_Front + pIMGEntry->getEntryName().substr(pRenamerDialogData->m_strCharAdding_Front.length());
				pIMGEntry->setEntryName(strNewEntryName);
			}

			if (pRenamerDialogData->m_strCharAdding_End != "")
			{
				string strNewEntryName = pIMGEntry->getEntryName().substr(0, pIMGEntry->getEntryName().length() - pRenamerDialogData->m_strCharAdding_End.length()) + pRenamerDialogData->m_strCharAdding_End;
				pIMGEntry->setEntryName(strNewEntryName);
			}

			if (pRenamerDialogData->m_strCharAdding_AtPosition != "")
			{
				uint32
					uiPosition = pRenamerDialogData->m_uiCharAddingAtPosition_Position;
				if (uiPosition <= pIMGEntry->getEntryName().length())
				{
					string
						strNameBefore = pIMGEntry->getEntryName().substr(0, uiPosition),
						strNameAfter = pIMGEntry->getEntryName().substr(uiPosition + pRenamerDialogData->m_strCharAdding_AtPosition.length());
					pIMGEntry->setEntryName(strNameBefore + pRenamerDialogData->m_strCharAdding_AtPosition + strNameAfter);
				}
			}
		}
		else if (pRenamerDialogData->m_ucCharReplacementType == 2) // only replace characters if length exceeds
		{
			uint32
				uiExceedLength = pRenamerDialogData->m_uiCharReplaceConditional_Length;

			if (pRenamerDialogData->m_strCharAdding_Front != "")
			{
				string strPrefixText = pRenamerDialogData->m_strCharAdding_Front;
				string strEntryName = pIMGEntry->getEntryName();

				string strExtension = CPathUtility::getFileExtension(pIMGEntry->getEntryName());
				uint32 uiCharCountLeft = uiExceedLength - (strExtension.length() + 1);

				string strEntryNewName = CStringUtility::mergeStrings(strPrefixText, CPathUtility::removeFileExtension(strEntryName), uiExceedLength) + "." + strExtension;


				/*
				uint32 uiNewLength = pRenamerDialogData->m_strCharAdding_Front.length() + pIMGEntry->getEntryName().length();
				uint32 uiNameAfterSubstrPosition = uiNewLength > uiExceedLength ? (uiNewLength - uiExceedLength) : 0;
				//CDebugger::log("pRenamerDialogData->m_strCharAdding_Front.length(): " + CStringUtility::toString(pRenamerDialogData->m_strCharAdding_Front.length()));
				//CDebugger::log("pIMGEntry->getEntryName().length(): " + CStringUtility::toString(pIMGEntry->getEntryName().length()));
				//CDebugger::log("uiNewLength: " + CStringUtility::toString(uiNewLength));
				//CDebugger::log("uiNameAfterSubstrPosition: " + CStringUtility::toString(uiNameAfterSubstrPosition));
				string strNameAfter = pIMGEntry->getEntryName().substr(uiNameAfterSubstrPosition); // substr(12) causes crash when str.size() == 11
				string strNewEntryName = pRenamerDialogData->m_strCharAdding_Front + strNameAfter;
				*/
				pIMGEntry->setEntryName(strEntryNewName);
			}

			if (pRenamerDialogData->m_strCharAdding_End != "")
			{
				uint32 uiNewLength = pRenamerDialogData->m_strCharAdding_End.length() + pIMGEntry->getEntryName().length();
				uint32 uiNameBeforeSubstrLength = uiNewLength > uiExceedLength ? (pIMGEntry->getEntryName().length() - (uiNewLength - uiExceedLength)) : 0;
				string strNameBefore = pIMGEntry->getEntryName().substr(0, uiNameBeforeSubstrLength);
				string strNewEntryName = strNameBefore + pRenamerDialogData->m_strCharAdding_End;
				pIMGEntry->setEntryName(strNewEntryName);
			}

			if (pRenamerDialogData->m_strCharAdding_AtPosition != "")
			{
				uint32
					uiPosition = pRenamerDialogData->m_uiCharAddingAtPosition_Position;
				if (uiPosition <= pIMGEntry->getEntryName().length())
				{
					uint32 uiNewLength = pRenamerDialogData->m_strCharAdding_AtPosition.length() + pIMGEntry->getEntryName().length();
					uint32 uiNameAfterSubstrPosition = uiNewLength > uiExceedLength ? (uiPosition + (uiNewLength - uiExceedLength)) : 0;
					string
						strNameBefore = pIMGEntry->getEntryName().substr(0, uiPosition),
						strNameAfter = pIMGEntry->getEntryName().substr(uiNameAfterSubstrPosition);
					string strNewEntryName = strNameBefore + pRenamerDialogData->m_strCharAdding_AtPosition + strNameAfter;
					pIMGEntry->setEntryName(strNewEntryName);
				}
			}
		}

		// character removing
		if (pRenamerDialogData->m_uiCharRemoving_Front > 0)
		{
			if ((pIMGEntry->getEntryName().length() - pRenamerDialogData->m_uiCharRemoving_Front) > 0)
			{
				pIMGEntry->setEntryName(pIMGEntry->getEntryName().substr(pRenamerDialogData->m_uiCharRemoving_Front));
			}
		}

		if (pRenamerDialogData->m_uiCharRemoving_End > 0)
		{
			if ((pIMGEntry->getEntryName().length() - pRenamerDialogData->m_uiCharRemoving_End) > 0)
			{
				pIMGEntry->setEntryName(pIMGEntry->getEntryName().substr(pIMGEntry->getEntryName().length() - pRenamerDialogData->m_uiCharRemoving_End));
			}
		}

		if (pRenamerDialogData->m_uiCharRemoving_AtPosition > 0)
		{
			uint32
				uiPosition = pRenamerDialogData->m_uiCharRemovingAtPosition_Position,
				uiLength = pRenamerDialogData->m_uiCharRemoving_AtPosition;
			if ((pIMGEntry->getEntryName().length() - uiLength) > 0)
			{
				string
					strNameBefore = pIMGEntry->getEntryName().substr(0, uiPosition),
					strNameAfter = pIMGEntry->getEntryName().substr(uiPosition + uiLength);
				pIMGEntry->setEntryName(strNameBefore + strNameAfter);
			}
		}

		// character case
		if (pRenamerDialogData->m_ucCharCaseType == 0)
		{
			// leave untouched
		}
		else if (pRenamerDialogData->m_ucCharCaseType == 1)
		{
			// uppercase
			pIMGEntry->setEntryName(CStringUtility::toUpperCase(pIMGEntry->getEntryName()));
		}
		else if (pRenamerDialogData->m_ucCharCaseType == 2)
		{
			// lowercase
			pIMGEntry->setEntryName(CStringUtility::toLowerCase(pIMGEntry->getEntryName()));
		}
		else if (pRenamerDialogData->m_ucCharCaseType == 3)
		{
			// title case
			pIMGEntry->setEntryName(CStringUtility::toTitleCase(pIMGEntry->getEntryName()));
		}

		// mark IMG entry as renamed
		if (strEntryPreviousName != pIMGEntry->getEntryName())
		{
			CRenamedIMGEntry *pRenamedIMGEntry = new CRenamedIMGEntry;
			pRenamedIMGEntry->m_pIMGEntry = pIMGEntry;
			pRenamedIMGEntry->m_strPreviousName = strEntryPreviousName;
			vecIMGEntriesWithNewNames.push_back(pRenamedIMGEntry);
		}

		// update LOD names to match new name
		if (pRenamerDialogData->m_bUpdateLODNamesToMatch)
		{
			string strLODPreviousName = "LOD" + strEntryPreviousName.substr(3);
			CIMGEntry *pIMGEntryLOD = getKGM()->getEntryListTab()->getIMGFile()->getEntryByName(strLODPreviousName);
			if (pIMGEntryLOD != nullptr)
			{
				string strLODNewName = pIMGEntryLOD->getEntryName().substr(0, 3) + pIMGEntry->getEntryName().substr(3);
				pIMGEntryLOD->setEntryName(strLODNewName);

				// mark IMG entry as renamed
				if (strLODPreviousName != strLODNewName)
				{
					CRenamedIMGEntry *pRenamedIMGEntry = new CRenamedIMGEntry;
					pRenamedIMGEntry->m_pIMGEntry = pIMGEntryLOD;
					pRenamedIMGEntry->m_strPreviousName = strLODPreviousName;
					vecIMGEntriesWithNewNames.push_back(pRenamedIMGEntry);
				}
			}
		}

		// progress bar
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// matching entries
	vector<string> vecFilePaths;

	// matching entries - IDE
	if (pRenamerDialogData->m_bMatchingState_IDE)
	{
		vecFilePaths = CFileUtility::getFilePaths(pRenamerDialogData->m_strMatchingPath_IDE, true);
		for (auto strFilePath : vecFilePaths)
		{
			if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFilePath)) == "IDE")
			{
				CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strFilePath);
				
				if(pIDEFile->doesHaveError())
				{
					pIDEFile->unload();
					delete pIDEFile;
					continue;
				}
				
				for (auto pRenamedIMGEntry : vecIMGEntriesWithNewNames)
				{
					vector<CIDEEntry*> vecIDEEntries;

					// OBJS/TOBJ model name
					vecIDEEntries = pIDEFile->getEntriesByModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_strPreviousName));
					for (auto pIDEEntry : vecIDEEntries)
					{
						switch (pIDEEntry->getSectionType()) // todo - make work with all sections with setModelName on CIDEEntry_Data and CIPLEntry_Data
						{
						case IDE_SECTION_OBJS:
							((CIDEEntry_OBJS*)pIDEEntry)->setModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
							break;
						case IDE_SECTION_TOBJ:
							((CIDEEntry_TOBJ*)pIDEEntry)->setModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
							break;
						}
					}

					// OBJS/TOBJ texture name
					vecIDEEntries = pIDEFile->getEntriesByTXDName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_strPreviousName));
					for (auto pIDEEntry : vecIDEEntries)
					{
						switch (pIDEEntry->getSectionType()) // todo - make work with all sections with setTXDName on CIDEEntry_Data and CIPLEntry_Data
						{
						case IDE_SECTION_OBJS:
							((CIDEEntry_OBJS*)pIDEEntry)->setTXDName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
							break;
						case IDE_SECTION_TOBJ:
							((CIDEEntry_TOBJ*)pIDEEntry)->setTXDName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
							break;
						}
					}
				}

				pIDEFile->serializeViaFile();
				pIDEFile->unload();
				delete pIDEFile;
			}
		}
	}

	// matching entries - IPL
	if (pRenamerDialogData->m_bMatchingState_IPL)
	{
		vecFilePaths = CFileUtility::getFilePaths(pRenamerDialogData->m_strMatchingPath_IPL, true);
		for (auto strFilePath : vecFilePaths)
		{
			if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFilePath)) == "IPL")
			{
				CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strFilePath);
				
				if(pIPLFile->doesHaveError())
				{
					pIPLFile->unload();
					delete pIPLFile;
					continue;
				}
				
				if (pIPLFile->isBinary()) // skip binary IPL files as they don't contain a model name
				{
					pIPLFile->unload();
					delete pIPLFile;
					continue;
				}

				for (auto pRenamedIMGEntry : vecIMGEntriesWithNewNames)
				{
					vector<CIPLEntry*> vecIPLEntries;

					// INST model name
					vecIPLEntries = pIPLFile->getEntriesByModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_strPreviousName));
					for (auto pIPLEntry : vecIPLEntries)
					{
						// todo - make work with all sections with setModelName on CIDEEntry_Data and CIPLEntry_Data
						if (pIPLEntry->getSectionType() == IPL_SECTION_INST)
						{
							((CIPLEntry_INST*)pIPLEntry)->setModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
						}
					}
				}

				pIPLFile->serializeViaFile();
				pIPLFile->unload();
				delete pIPLFile;
			}
		}
	}

	// matching entries - COL
	vector<string> vecCorruptCOLFiles;
	if (pRenamerDialogData->m_bMatchingState_COL)
	{
		vecFilePaths = CFileUtility::getFilePaths(pRenamerDialogData->m_strMatchingPath_COL, true);
		for (auto strFilePath : vecFilePaths)
		{
			if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFilePath)) == "COL")
			{
				//CDebugger::log("strFilePath: " + strFilePath);
				CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strFilePath);
				pCOLFile->setFilePath(strFilePath);
				if (pCOLFile->doesHaveError())
				{
					vecCorruptCOLFiles.push_back(CPathUtility::getFileName(pCOLFile->getFilePath()) + " - Corrupt Reason: " + pCOLFile->getErrorReason());
					//CDebugger::log(CPathUtility::getFileName(pCOLFile->getFilePath()) + " - CORRUPT REASON: " + pCOLFile->getCorruptReason());
					pCOLFile->unload();
					delete pCOLFile;
					continue;
				}

				for (auto pRenamedIMGEntry : vecIMGEntriesWithNewNames)
				{
					vector<CCOLEntry*> vecCOLEntries;

					// COL model name
					vecCOLEntries = pCOLFile->getEntriesByModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_strPreviousName));
					for (auto pCOLEntry : vecCOLEntries)
					{
						pCOLEntry->setModelName(CPathUtility::removeFileExtension(pRenamedIMGEntry->m_pIMGEntry->getEntryName()));
					}
				}

				pCOLFile->serializeViaFile();
				pCOLFile->unload();
				delete pCOLFile;
			}
		}
	}

	// log
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_116", vecIMGEntriesWithNewNames.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_117"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecCorruptCOLFiles, "\n"), true);

	// mark tab as modified
	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	// refresh tab's main list view
	getKGM()->getEntryListWindow()->refreshActiveTab();

	// clean up
	for (auto pRenamedIMGEntry : vecIMGEntriesWithNewNames)
	{
		delete pRenamedIMGEntry;
	}
	vecIMGEntriesWithNewNames.clear();
	delete pRenamerDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestRenamer");
}

void		CTaskDispatchManager::onRequestClearRecentlyOpenedList(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestClearRecentlyOpenedList");
	getKGM()->getRecentlyOpenManager()->removeRecentlyOpenedEntries();
	getKGM()->getTaskManager()->onTaskEnd("onRequestClearRecentlyOpenedList");
}

void		CTaskDispatchManager::onRequestBuildTXD(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestBuildTXD");

	getKGM()->getTaskManager()->onTaskPause();
	CBuildTXDDialogData *pBuildTXDDialogData = getKGM()->getGUIManager()->showBuildTXDDialog();
	getKGM()->getTaskManager()->onTaskUnpause();

	if (!pBuildTXDDialogData->m_bBuild) // cancel
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestBuildTXD", true);
		delete pBuildTXDDialogData;
		return;
	}

	// choose DFF files
	//vector<CDFFFormat*> veCDFFFormats;
	unordered_map<CDFFFormat*, string> umapDFFEntries;
	if (pBuildTXDDialogData->m_uCDFFFormatsType == 0) // All DFF entries in active tab
	{
		if (!getKGM()->getEntryListTab())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestBuildTXD", true);
			delete pBuildTXDDialogData;
			return;
		}

		vector<CIMGEntry*> vecIMGEntries = getKGM()->getEntryListTab()->getIMGFile()->getEntriesByExtension("DFF");
		vector<CIMGEntry*> vecIMGEntries_BSP = getKGM()->getEntryListTab()->getIMGFile()->getEntriesByExtension("BSP");
		for (auto pIMGEntry : vecIMGEntries_BSP)
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
		for (auto pIMGEntry : vecIMGEntries)
		{
			CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
			umapDFFEntries[pDFFFile] = CPathUtility::removeFileExtension(pIMGEntry->getEntryName());
		}
	}
	else if (pBuildTXDDialogData->m_uCDFFFormatsType == 1) // Selected DFF entries in active tab
	{
		if (!getKGM()->getEntryListTab())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestBuildTXD", true);
			delete pBuildTXDDialogData;
			return;
		}

		vector<CIMGEntry*> vecIMGEntries = getKGM()->getEntryListTab()->getSelectedEntries();
		
		for(auto pIMGEntry : vecIMGEntries)
		{
			if (CPathUtility::isModelExtension(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName()))))
			{
				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
				umapDFFEntries[pDFFFile] = CPathUtility::removeFileExtension(pIMGEntry->getEntryName());
			}
		}
	}
	else if (pBuildTXDDialogData->m_uCDFFFormatsType == 2) // All DFF entries in all tabs
	{
		if (!getKGM()->getEntryListTab())
		{
			getKGM()->getTaskManager()->onTaskEnd("onRequestBuildTXD", true);
			delete pBuildTXDDialogData;
			return;
		}

		for (auto pWindowTab : getKGM()->getEntryListWindow()->getEntries())
		{
			vector<CIMGEntry*> vecIMGEntries = ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntriesByExtension("DFF");
			vector<CIMGEntry*> vecIMGEntries_BSP = ((CEntryListWindowTab*)pWindowTab)->getIMGFile()->getEntriesByExtension("BSP");
			for (auto pIMGEntry : vecIMGEntries_BSP)
			{
				vecIMGEntries.push_back(pIMGEntry);
			}
			for (auto pIMGEntry : vecIMGEntries)
			{
				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
				umapDFFEntries[pDFFFile] = CPathUtility::removeFileExtension(pIMGEntry->getEntryName());
			}
		}
	}
	else if (pBuildTXDDialogData->m_uCDFFFormatsType == 3) // Folder containing DFF files
	{
		vector<string> vecFileNames = CFileUtility::getFileNames(pBuildTXDDialogData->m_strDFFsFolderPath);

		for (string strFileName : vecFileNames)
		{
			string strExtensionUpper = CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFileName));
			if (CPathUtility::isModelExtension(strExtensionUpper))
			{
				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pBuildTXDDialogData->m_strDFFsFolderPath + strFileName);
				umapDFFEntries[pDFFFile] = CPathUtility::removeFileExtension(strFileName);
			}
		}
	}
	else if (pBuildTXDDialogData->m_uCDFFFormatsType == 4) // IDE file referencing DFF files
	{
		CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(pBuildTXDDialogData->m_strIDEFilePath);
		if(!pIDEFile->doesHaveError())
		{
			// todo - make it work with all IDE sections
			for (auto pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_OBJS>(IDE_SECTION_OBJS))
			{
				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pBuildTXDDialogData->m_strDFFsFolderPath + pIDEEntry->getModelName());
				umapDFFEntries[pDFFFile] = pIDEEntry->getTXDName();
			}
			for (auto pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_TOBJ>(IDE_SECTION_TOBJ))
			{
				CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pBuildTXDDialogData->m_strDFFsFolderPath + pIDEEntry->getModelName());
				umapDFFEntries[pDFFFile] = pIDEEntry->getTXDName();
			}
		}
		
		pIDEFile->unload();
		delete pIDEFile;
	}

	// process
	//for (auto pDFFFile : veCDFFFormats)
	uint32
		uiTotalTXDFileCount = 0,
		uiTotalTextureCountUsed = 0;
	vector<string>
		vecTextureImagesNotFound,
		veCTXDFormatNames;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(umapDFFEntries.size());
	for (auto it : umapDFFEntries)
	{
		CDFFFormat *pDFFFile = it.first;
		string strOutputFileName = it.second + ".txd";

		if (pDFFFile->doesHaveError())
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		vector<string> vecTextureNames = pDFFFile->getTextureNames();
		vecTextureNames = CVectorUtility::removeDuplicates(vecTextureNames);

		uint32 uiTXDFileCount = pBuildTXDDialogData->m_uiTextureCountPerTXD == 0 ? 1 : (uint32) ceil((float32)vecTextureNames.size() / (float32)pBuildTXDDialogData->m_uiTextureCountPerTXD);
		uiTotalTXDFileCount += uiTXDFileCount;
		uint32 uiTXDFileIndex = 0;
		for (uint32 i = 0; i < uiTXDFileCount; i++)
		{
			string strFilePath = pBuildTXDDialogData->m_strDestinationFolderPath + strOutputFileName;
			strFilePath = CPathUtility::getNextFileName(strFilePath, uiTXDFileIndex, "-part");

			CTXDFormat *pTXDFile = CTXDManager::getInstance()->createFormat();
			pTXDFile->setRWVersion(pBuildTXDDialogData->m_pRWVersion);
			for (uint32 i2 = i * pBuildTXDDialogData->m_uiTextureCountPerTXD, j2 = i2 + (pBuildTXDDialogData->m_uiTextureCountPerTXD == 0 ? vecTextureNames.size() : pBuildTXDDialogData->m_uiTextureCountPerTXD); i2 < j2; i2++)
			{
				if (i2 >= vecTextureNames.size())
				{
					break;
				}

				string strTextureName = vecTextureNames[i2];
				string strTextureImagePath = CFileUtility::findImageFile(strTextureName, pBuildTXDDialogData->m_strTexturesFolderPath);

				// check if texture file exists
				if (strTextureImagePath != "")
				{
					uiTotalTextureCountUsed++;

					// add texture to TXD
					CRWSection_TextureNative *pTexture = pTXDFile->addTextureViaFile(strTextureImagePath, strTextureName, strTextureName + "a");

					if (pTexture != nullptr)
					{
						//vector<string> vecMipmapsRemoved;
						//pTexture->convertToRasterDataFormat(pBuildTXDDialogData->m_pRasterDataFormat->getId(), vecMipmapsRemoved);
					}
				}
				else
				{
					vecTextureImagesNotFound.push_back(strTextureName + ".BMP");
				}
			}
			if (pTXDFile->getTextures().size() > 0)
			{
				pTXDFile->storeViaFile(strFilePath);
				veCTXDFormatNames.push_back(CPathUtility::getFileName(strFilePath));
				uiTXDFileIndex++;
			}
			
			pTXDFile->unload();
			delete pTXDFile;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_118", uiTotalTXDFileCount, uiTotalTextureCountUsed));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_119", pBuildTXDDialogData->m_strTexturesFolderPath.c_str()), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecTextureImagesNotFound, "\n"), true);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_120"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(veCTXDFormatNames, "\n"), true);

	// clean up
	//for (CDFFFormat *pDFFFile : veCDFFFormats)
	for (auto it : umapDFFEntries)
	{
		CDFFFormat *pDFFFile = it.first;
		pDFFFile->unload();
		delete pDFFFile;
	}
	//veCDFFFormats.clear();
	umapDFFEntries.clear();
	delete pBuildTXDDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestBuildTXD");
}

void		CTaskDispatchManager::onRequestIMGVersionSettings(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestIMGVersionSettings");

	// ensure a tab is open
	if (getKGM()->getEntryListTab() == nullptr)
	{
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_50"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_50"), MB_OK);
		getKGM()->getTaskManager()->onTaskEnd("onRequestIMGVersionSettings", true);
		return;
	}

	// ensure IMG version has settings
	if (getKGM()->getEntryListTab()->getIMGFile()->getIMGVersion() != IMG_FASTMAN92)
	{
		CGUIUtility::showMessage(CLocalizationManager::getInstance()->getTranslatedText("TextPopup_51"), CLocalizationManager::getInstance()->getTranslatedText("TextPopupTitle_51"), MB_OK);
		getKGM()->getTaskManager()->onTaskEnd("onRequestIMGVersionSettings", true);
		return;
	}

	// show IMG Version Settings window
	getKGM()->getTaskManager()->onTaskPause();
	CIMGVersionSettingsDialogData *pIMGVersionSettingsDialogData = getKGM()->getGUIManager()->showIMGVersionSettingsDialog();
	getKGM()->getTaskManager()->onTaskUnpause();

	if (!pIMGVersionSettingsDialogData->m_bSave)
	{
		delete pIMGVersionSettingsDialogData;
		getKGM()->getTaskManager()->onTaskEnd("onRequestIMGVersionSettings", true);
		return;
	}

	// fetch selected entries
	vector<CIMGEntry*> vecSelectedIMGEntries = getKGM()->getEntryListTab()->getSelectedEntries();
	
	// choose IMG entries
	vector<CIMGEntry*> vecIMGEntries;
	if (pIMGVersionSettingsDialogData->m_ucEntriesType == 0) // All entries in active tab
	{
		vecIMGEntries = getKGM()->getEntryListTab()->getIMGFile()->getEntries();
	}
	else if (pIMGVersionSettingsDialogData->m_ucEntriesType == 1) // Selected entries in active tab
	{
		vecIMGEntries = vecSelectedIMGEntries;
	}

	// apply new compression type to IMG entries
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntries.size());
	vector<string> vecEntryNames;
	for (auto pIMGEntry : vecIMGEntries)
	{
		pIMGEntry->applyCompression(pIMGVersionSettingsDialogData->m_eCompressionAlgorithm, pIMGVersionSettingsDialogData->m_uiCompressionLevel);
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
		vecEntryNames.push_back(pIMGEntry->getEntryName());

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// update IMG version text in main window
	getKGM()->getEntryListTab()->updateIMGText();

	// log
	// todo rename getCompressionTypeText to alrogrfirmtm
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_121", CIMGManager::getCompressionTypeText(pIMGVersionSettingsDialogData->m_eCompressionAlgorithm).c_str(), vecIMGEntries.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_122"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecEntryNames, "\n"), true);

	// mark as modified since rebuild
	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	// clean up
	delete pIMGVersionSettingsDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestIMGVersionSettings");
}

void		CTaskDispatchManager::onRequestFeatureByName(string strFeatureName)
{
	if (strFeatureName == "onRequestFeatureByName")
	{
	}
	else if (strFeatureName == "onRequestOpen")
	{
		onRequestOpen();
	}
	else if (strFeatureName == "onRequestOpen2")
	{
		onRequestOpen2(getKGM()->getLastUsedValueManager()->getLastUsedValue_Open2_IMGPath());
	}
	else if (strFeatureName == "onRequestClose")
	{
		onRequestClose();
	}
	else if (strFeatureName == "onRequestCloseAll")
	{
		onRequestCloseAll();
	}
	else if (strFeatureName == "onRequestExitTool")
	{
		onRequestExitTool();
	}
	else if (strFeatureName == "onRequestImportViaFiles")
	{
		onRequestImportViaFiles();
	}
	else if (strFeatureName == "onRequestRemoveSelected")
	{
		onRequestRemoveSelected();
	}
	else if (strFeatureName == "onRequestRenameEntry")
	{
		onRequestRenameEntry();
	}
	else if (strFeatureName == "onRequestSelectAll")
	{
		onRequestSelectAll();
	}
	else if (strFeatureName == "onRequestSelectInverse")
	{
		onRequestSelectInverse();
	}
	else if (strFeatureName == "onRequestRebuild")
	{
		onRequestRebuild();
	}
	else if (strFeatureName == "onRequestRebuildAs")
	{
		onRequestRebuildAs();
	}
	else if (strFeatureName == "onRequestRebuildAll")
	{
		onRequestRebuildAll();
	}
	else if (strFeatureName == "onRequestConvertIMGVersion")
	{
		onRequestConvertIMGVersion(getKGM()->getLastUsedValueManager()->getLastUsedValue_Convert_IMGVersion());
	}
	else if (strFeatureName == "onRequestConvertIMGVersionViaButton")
	{
		onRequestConvertIMGVersionViaButton();
	}
	else if (strFeatureName == "onRequestMerge")
	{
		onRequestMerge();
	}
	else if (strFeatureName == "onRequestSplitViaButton")
	{
		onRequestSplitViaButton();
	}
	else if (strFeatureName == "onRequestSplitSelectedEntries")
	{
		onRequestSplitSelectedEntries();
	}
	else if (strFeatureName == "onRequestSplitViaIDEFile")
	{
		onRequestSplitViaIDEFile();
	}
	else if (strFeatureName == "onRequestSplitViaTextLines")
	{
		onRequestSplitViaTextLines();
	}
	else if (strFeatureName == "onRequestReplace")
	{
		onRequestReplace();
	}
	else if (strFeatureName == "onRequestExportSelected")
	{
		onRequestExportSelected();
	}
	else if (strFeatureName == "onRequestSearchText")
	{
		onRequestSearchText();
	}
	else if (strFeatureName == "onRequestSearchSelection")
	{
		onRequestSearchSelection();
	}
	else if (strFeatureName == "onRequestFilter")
	{
		onRequestFilter();
	}
	else if (strFeatureName == "onRequestFind")
	{
		onRequestFind();
	}
	else if (strFeatureName == "onRequestExportViaButton")
	{
		onRequestExportViaButton();
	}
	else if (strFeatureName == "onRequestExportViaIDEFile")
	{
		onRequestExportViaIDEFile();
	}
	else if (strFeatureName == "onRequestExportViaTextLines")
	{
		onRequestExportViaTextLines();
	}
	else if (strFeatureName == "onRequestSortEntries")
	{
		onRequestSortEntries();
	}
	else if (strFeatureName == "onRequestSortButton")
	{
		onRequestSortButton();
	}
	else if (strFeatureName == "onRequestRemoveViaIDEFile")
	{
		onRequestRemoveViaIDEFile();
	}
	else if (strFeatureName == "onRequestRemoveViaTextLines")
	{
		onRequestRemoveViaTextLines();
	}
	else if (strFeatureName == "onRequestRemoveViaButton")
	{
		onRequestRemoveViaButton();
	}
	else if (strFeatureName == "onRequestImportViaButton")
	{
		onRequestImportViaButton();
	}
	else if (strFeatureName == "onRequestImportViaIDEFile")
	{
		onRequestImportViaIDEFile();
	}
	else if (strFeatureName == "onRequestImportViaTextLines")
	{
		onRequestImportViaTextLines();
	}
	else if (strFeatureName == "onRequestNew")
	{
		onRequestNew(getKGM()->getLastUsedValueManager()->getLastUsedValue_New_IMGVersion());
	}
	else if (strFeatureName == "onRequestStats")
	{
		onRequestStats();
	}
	else if (strFeatureName == "onRequestNameCase")
	{
		onRequestNameCase(getKGM()->getLastUsedValueManager()->getLastUsedValue_NameCase_CaseType(), getKGM()->getLastUsedValueManager()->getLastUsedValue_NameCase_FilenameType());
	}
	else if (strFeatureName == "onRequestCopyEntryData")
	{
		onRequestCopyEntryData(getKGM()->getLastUsedValueManager()->getLastUsedValue_Copy_IMGEntryProperty());
	}
	else if (strFeatureName == "onRequestShift")
	{
		onRequestShift(getKGM()->getLastUsedValueManager()->getLastUsedValue_Shift_Direction());
	}
	else if (strFeatureName == "onRequestQuickExport")
	{
		onRequestQuickExport();
	}
	else if (strFeatureName == "onRequestSelectViaFileExtension")
	{
		onRequestSelectViaFileExtension();
	}
	else if (strFeatureName == "onRequestSelectViaRWVersion")
	{
		onRequestSelectViaRWVersion(getKGM()->getLastUsedValueManager()->getLastUsedValue_Select_RWVersion());
	}
	else if (strFeatureName == "onRequestVersion")
	{
		onRequestVersion();
	}
	else if (strFeatureName == "onRequestTextureList")
	{
		onRequestTextureList();
	}
	else if (strFeatureName == "onRequestAssociateIMGExtension")
	{
		onRequestAssociateIMGExtension();
	}
	else if (strFeatureName == "onRequestSaveLog")
	{
		onRequestSaveLog(getKGM()->getLastUsedValueManager()->getLastUsedValue_SaveLog_ActiveTab(), getKGM()->getLastUsedValueManager()->getLastUsedValue_SaveLog_NormalFormat());
	}
	else if (strFeatureName == "onRequestSaveSession")
	{
		onRequestSaveSession();
	}
	else if (strFeatureName == "onRequestSettings")
	{
		onRequestSettings();
	}
	else if (strFeatureName == "onRequestReopen")
	{
		onRequestReopen();
	}
	else if (strFeatureName == "onRequestConvertDFFToRWVersion")
	{
		onRequestConvertDFFToRWVersion(getKGM()->getLastUsedValueManager()->getLastUsedValue_ConvertDFF_RWVersion());
	}
	else if (strFeatureName == "onRequestMissingTextures")
	{
		onRequestMissingTextures();
	}
	else if (strFeatureName == "onRequestClose2")
	{
		onRequestClose2(getKGM()->getLastUsedValueManager()->getLastUsedValue_Close2_CloseAll());
	}
	else if (strFeatureName == "onRequestReplaceAllFromFolder")
	{
		onRequestReplaceAllFromFolder();
	}
	else if (strFeatureName == "onRequestExportAllEntriesFromAllTabs")
	{
		onRequestExportAllEntriesFromAllTabs();
	}
	else if (strFeatureName == "onRequestExportEntriesViaIDEFileFromAllTabs")
	{
		onRequestExportEntriesViaIDEFileFromAllTabs();
	}
	else if (strFeatureName == "onRequestExportEntriesViaTextLinesFromAllTabs")
	{
		onRequestExportEntriesViaTextLinesFromAllTabs();
	}
	else if (strFeatureName == "onRequestImportViaFolder")
	{
		onRequestImportViaFolder();
	}
	else if (strFeatureName == "onRequestDuplicateEntries")
	{
		onRequestDuplicateEntries();
	}
	else if (strFeatureName == "onRequestExportAllEntriesFromAllTabsIntoMultipleFolders")
	{
		onRequestExportAllEntriesFromAllTabsIntoMultipleFolders();
	}
	else if (strFeatureName == "onRequestOpenLast")
	{
		onRequestOpenLast();
	}
	else if (strFeatureName == "onRequestConvertTXDToGame")
	{
		onRequestConvertTXDToGame(getKGM()->getLastUsedValueManager()->getLastUsedValue_ConvertTXD_Game());
	}
	else if (strFeatureName == "onRequestConvertTXDToRWVersion")
	{
		onRequestConvertTXDToRWVersion(getKGM()->getLastUsedValueManager()->getLastUsedValue_ConvertTXD_RWVersion());
	}
	else if (strFeatureName == "onRequestDump")
	{
		onRequestDump();
	}
	else if (strFeatureName == "onRequestSessionManager")
	{
		onRequestSessionManager();
	}
	else if (strFeatureName == "onRequestWebsite")
	{
		onRequestWebsite();
	}
	else if (strFeatureName == "onRequestOpenLogBasic")
	{
		onRequestOpenLogBasic();
	}
	else if (strFeatureName == "onRequestOpenLogExtended")
	{
		onRequestOpenLogExtended();
	}
	else if (strFeatureName == "onRequestOpenLogFolder")
	{
		onRequestOpenLogFolder();
	}
	else if (strFeatureName == "onRequestProcessLSTFile")
	{
		onRequestProcessLSTFile();
	}
	else if (strFeatureName == "onRequestSelectViaIDE")
	{
		onRequestSelectViaIDE();
	}
	else if (strFeatureName == "onRequestExportViaIPLFile")
	{
		onRequestExportViaIPLFile();
	}
	else if (strFeatureName == "onRequestRenameIMG")
	{
		onRequestRenameIMG();
	}
	else if (strFeatureName == "onRequestUpdate")
	{
		onRequestUpdate();
	}
	else if (strFeatureName == "onRequestAutoUpdate")
	{
		onRequestAutoUpdate();
	}
	else if (strFeatureName == "onRequestSaveIMGSignature")
	{
		onRequestSaveIMGSignature();
	}
	else if (strFeatureName == "onRequestVerifyIMGSignature")
	{
		onRequestVerifyIMGSignature();
	}
	else if (strFeatureName == "onRequestCompareIMG")
	{
		onRequestCompareIMG();
	}
	else if (strFeatureName == "onRequestConvertTXDToTextureFormat")
	{
		onRequestConvertTXDToTextureFormat(getKGM()->getLastUsedValueManager()->getLastUsedValue_ConvertTXD_TextureFormat());
	}
	else if (strFeatureName == "onRequestClearLogs")
	{
		onRequestClearLogs(getKGM()->getLastUsedValueManager()->getLastUsedValue_ClearLogs_AllTabs());
	}
	else if (strFeatureName == "onRequestValidateAllDFFInActiveTab")
	{
		onRequestValidateAllDFFInActiveTab();
	}
	else if (strFeatureName == "onRequestValidateAllTXDInActiveTab")
	{
		onRequestValidateAllTXDInActiveTab();
	}
	else if (strFeatureName == "onRequestCredits")
	{
		onRequestCredits();
	}
	else if (strFeatureName == "onRequestEntryViewer")
	{
		onRequestEntryViewer();
	}
	else if (strFeatureName == "onRequestRenamer")
	{
		onRequestRenamer();
	}
	else if (strFeatureName == "onRequestClearRecentlyOpenedList")
	{
		onRequestClearRecentlyOpenedList();
	}
	else if (strFeatureName == "onRequestBuildTXD")
	{
		onRequestBuildTXD();
	}
	else if (strFeatureName == "onRequestIMGVersionSettings")
	{
		onRequestIMGVersionSettings();
	}
	else if (strFeatureName == "onRequestConvertCOLtoCOLVersion")
	{
		onRequestConvertCOLtoCOLVersion(getKGM()->getLastUsedValueManager()->getLastUsedValue_ConvertCOL_COLVersion());
	}
	else if (strFeatureName == "onRequestReportIssueOrIdea")
	{
		onRequestReportIssueOrIdea();
	}
	else if (strFeatureName == "onRequestSortViaColumn")
	{
		onRequestSortViaColumn(getKGM()->getLastUsedValueManager()->getLastUsedValue_SortViaColumn_Column());
	}
	else if (strFeatureName == "onRequestCenterCOLCollisionMeshes")
	{
		onRequestCenterCOLCollisionMeshes();
	}
	else if (strFeatureName == "onRequestAlignCOLCollisionMeshesToDFFMesh")
	{
		onRequestAlignCOLCollisionMeshesToDFFMesh();
	}
	else if (strFeatureName == "onRequestConvertDFFFileToWDRFile")
	{
		onRequestConvertDFFFileToWDRFile();
	}
	else if (strFeatureName == "onRequestTXDOrganizer")
	{
		onRequestTXDOrganizer();
	}
	else if (strFeatureName == "onRequestConvertWTDFileToTXDFile")
	{
		onRequestConvertWTDFileToTXDFile();
	}
	else if (strFeatureName == "onRequestDATPathsMover")
	{
		onRequestDATPathsMover();
	}
	else if (strFeatureName == "onRequestExportViaDATFile")
	{
		onRequestExportViaDATFile();
	}
	else if (strFeatureName == "onRequestMapMoverAndIDShifter")
	{
		onRequestMapMoverAndIDShifter();
	}
	else if (strFeatureName == "onRequestDATModelList")
	{
		onRequestDATModelList();
	}
	else if (strFeatureName == "onRequestFindTXDMissingFromIMGFoundInIDE")
	{
		onRequestFindTXDMissingFromIMGFoundInIDE();
	}
	else if (strFeatureName == "onRequestFindCOLMissingFromCOLFoundInIDE")
	{
		onRequestFindCOLMissingFromCOLFoundInIDE();
	}
	else if (strFeatureName == "onRequestFindDFFMissingFromIMGFoundInIDE")
	{
		onRequestFindDFFMissingFromIMGFoundInIDE();
	}
	else if (strFeatureName == "onRequestCloneIMG")
	{
		onRequestCloneIMG();
	}
	else if (strFeatureName == "onRequestOpenIMGFolder")
	{
		onRequestOpenIMGFolder();
	}
	else if (strFeatureName == "onRequestRemoveOrphanTexturesFromModel")
	{
		onRequestRemoveOrphanTexturesFromModel();
	}
	else if (strFeatureName == "onRequestNewWindow")
	{
		onRequestNewWindow();
	}
	else if (strFeatureName == "onRequestOrphanDFFEntriesNotInCOL")
	{
		onRequestOrphanDFFEntriesNotInCOL();
	}
	else if (strFeatureName == "onRequestOrphanIDEEntriesNotInCOL")
	{
		onRequestOrphanIDEEntriesNotInCOL();
	}
	else if (strFeatureName == "onRequestOrphanCOLEntriesNotInIDE")
	{
		onRequestOrphanCOLEntriesNotInIDE();
	}
	else if (strFeatureName == "onRequestOrphanDFFEntriesNotInIDE")
	{
		onRequestOrphanDFFEntriesNotInIDE();
	}
	else if (strFeatureName == "onRequestOrphanIMGEntriesNotInIDE")
	{
		onRequestOrphanIMGEntriesNotInIDE();
	}
	else if (strFeatureName == "onRequestOrphanIPLEntriesNotInIDE")
	{
		//onRequestOrphanIPLEntriesNotInIDE();
	}
	else if (strFeatureName == "onRequestOrphanTXDEntriesNotInIDE")
	{
		onRequestOrphanTXDEntriesNotInIDE();
	}
	else if (strFeatureName == "onRequestOrphanIDEEntriesNotInIMG")
	{
		onRequestOrphanIDEEntriesNotInIMG();
	}
	else if (strFeatureName == "onRequestFindDFFMissingFromIDEFoundInIPL")
	{
		onRequestFindDFFMissingFromIDEFoundInIPL();
	}

	else
	{
	}
}

void		CTaskDispatchManager::onRequestLastFeatureUsed(void)
{
	string strPreviousTaskName = getKGM()->getTaskManager()->getPreviousTaskName();
	if (strPreviousTaskName == "")
	{
		return;
	}

	onRequestFeatureByName(strPreviousTaskName);
}

void		CTaskDispatchManager::onRequestConvertCOLtoCOLVersion(CCOLVersion *pCOLVersion)
{
	/*
	todo
	getKGM()->getLastUsedValueManager()->setLastUsedValue_ConvertCOL_COLVersion(pCOLVersion);
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertCOLtoCOLVersion");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertCOLtoCOLVersion", true);
		return;
	}

	// fetch selected entries
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertCOLtoCOLVersion", true);
		return;
	}
	uint32 uiEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount());

	CIMGEntry *pIMGEntry = nullptr;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) != "COL")
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pCOLFile->doesHaveError())
		{
			pCOLFile->unload();
			delete pCOLFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		for (auto pCOLEntry : pCOLFile->getEntries())
		{
			pCOLEntry->setCOLVersion(pCOLVersion);
		}

		string strNewEntryData = pCOLFile->storeViaMemory();
		pCOLFile->unload();
		delete pCOLFile;

		pIMGEntry->setEntryData(strNewEntryData);

		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
		uiEntryCount++;
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_123", uiEntryCount, pCOLVersion->getText().c_str()));

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertCOLtoCOLVersion");
	*/
}

void			CTaskDispatchManager::onRequestReportIssueOrIdea(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestReportIssueOrIdea");
	ShellExecute(NULL, L"open", L"http://mvec.io/todo/project/4", NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskEnd("onRequestReportIssueOrIdea");
}

uint32 uiSortPreviousColumnIndex;
bool bSortDirectionIsAscending = true;

int CALLBACK		CTaskDispatchManager::sortMainListView(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	/*
	todo
	int nRetVal = 0;

	CListCtrl *pListCtrl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);

	CIMGEntry *pIMGEntry1 = (CIMGEntry*)lParam1;
	CIMGEntry *pIMGEntry2 = (CIMGEntry*)lParam2;

	switch (lParamSort)
	{
	case 0: // ID
		break;

	case 1: // Type
		nRetVal = strcmp(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry1->getEntryName())).c_str(), CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry2->getEntryName())).c_str());
		break;

	case 2: // Name
		nRetVal = strcmp(pIMGEntry1->getEntryName().c_str(), pIMGEntry2->getEntryName().c_str());
		break;

	case 3: // Offset
		nRetVal = pIMGEntry1->getEntryOffset() - pIMGEntry2->getEntryOffset();
		break;

	case 4: // Size
		nRetVal = pIMGEntry1->getEntrySize() - pIMGEntry2->getEntrySize();
		break;

	case 5: // RW Version / Resource Type
		break;

	default:
		break;
	}

	if (!bSortDirectionIsAscending)
	{
		nRetVal = -nRetVal;
	}

	return nRetVal;
	*/
	return 0;
}

bool sortViaColumn_Type(CIMGEntry *pIMGEntry1, CIMGEntry *pIMGEntry2)
{
	int32 iResult = strcmp(CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry1->getEntryName())).c_str(), CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry2->getEntryName())).c_str());
	if (bSortDirectionIsAscending)
	{
		return iResult < 0;
	}
	else
	{
		return iResult > 0;
	}
}
bool sortViaColumn_Name(CIMGEntry *pIMGEntry1, CIMGEntry *pIMGEntry2)
{
	int32 iResult = strcmp(pIMGEntry1->getEntryName().c_str(), pIMGEntry2->getEntryName().c_str());
	if (bSortDirectionIsAscending)
	{
		return iResult < 0;
	}
	else
	{
		return iResult > 0;
	}
}
bool sortViaColumn_Offset(CIMGEntry *pIMGEntry1, CIMGEntry *pIMGEntry2)
{
	if (bSortDirectionIsAscending)
	{
		return pIMGEntry1->getEntryOffset() < pIMGEntry2->getEntryOffset();
	}
	else
	{
		return pIMGEntry1->getEntryOffset() > pIMGEntry2->getEntryOffset();
	}
}
bool sortViaColumn_Size(CIMGEntry *pIMGEntry1, CIMGEntry *pIMGEntry2)
{
	if (bSortDirectionIsAscending)
	{
		return pIMGEntry1->getEntrySize() < pIMGEntry2->getEntrySize();
	}
	else
	{
		return pIMGEntry1->getEntrySize() > pIMGEntry2->getEntrySize();
	}
}
bool sortViaColumn_ExtraInfo(CIMGEntry *pIMGEntry1, CIMGEntry *pIMGEntry2)
{
	return false;// strcmp(pIMGEntry1->getEntryName().c_str(), pIMGEntry2->getEntryName().c_str());
}

void			CTaskDispatchManager::onRequestSortViaColumn(uint32 uiColumnIndex)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestSortViaColumn");
	if (uiColumnIndex == uiSortPreviousColumnIndex)
	{
		bSortDirectionIsAscending = !bSortDirectionIsAscending;
	}
	CListCtrl *pListCtrl = (CListCtrl*)getKGM()->getDialog()->GetDlgItem(37);
	CIMGFormat *pIMGFile = getKGM()->getEntryListTab()->getIMGFile();
	if (uiColumnIndex == 1) // Type
	{
		std::sort(pIMGFile->getEntries().begin(), pIMGFile->getEntries().end(), sortViaColumn_Type);
	}
	else if (uiColumnIndex == 2) // Name
	{
		std::sort(pIMGFile->getEntries().begin(), pIMGFile->getEntries().end(), sortViaColumn_Name);
	}
	else if (uiColumnIndex == 3) // Offset
	{
		std::sort(pIMGFile->getEntries().begin(), pIMGFile->getEntries().end(), sortViaColumn_Offset);
	}
	else if (uiColumnIndex == 4) // Size
	{
		std::sort(pIMGFile->getEntries().begin(), pIMGFile->getEntries().end(), sortViaColumn_Size);
	}
	else if (uiColumnIndex == 5) // Extra Info
	{
		//std::sort(pIMGFile->getEntries().begin(), pIMGFile->getEntries().end(), sortViaColumn_ExtraInfo);
	}
	pListCtrl->SortItems(sortMainListView, uiColumnIndex);
	getKGM()->getEntryListTab()->reassignEntryIds();
	uiSortPreviousColumnIndex = uiColumnIndex;
	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestSortViaColumn");
	*/
}

void			CTaskDispatchManager::onRequestCenterCOLCollisionMeshes(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestCenterCOLCollisionMeshes");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCenterCOLCollisionMeshes", true);
		return;
	}

	// fetch selected entries
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCenterCOLCollisionMeshes", true);
		return;
	}
	uint32 uiEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());

	CIMGEntry *pIMGEntry = nullptr;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(pIMGEntry->getEntryName())) != "COL")
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pCOLFile->doesHaveError())
		{
			delete pCOLFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		for (auto pCOLEntry : pCOLFile->getEntries())
		{
			if (pCOLEntry->getCollisionMeshVertexCount() > 1)
			{
				vector<CVector3D>& vecVertices = (vector<CVector3D>&) pCOLEntry->getCollisionMeshVertices(); // CVector3D is the same size and data types as TVertex so the cast should work.
				CVector3D vecCollisionMeshCenter = CMathUtility::getPolygonCenter(vecVertices);
				if (vecCollisionMeshCenter != 0.0f)
				{
					pCOLEntry->applyCollisionMeshVerticesOffset(-vecCollisionMeshCenter);
				}
			}
		}

		string strNewEntryData = pCOLFile->storeViaMemory();
		pCOLFile->unload();
		delete pCOLFile;

		pIMGEntry->setEntryData(strNewEntryData);
		
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
		uiEntryCount++;
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_124", uiEntryCount));

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);

	getKGM()->getTaskManager()->onTaskEnd("onRequestCenterCOLCollisionMeshes");
	*/
}

void			CTaskDispatchManager::onRequestAlignCOLCollisionMeshesToDFFMesh(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestAlignCOLCollisionMeshesToDFFMesh");

	// choose DFFs folder
	getKGM()->getTaskManager()->onTaskPause();
	string strDFFFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_8"), getKGM()->getLastUsedDirectory("ALIGNCOLMESHTODFFMESH_DFF"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strDFFFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestAlignCOLCollisionMeshesToDFFMesh", true);
		return;
	}
	strDFFFolderPath = CPathUtility::addSlashToEnd(strDFFFolderPath);
	getKGM()->setLastUsedDirectory("ALIGNCOLMESHTODFFMESH_DFF", strDFFFolderPath);

	// choose COLs folder
	getKGM()->getTaskManager()->onTaskPause();
	string strCOLFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_7"), getKGM()->getLastUsedDirectory("ALIGNCOLMESHTODFFMESH_COL"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strCOLFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestAlignCOLCollisionMeshesToDFFMesh", true);
		return;
	}
	strCOLFolderPath = CPathUtility::addSlashToEnd(strCOLFolderPath);
	getKGM()->setLastUsedDirectory("ALIGNCOLMESHTODFFMESH_COL", strCOLFolderPath);

	// initialize
	unordered_map<string, CVector3D> umapGeometryPolygonCenters;
	vector<string>
		vecFilePaths_DFF = CFileUtility::getFileNamesByExtension(strDFFFolderPath, "dff"),
		vecFilePaths_BSP = CFileUtility::getFileNamesByExtension(strDFFFolderPath, "bsp"),
		vecFilePaths_COL = CFileUtility::getFileNamesByExtension(strCOLFolderPath, "col");

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecFilePaths_DFF.size() + vecFilePaths_COL.size());

	vecFilePaths_DFF = CVectorUtility::combineVectors(vecFilePaths_DFF, vecFilePaths_BSP);
	
	// fetch DFF texture names and mesh centers
	for (string strFilePath : vecFilePaths_DFF)
	{
		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaFile(strFilePath);
		if(pDFFFile->doesHaveError())
		{
			pDFFFile->unload();
			delete pDFFFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		
		vector<CRWSection*> vecDFFGeometrySections = pDFFFile->getSectionsByType(RW_SECTION_GEOMETRY);
		for (CRWSection *pRWSection : vecDFFGeometrySections)
		{
			CRWSection_Geometry *pGeometry = (CRWSection_Geometry*) pRWSection;

			string strTextureDiffuseName = pGeometry->getTextureDiffuseName();
			vector<CVector3D> vecVertices = pGeometry->getVertexPositions();
			CVector3D vecPolygonCenter = CMathUtility::getPolygonCenter(vecVertices);
			umapGeometryPolygonCenters[CStringUtility::toUpperCase(strTextureDiffuseName)] = vecPolygonCenter;
		}

		pDFFFile->unload();
		delete pDFFFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// check if COL mesh centers are the same as DFF mesh centers, if not then align the COLs to match DFFs
	for (string strFilePath : vecFilePaths_COL)
	{
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strFilePath);
		if(pCOLFile->doesHaveError())
		{
			pCOLFile->unload();
			delete pCOLFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		pCOLFile->setFilePath(strFilePath);

		for (auto pCOLEntry : pCOLFile->getEntries())
		{
			if (umapGeometryPolygonCenters.find(CStringUtility::toUpperCase(pCOLEntry->getModelName())) == umapGeometryPolygonCenters.end())
			{
				continue;
			}
			CVector3D vecDFFPolygonCenter = umapGeometryPolygonCenters[CStringUtility::toUpperCase(pCOLEntry->getModelName())];

			if (pCOLEntry->getCollisionMeshVertexCount() > 1)
			{
				vector<CVector3D>& vecVertices = (vector<CVector3D>&) pCOLEntry->getCollisionMeshVertices(); // CVector3D is the same size and data types as TVertex so the cast should work.
				CVector3D vecCollisionMeshCenter = CMathUtility::getPolygonCenter(vecVertices);
				if (vecCollisionMeshCenter != vecDFFPolygonCenter)
				{
					pCOLEntry->applyCollisionMeshVerticesOffset(-(vecCollisionMeshCenter - vecDFFPolygonCenter));
				}
			}
		}

		pCOLFile->serializeViaFile();

		pCOLFile->unload();
		delete pCOLFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}
	
	string strLogText = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_AlignMeshes_COL_DFF", vecFilePaths_COL.size());
	if (getKGM()->getEntryListWindow()->getEntryCount() > 0)
	{
		getKGM()->getEntryListTab()->log(strLogText);
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(strLogText);
	}

	getKGM()->getTaskManager()->onTaskEnd("onRequestAlignCOLCollisionMeshesToDFFMesh");
}

void			CTaskDispatchManager::onRequestConvertDFFFileToWDRFile(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertDFFFileToWDRFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFFileToWDRFile", true);
		return;
	}

	// fetch selected entries
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFFileToWDRFile", true);
		return;
	}
	uint32 uiEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());

	CIMGEntry *pIMGEntry = nullptr;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (!pIMGEntry->isModelFile())
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pDFFFile->doesHaveError())
		{
			pDFFFile->unload();
			delete pDFFFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CIntermediateModelFormat *pGeneralModelFile = pDFFFile->convertToIntermediateModelFormat();
		CWDRFormat *pWDRFile = CWDRManager::getInstance()->convertIntermediateModelFileToWDRFile(pGeneralModelFile);
		pGeneralModelFile->unload();
		delete pGeneralModelFile;
		string strWDRFileData = pWDRFile->serializeViaMemory();
		pWDRFile->unload();
		delete pWDRFile;

		pIMGEntry->setEntryName(CPathUtility::replaceFileExtension(pIMGEntry->getEntryName(), "wdr"));
		pIMGEntry->setEntryData(strWDRFileData);
		
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);
		
		pDFFFile->unload();
		delete pDFFFile;
		
		getKGM()->getTaskManager()->onTaskProgressTick();
		uiEntryCount++;
	}

	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_Convert_DFF_WDR", uiEntryCount));

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertDFFFileToWDRFile");
	*/
}

void				CTaskDispatchManager::onRequestTXDOrganizer(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestTXDOrganizer");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestTXDOrganizer", true);
		return;
	}

	CTXDOrganizerDialogData *pTXDOrganizerDialogData = getKGM()->getGUIManager()->showTXDOrganizerDialog();

	if (!pTXDOrganizerDialogData->m_bOrganize)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestTXDOrganizer", true);
		return;
	}

	vector<string> vecFilePaths;
	if (pTXDOrganizerDialogData->m_bIDEUpdate)
	{
		vecFilePaths = CFileUtility::getFilePaths(pTXDOrganizerDialogData->m_strIDEUpdateFolderPath, true);
	}

	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetItemCount() + (pTXDOrganizerDialogData->m_bIDEUpdate ? vecFilePaths.size() : 0));

	uint32 uiTXDCount = 0;
	CTXDFormat *pTXDFile = CTXDManager::getInstance()->createFormat();
	pTXDFile->setDeviceId(0);
	pTXDFile->setRWVersion(CRWManager::getInstance()->getVersionManager()->getEntryByVersionId(RW_VERSION_3_6_0_3));

	CIMGEntry *pIMGEntry = nullptr;
	uint32
		uiEntryCount = 0,
		uiDFFFileCount = 0;
	unordered_map<string, string> umapNewEntryNames;
	unordered_map<string, bool> umapTextureNamesUsedInTXD;

	for (uint32 i = 0, j = pListControl->GetItemCount(); i < j; i++)
	{
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(i);

		if (!pIMGEntry->isModelFile())
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
		if (pDFFFile->doesHaveError())
		{
			pDFFFile->unload();
			delete pDFFFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		vector<string> vecDFFTextureNames = pDFFFile->getTextureNames();
		for (string strDFFTextureName : vecDFFTextureNames)
		{
			// check if texture is already in TXD
			if (umapTextureNamesUsedInTXD.find(CStringUtility::toUpperCase(strDFFTextureName)) != umapTextureNamesUsedInTXD.end())
			{
				continue;
			}

			// check if texture file exists
			string strTextureImagePath = CFileUtility::findImageFile(strDFFTextureName, pTXDOrganizerDialogData->m_strTextureImportFolderPath);
			if (strTextureImagePath != "")
			{
				// add texture to TXD
				umapTextureNamesUsedInTXD[CStringUtility::toUpperCase(strDFFTextureName)] = true;
				CRWSection_TextureNative *pTexture = pTXDFile->addTextureViaFile(strTextureImagePath, strDFFTextureName, strDFFTextureName + "a");

				if (pTexture != nullptr)
				{
					vector<string> vecMipmapsRemoved;
					//CDebugger::log("pTXDOrganizerDialogData->m_pTextureFormat->getId(): " + CStringUtility::toString(pTXDOrganizerDialogData->m_pTextureFormat->getId()));
					pTexture->convertToRasterDataFormat(pTXDOrganizerDialogData->m_pRasterDataFormat->getRasterDataFormatId(), vecMipmapsRemoved);
				}
			}
		}

		string strNextTXDFileName = pTXDOrganizerDialogData->m_strTXDNamePrefix + ((uiTXDCount + 1) < 10 ? "0" : "") + CStringUtility::toString(uiTXDCount + 1) + ".txd";
		string strDFFFileNameWithoutExtension = CPathUtility::removeFileExtension(pIMGEntry->getEntryName());
		string strTXDFileNameWithoutExtension = CPathUtility::removeFileExtension(strNextTXDFileName);
		umapNewEntryNames[strDFFFileNameWithoutExtension] = strTXDFileNameWithoutExtension;

		uiDFFFileCount++;
		if (uiDFFFileCount == pTXDOrganizerDialogData->m_uiEveryNDFFFiles)
		{
			uiTXDCount++;
			string strTXDFileName = pTXDOrganizerDialogData->m_strTXDNamePrefix + (uiTXDCount < 10 ? "0" : "") + CStringUtility::toString(uiTXDCount) + ".txd";
			pTXDFile->storeViaFile(pTXDOrganizerDialogData->m_strOutputFolderPath + strTXDFileName);
			pTXDFile->unload();
			delete pTXDFile;
			pTXDFile = nullptr;
			
			uiDFFFileCount = 0;
			umapTextureNamesUsedInTXD.clear();
			pTXDFile = CTXDManager::getInstance()->createFormat();
			pTXDFile->setDeviceId(0);
			pTXDFile->setRWVersion(CRWManager::getInstance()->getVersionManager()->getEntryByVersionId(RW_VERSION_3_6_0_3));
		}
		
		pDFFFile->unload();
		delete pDFFFile;
		
		getKGM()->getTaskManager()->onTaskProgressTick();
		uiEntryCount++;
	}

	if (uiDFFFileCount > 0)
	{
		uiTXDCount++;
		string strTXDFileName = pTXDOrganizerDialogData->m_strTXDNamePrefix + (uiTXDCount < 10 ? "0" : "") + CStringUtility::toString(uiTXDCount) + ".txd";
		pTXDFile->storeViaFile(pTXDOrganizerDialogData->m_strOutputFolderPath + strTXDFileName);
		pTXDFile->unload();
		delete pTXDFile;
		pTXDFile = nullptr;
	}
	
	if(pTXDFile != nullptr)
	{
		pTXDFile->unload();
		delete pTXDFile;
		pTXDFile = nullptr;
	}

	// update IDE files
	if (pTXDOrganizerDialogData->m_bIDEUpdate)
	{
		for (string strFilePath : vecFilePaths)
		{
			if (CStringUtility::toUpperCase(CPathUtility::getFileExtension(strFilePath)) != "IDE")
			{
				continue;
			}

			CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strFilePath);

			if (pIDEFile->doesHaveError())
			{
				getKGM()->getTaskManager()->onTaskProgressTick();
				continue;
			}

			for (auto it : umapNewEntryNames)
			{
				string strDFFFileName = it.first;
				string strTXDFileName = it.second;
				vector<CIDEEntry*> vecIDEEntries = pIDEFile->getEntriesByModelName(strDFFFileName);

				for (CIDEEntry *pIDEEntry : vecIDEEntries)
				{
					switch (pIDEEntry->getSectionType())
					{
						// todo - make it work with all IDE sections
					case IDE_SECTION_OBJS:
					{
						CIDEEntry_OBJS *pIDEEntry_OBJS = (CIDEEntry_OBJS*) pIDEEntry;
						pIDEEntry_OBJS->setTXDName(strTXDFileName);
						break;
					}
					case IDE_SECTION_TOBJ:
					{
						CIDEEntry_TOBJ *pIDEEntry_TOBJ = (CIDEEntry_TOBJ*)pIDEEntry;
						pIDEEntry_TOBJ->setTXDName(strTXDFileName);
						break;
					}
					}
				}
			}

			pIDEFile->serializeViaFile();

			pIDEFile->unload();
			delete pIDEFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	// log
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_TXDOrganizer", uiEntryCount, uiTXDCount));

	// clean up
	delete pTXDOrganizerDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestTXDOrganizer");
	*/
}

void			CTaskDispatchManager::onRequestConvertWTDFileToTXDFile(void)
{
	/*
	todo
	getKGM()->getTaskManager()->onTaskBegin("onRequestConvertWTDFileToTXDFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertWTDFileToTXDFile", true);
		return;
	}

	// fetch selected entries
	CListCtrl *pListControl = ((CListCtrl*)getKGM()->getDialog()->GetDlgItem(37));
	POSITION pos = pListControl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestConvertWTDFileToTXDFile", true);
		return;
	}
	uint32 uiEntryCount = 0;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pListControl->GetSelectedCount());

	CIMGEntry *pIMGEntry = nullptr;
	while (pos)
	{
		int nItem = pListControl->GetNextSelectedItem(pos);
		pIMGEntry = (CIMGEntry*)pListControl->GetItemData(nItem);

		if (!pIMGEntry->isWTDFile())
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		string strEntryData = pIMGEntry->getEntryData();
		CWTDFormat *pWTDFile = CWTDManager::getInstance()->parseViaMemory(strEntryData);
		if (pWTDFile->doesHaveError())
		{
			pWTDFile->unload();
			delete pWTDFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CIntermediateTextureFormat *pGeneralTextureFile = pWTDFile->convertToIntermediateFormat();
		pWTDFile->unload();
		delete pWTDFile;
		CTXDFormat *pTXDFile = CTXDManager::getInstance()->convertIntermediateTextureFileToTXDFile(pGeneralTextureFile);
		pGeneralTextureFile->unload();
		delete pGeneralTextureFile;
		string strTXDFileData = pTXDFile->serializeViaMemory();
		pTXDFile->unload();
		delete pTXDFile;

		pIMGEntry->setEntryName(CPathUtility::replaceFileExtension(pIMGEntry->getEntryName(), "txd"));
		pIMGEntry->setEntryData(strTXDFileData);
		
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		getKGM()->getTaskManager()->onTaskProgressTick();
		uiEntryCount++;
	}

	getKGM()->getEntryListTab()->log("Converted " + CStringUtility::toString(uiEntryCount) + " WTD file" + (uiEntryCount == 1 ? "" : "s") + " to TXD file" + (uiEntryCount == 1 ? "" : "s") + ".");

	getKGM()->getEntryListTab()->setIMGModifiedSinceRebuild(true);
	getKGM()->getTaskManager()->onTaskEnd("onRequestConvertWTDFileToTXDFile");
	*/
}

bool			sortDATFiles(CDATPathFormat *pDATFile_Paths1, CDATPathFormat *pDATFile_Paths2)
{
	return pDATFile_Paths1->m_uiFileIndex < pDATFile_Paths2->m_uiFileIndex;
}
bool			sortDATPathsEntries(CDATEntry_Paths_General_PathNode &pathNode1, CDATEntry_Paths_General_PathNode& pathNode2)
{
	if (pathNode1.m_bIsVehiclePathNode && pathNode2.m_bIsVehiclePathNode)
	{
		return false;
	}
	if (pathNode1.m_bIsVehiclePathNode)
	{
		return true;
	}
	return false;
}
void			CTaskDispatchManager::onRequestDATPathsMover(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestDATPathsMover");
	
	getKGM()->getTaskManager()->onTaskPause();
	CDATPathsMoverDialogData *pDATPathsMoverDialogData = getKGM()->getGUIManager()->showDATPathsMoverDialogData();
	getKGM()->getTaskManager()->onTaskUnpause();

	if (!pDATPathsMoverDialogData->m_bMove)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestDATPathsMover", true);
		return;
	}

	vector<string>
		vecFileNames = CFileUtility::getFileNamesByExtension(pDATPathsMoverDialogData->m_strInputFolderPath, "DAT");
	vector<CDATPathFormat*>
		vecDATInputFiles,
		vecDATOutputFiles;
	for (auto strFileName : vecFileNames)
	{
		uint32 uiAreaId = CStringUtility::toNumber(CPathUtility::removeFileExtension(strFileName).substr(5)); // example filename: nodes0.dat, nodes1.dat, nodes63.dat

		CDATPathFormat *pDATFile = CDATPathManager::getInstance()->parseViaMemory(CFileUtility::getFileContent(pDATPathsMoverDialogData->m_strInputFolderPath + strFileName, true)/*, uiAreaId*/);
		if(!pDATFile->doesHaveError())
		{
			vecDATInputFiles.push_back(pDATFile);
		}
	}
	for (uint32 i = 0, j = CDATPathManager::getTileCount(pDATPathsMoverDialogData->m_vecOutputMapRangeMin, pDATPathsMoverDialogData->m_vecOutputMapRangeMax, pDATPathsMoverDialogData->m_vecOutputTileSize); i < j; i++)
	{
		CDATPathFormat *pDATFile = new CDATPathFormat;
		pDATFile->setPathsFormat(DAT_PATH_FASTMAN92);
		pDATFile->m_uiFileIndex = i;
		vecDATOutputFiles.push_back(pDATFile);
	}
	
	std::sort(vecDATInputFiles.begin(), vecDATInputFiles.end(), sortDATFiles);

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount((vecDATInputFiles.size() * 2) + (vecDATOutputFiles.size() * 4));

	for (auto pDATFile : vecDATInputFiles)
	{
		pDATFile->applyOffsetToPositions(pDATPathsMoverDialogData->m_vecPositionOffset);

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// process
	//unordered_map<CDATEntry_Paths_General_PathNode&, CNodeAddress> umapNewAddresses_PathNode;
	for (auto pDATFile : vecDATInputFiles)
	{
		uint32 uiAreaId_SA = pDATFile->m_uiFileIndex;

		for (auto& pathNode : pDATFile->m_vecPathNodes)
		{
			CVector2D vecPathNodePosition = { (float32)pathNode.m_vecPosition.x / 8.0f, (float32)pathNode.m_vecPosition.y / 8.0f };
			uint32 uiNewAreaIdForPathNode = CDATPathManager::getAreaIdFromPosition(vecPathNodePosition, pDATPathsMoverDialogData->m_vecOutputMapRangeMin, pDATPathsMoverDialogData->m_vecOutputMapRangeMax, pDATPathsMoverDialogData->m_vecOutputTileSize);

			uint32 uiFirstLinkIndex = pathNode.m_wConnectedNodesStartId;
			uint32 uiLinkCountForNode = pathNode.m_uiFlags & 0xF;
			pathNode.m_wConnectedNodesStartId = vecDATOutputFiles[uiNewAreaIdForPathNode]->m_vecLinks.size();

			CDATEntry_Paths_General_Link link;
			for (uint32 i = 0; i < uiLinkCountForNode; i++)
			{
				link = pDATFile->m_vecLinks[uiFirstLinkIndex + i];
				vecDATOutputFiles[uiNewAreaIdForPathNode]->addLink(link);
			}
			//uint32 uiNewPathNodeId = vecDATOutputFiles[uiNewAreaIdForPathNode].size();
			vecDATOutputFiles[uiNewAreaIdForPathNode]->addPathNode(pathNode);
			
			//umapNewAddresses_PathNode[vecDATOutputFiles[uiNewAreaIdForPathNode]->m_vecPathNodes[uiNewPathNodeId]] = { uiNewAreaIdForPathNode, uiNewPathNodeId };
		}

		uint32 i = 0;
		for (auto& naviNode : pDATFile->m_vecNaviNodes)
		{
			CVector2D vecNaviNodePosition = { (float32)naviNode.m_iPosition[0] / 8.0f, (float32)naviNode.m_iPosition[1] / 8.0f };
			uint32 uiNewAreaIdForNaviNode = CDATPathManager::getAreaIdFromPosition(vecNaviNodePosition, pDATPathsMoverDialogData->m_vecOutputMapRangeMin, pDATPathsMoverDialogData->m_vecOutputMapRangeMax, pDATPathsMoverDialogData->m_vecOutputTileSize);

			vecDATOutputFiles[uiNewAreaIdForNaviNode]->addNaviNode(naviNode);
			i++;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	for (auto pDATFile : vecDATOutputFiles)
	{
		std::sort(pDATFile->m_vecPathNodes.begin(), pDATFile->m_vecPathNodes.end(), sortDATPathsEntries);

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	for (auto pDATFile : vecDATOutputFiles)
	{
		for (auto& naviNode : pDATFile->m_vecNaviNodes)
		{
			// this code is in a separate loop because the new position may not have been applied yet.
			
			auto& targetPathNode_Input = vecDATInputFiles[naviNode.m_usTargetNode_AreaId]->m_vecPathNodes[naviNode.m_usTargetNode_NodeId]; // the position has already been updated for this target node.

			uint32 uiAreaId_Out;
			uint32 uiNodeId_Out;
			bool bFound = CDATPathManager::findPathNode(vecDATOutputFiles, targetPathNode_Input, uiAreaId_Out, uiNodeId_Out);
			if (bFound)
			{
				naviNode.m_usTargetNode_AreaId = uiAreaId_Out;
				naviNode.m_usTargetNode_NodeId = uiNodeId_Out;
			}
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}
	
	for (auto pDATFile : vecDATOutputFiles)
	{
		for (auto& link : pDATFile->m_vecLinks)
		{
			auto& pathNode_Input = vecDATInputFiles[link.m_usAreaId]->m_vecPathNodes[link.m_usNodeId];

			uint32 uiAreaId_Out;
			uint32 uiNodeId_Out;
			bool bFound = CDATPathManager::findPathNode(vecDATOutputFiles, pathNode_Input, uiAreaId_Out, uiNodeId_Out);
			if (bFound)
			{
				link.m_usAreaId = uiAreaId_Out;
				link.m_usNodeId = uiNodeId_Out;
			}



			if (link.m_usNaviAreaId == 0 && link.m_usNaviNodeId == 0)
			{
				// do nothing
			}
			else
			{
				auto& naviNode_Input2 = vecDATInputFiles[link.m_usNaviAreaId]->m_vecNaviNodes[link.m_usNaviNodeId];

				uint32 uiAreaId_Out2;
				uint32 uiNodeId_Out2;
				bool bFound2 = CDATPathManager::findNaviNode(vecDATOutputFiles, naviNode_Input2, uiAreaId_Out2, uiNodeId_Out2);
				if (bFound2)
				{
					link.m_usNaviAreaId = uiAreaId_Out2;
					link.m_usNaviNodeId = uiNodeId_Out2;
				}
			}
		}

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// update target nodes of navi nodes to always be the "lower" node
	/*
	for (auto pDATFile : vecDATOutputFiles)
	{
		uint32 uiNaviNodeAreaId = pDATFile->m_uiFileIndex;
		uint32 uiNaviNodeNodeId = 0;
		for (auto& naviNode : pDATFile->m_vecNaviNodes)
		{
			CDATEntry_Paths_General_PathNode pathNode1 = vecDATOutputFiles[naviNode.m_usTargetNode_AreaId]->m_vecPathNodes[naviNode.m_usTargetNode_NodeId];
			uint32 uiLinkId_Out;
			vector<CDATEntry_Paths_General_PathNode> vecTargetPathNodes = CDATManager::getTargetPathNodesForNaviNode(vecDATOutputFiles, naviNode, uiNaviNodeAreaId, uiNaviNodeNodeId, uiLinkId_Out);
			vecTargetPathNodes.push_back(pathNode1);
			//if (vecTargetPathNodes.size() != 2)
			//{
			//	CDebugger::log("vecTargetPathNodes.size(): " + CStringUtility::toString(vecTargetPathNodes.size()));
			//}

			//CDATEntry_Paths_General_PathNode lowestTargetPathNode = CDATManager::getLowestPathNode(vecTargetPathNodes);
			//naviNode.m_usTargetNode_AreaId = lowestTargetPathNode.m_wAreaId;
			//naviNode.m_usTargetNode_NodeId = lowestTargetPathNode.m_wNodeId;

			CDATEntry_Paths_General_PathNode lowerTargetPathNode = CDATManager::getLowestPathNode(vecTargetPathNodes);
			CDATEntry_Paths_General_PathNode higherTargetPathNode;
			if (vecTargetPathNodes[0].m_vecPosition.x == lowerTargetPathNode.m_vecPosition.x
			 && vecTargetPathNodes[0].m_vecPosition.y == lowerTargetPathNode.m_vecPosition.y
			 && vecTargetPathNodes[0].m_vecPosition.z == lowerTargetPathNode.m_vecPosition.z
			 && vecTargetPathNodes[0].m_ucPathWidth == lowerTargetPathNode.m_ucPathWidth)
			{
				higherTargetPathNode = vecTargetPathNodes[1];
			}
			else
			{
				higherTargetPathNode = vecTargetPathNodes[0];
			}

			auto& link = vecDATOutputFiles[vecTargetPathNodes[0].m_wAreaId]->m_vecLinks[uiLinkId_Out];
			link.m_usNaviAreaId = higherTargetPathNode.m_wAreaId;
			link.m_usNaviNodeId = higherTargetPathNode.m_wNodeId;
			naviNode.m_usTargetNode_AreaId = lowerTargetPathNode.m_wAreaId;
			naviNode.m_usTargetNode_NodeId = lowerTargetPathNode.m_wNodeId;

			//
			CDATEntry_Paths_General_PathNode previousPathNode = vecTargetPathNodes[0];
			naviNode.m_usTargetNode_AreaId = previousPathNode.m_wAreaId;
			naviNode.m_usTargetNode_NodeId = previousPathNode.m_wNodeId;
			naviNode.m_ucDirection[0] = -((int8)naviNode.m_ucDirection[0]);
			naviNode.m_ucDirection[1] = -((int8)naviNode.m_ucDirection[1]);
			
			uint32 uiLeftLaneCount = naviNode.m_uiFlags & 1792;
			uint32 uiRightLaneCount = naviNode.m_uiFlags & 14336;
			
			uint32 uiNewLeftLaneCount = uiRightLaneCount;
			uint32 uiNewRightLaneCount = uiLeftLaneCount;

			naviNode.m_uiFlags &= 0xFFFFF8FF;
			naviNode.m_uiFlags |= uiNewLeftLaneCount;

			naviNode.m_uiFlags &= 0xFFFFC7FF;
			naviNode.m_uiFlags |= uiNewRightLaneCount;
			//

			uiNaviNodeNodeId++;
		}
	}
	*/

	// store output DAT files
	uint32 i = 0;
	for (auto pDATFile : vecDATOutputFiles)
	{
		CFileUtility::storeFile(pDATPathsMoverDialogData->m_strOutputFolderPath + "nodes" + CStringUtility::toString(i) + ".dat", pDATFile->serializeViaMemory(), false, true);
		getKGM()->getTaskManager()->onTaskProgressTick();
		i++;
	}

	// clean up
	for (auto pDATFile : vecDATInputFiles)
	{
		pDATFile->unload();
		delete pDATFile;
	}
	for (auto pDATFile : vecDATOutputFiles)
	{
		pDATFile->unload();
		delete pDATFile;
	}
	delete pDATPathsMoverDialogData;

	getKGM()->getTaskManager()->onTaskEnd("onRequestDATPathsMover");
}

void			CTaskDispatchManager::onRequestExportViaDATFile(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestExportViaDATFile");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile", true);
		return;
	}

	// choose input DAT GTA files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecDATPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("EXPORTVIADAT_DAT"), "DAT", true);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecDATPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile", true);
		return;
	}
	getKGM()->setLastUsedDirectory("EXPORTVIADAT_DAT", CPathUtility::getDirectory(vecDATPaths[0]));

	// choose input game folder
	getKGM()->getTaskManager()->onTaskPause();
	string strGameFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_3"), getKGM()->getLastUsedDirectory("EXPORTVIADAT_GAME"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecDATPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile", true);
		return;
	}
	strGameFolderPath = CPathUtility::addSlashToEnd(strGameFolderPath);
	getKGM()->setLastUsedDirectory("EXPORTVIADAT_GAME", strGameFolderPath);

	// fetch IDE paths from DAT files
	vector<string> vecIDEPaths;
	for (string strDATPath : vecDATPaths)
	{
		CDATLoaderFormat *pDATFile = CDATLoaderManager::getInstance()->parseViaFile(strDATPath);
		if(!pDATFile->doesHaveError())
		{
			vector<string> vecRelativeIDEPaths = pDATFile->getRelativeIDEPaths();
			for (string strRelativeIDEPath : vecRelativeIDEPaths)
			{
				vecIDEPaths.push_back(strGameFolderPath + strRelativeIDEPath);
			}
		}
		pDATFile->unload();
		delete pDATFile;
	}

	// choose input IDE files to export entries from
	getKGM()->getTaskManager()->onTaskPause();
	string strData = getKGM()->getGUIManager()->showTextAreaDialog(CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_8_Title"), CLocalizationManager::getInstance()->getTranslatedText("Window_TextArea_8_Message"), CStringUtility::join(vecIDEPaths, "\r\n"));
	if (strData == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile", true);
		return;
	}
	getKGM()->getTaskManager()->onTaskUnpause();

	// choose output folder
	getKGM()->getTaskManager()->onTaskPause();
	string strOutputFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_12"), getKGM()->getLastUsedDirectory("EXPORTVIADAT_OUTPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strOutputFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile", true);
		return;
	}
	strOutputFolderPath = CPathUtility::addSlashToEnd(strOutputFolderPath);
	getKGM()->setLastUsedDirectory("EXPORTVIADAT_OUTPUT", strOutputFolderPath);

	// generate input IDE file objects
	vector<string> vecEntryNames;
	vector<string> vecLines = CStringUtility::split(strData, "\r\n");
	vector<CIDEFormat*> veCIDEFormats;
	for (auto strLine : vecLines)
	{
		strLine = CStringUtility::trim(strLine);
		if (strLine == "")
		{
			continue;
		}

		vecEntryNames.push_back(strLine);
		CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strLine);
		if(!pIDEFile->doesHaveError())
		{
			veCIDEFormats.push_back(pIDEFile);
		}
	}
	vecEntryNames = CVectorUtility::toUpperCase(vecEntryNames);

	// fetch IDE entry names
	unordered_map<string, CIDEEntry*> umapIDEEntryNamesUpper;
	for (auto pIDEFile : veCIDEFormats)
	{ // todo - make it work with all IDE sections
		for (auto pIDEEntry : pIDEFile->getEntriesBySection<CIDEEntry_OBJS>(IDE_SECTION_OBJS))
		{
			umapIDEEntryNamesUpper[CStringUtility::toUpperCase(pIDEEntry->getModelName())] = pIDEEntry;
			umapIDEEntryNamesUpper[CStringUtility::toUpperCase(pIDEEntry->getTXDName())] = pIDEEntry;
		}
	}

	vector<CIMGEntry*> vecIMGEntriesToExport;
	vector<string> vecExportedEntryNames;
	for (auto pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		string strEntryNameUpperWithoutExtension = CStringUtility::toUpperCase(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));

		if (umapIDEEntryNamesUpper.find(strEntryNameUpperWithoutExtension) != umapIDEEntryNamesUpper.end())
		{
			vecIMGEntriesToExport.push_back(pIMGEntry);
			vecExportedEntryNames.push_back(pIMGEntry->getEntryName());
		}
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIMGEntriesToExport.size());
	getKGM()->getEntryListTab()->getIMGFile()->exportMultiple(vecIMGEntriesToExport, strOutputFolderPath);
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_Export_DAT", vecIMGEntriesToExport.size()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_77"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecExportedEntryNames, "\n"), true);
	
	for (auto pIDEFile : veCIDEFormats)
	{
		pIDEFile->unload();
		delete pIDEFile;
	}
	
	getKGM()->getTaskManager()->onTaskEnd("onRequestExportViaDATFile");
}

void						CTaskDispatchManager::onRequestMapMoverAndIDShifter(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestMapMoverAndIDShifter");

	CMapMoverAndIDShifterDialogData *pMapMoverAndIDShifterDialogData = getKGM()->getGUIManager()->showMapMoverAndIDShifterDialog();
	if (!pMapMoverAndIDShifterDialogData->m_bGo)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMapMoverAndIDShifter", true);
		return;
	}

	unordered_map<ePlatformedGame, vector<string>>
		umapIgnoreDefaultObjectFileNamesVector;
	string
		strDefaultModelNamesFolder = getKGM()->getInstallationMeta().getProgramFilesPath() + "Default Files/Model Names/",
		strModelNamesPath_PC_GTA_III = strDefaultModelNamesFolder + "Model Names - PC GTA III.txt",
		strModelNamesPath_PC_GTA_VC = strDefaultModelNamesFolder + "Model Names - PC GTA VC.txt",
		strModelNamesPath_PC_GTA_SA = strDefaultModelNamesFolder + "Model Names - PC GTA SA.txt"
	;
	if (CFileUtility::doesFileExist(strModelNamesPath_PC_GTA_III))
	{
		umapIgnoreDefaultObjectFileNamesVector[PLATFORMED_GAME_PC_GTA_III] = CStringUtility::split(CFileUtility::getFileContent(strModelNamesPath_PC_GTA_III), ", ");
	}
	if (CFileUtility::doesFileExist(strModelNamesPath_PC_GTA_VC))
	{
		umapIgnoreDefaultObjectFileNamesVector[PLATFORMED_GAME_PC_GTA_VC] = CStringUtility::split(CFileUtility::getFileContent(strModelNamesPath_PC_GTA_VC), ", ");
	}
	if (CFileUtility::doesFileExist(strModelNamesPath_PC_GTA_SA))
	{
		umapIgnoreDefaultObjectFileNamesVector[PLATFORMED_GAME_PC_GTA_SA] = CStringUtility::split(CFileUtility::getFileContent(strModelNamesPath_PC_GTA_SA), ", ");
	}

	unordered_map<string, bool> umapIgnoreDefaultObjectFileNames;
	for (auto it : umapIgnoreDefaultObjectFileNamesVector)
	{
		for (string& strFileName : it.second)
		{
			umapIgnoreDefaultObjectFileNames[strFileName] = true;
		}
	}
	umapIgnoreDefaultObjectFileNamesVector.clear();

	CDATLoaderFormat *pDATFile = CDATLoaderManager::getInstance()->parseViaFile(pMapMoverAndIDShifterDialogData->m_strDATFilePath);
	if(pDATFile->doesHaveError())
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMapMoverAndIDShifter", true);
		return;
	}
	vector<string>
		vecRelativeIDEPaths = pDATFile->getRelativeIDEPaths(),
		vecRelativeIPLPaths = pDATFile->getRelativeIPLPaths();
	pDATFile->unload();
	delete pDATFile;

	vector<string>
		vecIDEPaths,
		vecIPLPaths;
	uint32 i;
	vecIDEPaths.resize(vecRelativeIDEPaths.size());
	vecIPLPaths.resize(vecRelativeIPLPaths.size());
	i = 0;
	for (string& strRelativeIDEPath : vecRelativeIDEPaths)
	{
		vecIDEPaths[i] = pMapMoverAndIDShifterDialogData->m_strGameFolderPath + strRelativeIDEPath;
		i++;
	}
	i = 0;
	for (string& strRelativeIPLPath : vecRelativeIPLPaths)
	{
		vecIPLPaths[i] = pMapMoverAndIDShifterDialogData->m_strGameFolderPath + strRelativeIPLPath;
		i++;
	}
	vecRelativeIDEPaths.clear();
	vecRelativeIPLPaths.clear();
	if (pMapMoverAndIDShifterDialogData->m_bUpdateIDEInFolder)
	{
		vecIDEPaths = CVectorUtility::combineVectors(vecIDEPaths, CFileUtility::getFilePaths(pMapMoverAndIDShifterDialogData->m_strUpdateIDEInFolderPath, true, false, "IDE"));
	}
	if (pMapMoverAndIDShifterDialogData->m_bUpdateIPLInFolder)
	{
		vecIPLPaths = CVectorUtility::combineVectors(vecIPLPaths, CFileUtility::getFilePaths(pMapMoverAndIDShifterDialogData->m_strUpdateIPLInFolderPath, true, false, "IPL"));
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecIDEPaths.size() + vecIPLPaths.size());

	uint32 uiIDEEntryNewObjectId = pMapMoverAndIDShifterDialogData->m_uiIDStart;
	unordered_map<uint32, uint32> umapNewObjectIds;
	uint32
		uiIPLCount_Text = 0,
		uiIPLCount_Binary = 0;
	if (pMapMoverAndIDShifterDialogData->m_bShiftIds)
	{
		for (string& strIDEPath : vecIDEPaths)
		{
			bool bIgnoreShifter = pMapMoverAndIDShifterDialogData->m_umapShifterIgnoreFilenames.find(CStringUtility::toUpperCase(CPathUtility::getFileName(strIDEPath))) != pMapMoverAndIDShifterDialogData->m_umapShifterIgnoreFilenames.end();
			if (bIgnoreShifter)
			{
				// ignore file for ID shifter
				continue;
			}
			CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strIDEPath);
			if(!pIDEFile->doesHaveError())
			{
				for (auto it : pIDEFile->getSectionEntries())
				{
					for (CIDEEntry *pIDEEntry : it.second)
					{
						if (pIDEEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
						{
							CIDEEntry_Data *pIDEEntry_Data = (CIDEEntry_Data*)pIDEEntry;
							umapNewObjectIds[pIDEEntry_Data->getObjectId()] = uiIDEEntryNewObjectId;
							//pIDEEntry_Data->setObjectId(uiIDEEntryNewObjectId); // todo - must be uncommented
							uiIDEEntryNewObjectId++;
						}
					}
				}

				pIDEFile->setFilePath(pMapMoverAndIDShifterDialogData->m_strOutputFolderPath + CPathUtility::getFileName(strIDEPath));
				pIDEFile->serializeViaFile();
			}
			pIDEFile->unload();
			delete pIDEFile;

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}

	for (string& strIPLPath : vecIPLPaths)
	{
		bool
			bIgnoreShifter = !pMapMoverAndIDShifterDialogData->m_bShiftIds,
			bIgnoreMover = !pMapMoverAndIDShifterDialogData->m_bMovePosition;
		if (!bIgnoreShifter)
		{
			bIgnoreShifter = pMapMoverAndIDShifterDialogData->m_umapShifterIgnoreFilenames.find(CStringUtility::toUpperCase(CPathUtility::getFileName(strIPLPath))) != pMapMoverAndIDShifterDialogData->m_umapShifterIgnoreFilenames.end();
		}
		if (!bIgnoreMover)
		{
			bIgnoreMover = pMapMoverAndIDShifterDialogData->m_umapMoverIgnoreFilenames.find(CStringUtility::toUpperCase(CPathUtility::getFileName(strIPLPath))) != pMapMoverAndIDShifterDialogData->m_umapMoverIgnoreFilenames.end();
		}

		CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strIPLPath);
		if(!pIPLFile->doesHaveError())
		{
			if (pIPLFile->isBinary())
			{
				uiIPLCount_Binary++;
			}
			else
			{
				uiIPLCount_Text++;
			}
			for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_INST>(IPL_SECTION_INST))
			{
				if (umapNewObjectIds.find(pIPLEntry->getObjectId()) != umapNewObjectIds.end())
				{
					if (!bIgnoreShifter)
					{
						pIPLEntry->setObjectId(umapNewObjectIds[pIPLEntry->getObjectId()]);
					}
				}
				if (!bIgnoreMover)
				{
					pIPLEntry->getPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
			}
			if (!bIgnoreMover)
			{
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_AUZO>(IPL_SECTION_AUZO))
				{
					switch (pIPLEntry->getFormatType())
					{
					case 0:
						pIPLEntry->getLowerLeftPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getLowerLeftPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getLowerLeftPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						pIPLEntry->getUpperRightPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getUpperRightPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getUpperRightPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						break;
					case 1:
						pIPLEntry->getCenterPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getCenterPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getCenterPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						break;
					}
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_CARS>(IPL_SECTION_CARS))
				{
					pIPLEntry->getPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_CULL>(IPL_SECTION_CULL))
				{
					switch (pIPLEntry->getFormatType())
					{
					case 1:
						pIPLEntry->getLowerLeftPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getLowerLeftPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getLowerLeftPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						pIPLEntry->getUpperRightPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getUpperRightPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getUpperRightPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						/* no break intended */
					case 0:
					case 2:
						pIPLEntry->getCenterPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
						pIPLEntry->getCenterPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
						pIPLEntry->getCenterPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
						break;
					}
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_ENEX>(IPL_SECTION_ENEX))
				{
					pIPLEntry->getEntrancePosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getEntrancePosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getEntrancePosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getExitPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getExitPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getExitPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_GRGE>(IPL_SECTION_GRGE))
				{
					pIPLEntry->getPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getLine().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getLine().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getCubePosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getCubePosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getCubePosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_JUMP>(IPL_SECTION_JUMP))
				{
					pIPLEntry->getStartLowerPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getStartLowerPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getStartLowerPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getStartUpperPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getStartUpperPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getStartUpperPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getTargetLowerPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getTargetLowerPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getTargetLowerPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getTargetUpperPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getTargetUpperPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getTargetUpperPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getCameraPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getCameraPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getCameraPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_OCCL>(IPL_SECTION_OCCL))
				{
					pIPLEntry->getMidPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getMidPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_PICK>(IPL_SECTION_PICK))
				{
					pIPLEntry->getPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
				for (auto pIPLEntry : pIPLFile->getEntriesBySection<CIPLEntry_ZONE>(IPL_SECTION_ZONE))
				{
					pIPLEntry->getBottomLeftPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getBottomLeftPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getBottomLeftPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
					pIPLEntry->getTopRightPosition().m_x += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_x;
					pIPLEntry->getTopRightPosition().m_y += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_y;
					pIPLEntry->getTopRightPosition().m_z += pMapMoverAndIDShifterDialogData->m_vecPositionOffset.m_z;
				}
			}
			
			pIPLFile->setFilePath(pMapMoverAndIDShifterDialogData->m_strOutputFolderPath + CPathUtility::getFileName(strIPLPath));
			pIPLFile->serializeViaFile();
		}
		
		pIPLFile->unload();
		delete pIPLFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen("Moved and ID shifted " + CStringUtility::toString(vecIDEPaths.size()) + " IDE files and " + CStringUtility::toString(vecIPLPaths.size()) + " IPL files (" + CStringUtility::toString(uiIPLCount_Text) + " text, " + CStringUtility::toString(uiIPLCount_Binary) + " binary) in " + CPathUtility::getFileName(pMapMoverAndIDShifterDialogData->m_strDATFilePath));
	}
	else
	{
		getKGM()->getEntryListTab()->log("Moved and ID shifted " + CStringUtility::toString(vecIDEPaths.size()) + " IDE files and " + CStringUtility::toString(vecIPLPaths.size()) + " IPL files in " + CPathUtility::getFileName(pMapMoverAndIDShifterDialogData->m_strDATFilePath));
	}

	delete pMapMoverAndIDShifterDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestMapMoverAndIDShifter");
}

void						CTaskDispatchManager::onRequestDATModelList(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestModelListFromDAT");

	CDATModelListDialogData *pDATModelListDialogData = getKGM()->getGUIManager()->showDATModelListDialog();
	if (!pDATModelListDialogData->m_bFetch)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestModelListFromDAT", true);
		return;
	}

	CDATLoaderFormat *pDATFile = CDATLoaderManager::getInstance()->parseViaFile(pDATModelListDialogData->m_strDATFilePath);
	if(pDATFile->doesHaveError())
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestModelListFromDAT", true);
		return;
	}
	
	vector<string>
		vecRelativeIDEPaths = pDATFile->getRelativeIDEPaths(),
		vecRelativeIPLPaths = pDATFile->getRelativeIPLPaths();
	pDATFile->unload();
	delete pDATFile;

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecRelativeIDEPaths.size() + vecRelativeIPLPaths.size() + 1);

	vector<string> vecModelNames;
	for (string& strRelativeIDEPath : vecRelativeIDEPaths)
	{
		string strIDEPath = pDATModelListDialogData->m_strGameFolderPath + strRelativeIDEPath;

		CIDEFormat *pIDEFile = CIDEManager::getInstance()->parseViaFile(strIDEPath);
		if(!pIDEFile->doesHaveError())
		{
			vecModelNames = CVectorUtility::combineVectors(vecModelNames, pIDEFile->getModelNames());
		}
		pIDEFile->unload();
		delete pIDEFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	for (string& strRelativeIPLPath : vecRelativeIPLPaths)
	{
		string strIPLPath = pDATModelListDialogData->m_strGameFolderPath + strRelativeIPLPath;

		CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strIPLPath);
		if(!pIPLFile->doesHaveError())
		{
			vecModelNames = CVectorUtility::combineVectors(vecModelNames, pIPLFile->getModelNames());
		}
		pIPLFile->unload();
		delete pIPLFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	vecModelNames = CVectorUtility::removeDuplicates(vecModelNames);
	getKGM()->getTaskManager()->onTaskProgressTick();

	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen("Found " + CStringUtility::toString(vecModelNames.size()) + " unique model names in IDE/IPL files in " + CPathUtility::getFileName(pDATModelListDialogData->m_strDATFilePath));
	}
	else
	{
		getKGM()->getEntryListTab()->log("Found " + CStringUtility::toString(vecModelNames.size()) + " unique model names in IDE/IPL files in " + CPathUtility::getFileName(pDATModelListDialogData->m_strDATFilePath));
	}

	/*
	string strFilePath = CGUIUtility::saveFileDialog(getKGM()->getLastUsedDirectory("DATMODELLIST"), "TXT", "Model Names for " + CPathUtility::removeFileExtension(CPathUtility::getFileName(strDATFilePath)) + ".txt");
	if (strFilePath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestModelListFromDAT", true);
		return;
	}
	getKGM()->setLastUsedDirectory("DATMODELLIST", CPathUtility::getDirectory(strFilePath));
	*/

	CFileUtility::storeFile(pDATModelListDialogData->m_strOutputFilePath, CStringUtility::join(vecModelNames, "\n"), false, false);

	delete pDATModelListDialogData;
	getKGM()->getTaskManager()->onTaskEnd("onRequestModelListFromDAT");
}

void						CTaskDispatchManager::onRequestFindTXDMissingFromIMGFoundInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestFindTXDMissingFromIMGFoundInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindTXDMissingFromIMGFoundInIDE", true);
		return;
	}

	// fetch TXD names in IDE files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IDE_IMG_TXD__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindTXDMissingFromIMGFoundInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IDE_IMG_TXD__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	vector<string> vecTXDNamesWithoutExtensionInIDE = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, false, true);

	// fetch TXD names in active IMG file
	vector<string> vecTXDNamesWithoutExtensionInIMG;
	for (CIMGEntry *pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (pIMGEntry->isTXDFile())
		{
			vecTXDNamesWithoutExtensionInIMG.push_back(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		}
	}
	vecTXDNamesWithoutExtensionInIMG = CVectorUtility::toUpperCase(vecTXDNamesWithoutExtensionInIMG);
	unordered_map<string, bool> umapTXDNamesWithoutExtensionInIMG = CVectorUtility::convertVectorToUnorderedMap(vecTXDNamesWithoutExtensionInIMG);
	vecTXDNamesWithoutExtensionInIMG.clear();

	// find TXD names found in IDE but not found in IMG
	vector<string> vecTXDNamesWithoutExtensionMissingFromIMG;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecTXDNamesWithoutExtensionInIDE.size());
	for (string& strTXDName : vecTXDNamesWithoutExtensionInIDE)
	{
		if (umapTXDNamesWithoutExtensionInIMG.count(CStringUtility::toUpperCase(strTXDName)) == 0)
		{
			vecTXDNamesWithoutExtensionMissingFromIMG.push_back(strTXDName);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	string strIMGFileName = CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath());
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_132", vecTXDNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_135"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecTXDNamesWithoutExtensionMissingFromIMG, "\n"), true);

	// popup
	string strInitialFilename = CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_9_InitialFilename", CPathUtility::replaceFileExtension(CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()), "txt").c_str());
	string strTitle = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_132", vecTXDNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str());
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog("Missing Entries", strTitle, "TXD Name", vecTXDNamesWithoutExtensionMissingFromIMG, strInitialFilename, "MISSINGENTRIES");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestFindTXDMissingFromIMGFoundInIDE");
}
void						CTaskDispatchManager::onRequestFindCOLMissingFromCOLFoundInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestFindCOLMissingFromCOLFoundInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindCOLMissingFromCOLFoundInIDE", true);
		return;
	}

	// fetch DFF names in IDE files (these DFF names will be compared with COL names)
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IDE_COL_COL__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindCOLMissingFromCOLFoundInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IDE_COL_COL__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	vector<string> vecDFFNamesWithoutExtensionInIDE = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);

	// fetch COL names in COL file
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecCOLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IDE_COL_COL__COL"), "COL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecCOLPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestMissingCOLEntriesInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IDE_COL_COL__COL", CPathUtility::getDirectory(vecCOLPaths[0]));

	vector<string> vecCOLNamesWithoutExtensionInCOL;
	for (string& strCOLPath : vecCOLPaths)
	{
		CCOLFormat *pCOLFile = CCOLManager::getInstance()->parseViaFile(strCOLPath);
		if (!pCOLFile->doesHaveError())
		{
			for (CCOLEntry *pCOLEntry : pCOLFile->getEntries())
			{
				vecCOLNamesWithoutExtensionInCOL.push_back(pCOLEntry->getModelName());
			}
		}
		pCOLFile->unload();
		delete pCOLFile;
	}

	vecCOLNamesWithoutExtensionInCOL = CVectorUtility::toUpperCase(vecCOLNamesWithoutExtensionInCOL);
	unordered_map<string, bool> umapCOLNamesWithoutExtensionInCOL = CVectorUtility::convertVectorToUnorderedMap(vecCOLNamesWithoutExtensionInCOL);
	vecCOLNamesWithoutExtensionInCOL.clear();

	// find COL names found in IDE but not found in COL
	vector<string> vecCOLNamesWithoutExtensionMissingFromIMG;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFNamesWithoutExtensionInIDE.size());
	for (string& strDFFName : vecDFFNamesWithoutExtensionInIDE)
	{
		if (umapCOLNamesWithoutExtensionInCOL.count(CStringUtility::toUpperCase(strDFFName)) == 0)
		{
			vecCOLNamesWithoutExtensionMissingFromIMG.push_back(strDFFName);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	string strIMGFileName = CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath());
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_133", vecCOLNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_135"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecCOLNamesWithoutExtensionMissingFromIMG, "\n"), true);

	// popup
	string strInitialFilename = CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_9_InitialFilename", CPathUtility::replaceFileExtension(CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()), "txt").c_str());
	string strTitle = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_133", vecCOLNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str());
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog("Missing Entries", strTitle, "TXD Name", vecCOLNamesWithoutExtensionMissingFromIMG, strInitialFilename, "MISSINGENTRIES");
	getKGM()->getTaskManager()->onTaskUnpause();
	getKGM()->getTaskManager()->onTaskEnd("onRequestFindCOLMissingFromCOLFoundInIDE");
}
void						CTaskDispatchManager::onRequestFindDFFMissingFromIMGFoundInIDE(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestFindDFFMissingFromIMGFoundInIDE");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIMGFoundInIDE", true);
		return;
	}

	// fetch DFF names in IDE files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IDE_IMG_DFF__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIMGFoundInIDE", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IDE_IMG_DFF__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	vector<string> vecDFFNamesWithoutExtensionInIDE = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);

	// fetch DFF names in active IMG file
	vector<string> vecDFFNamesWithoutExtensionInIMG;
	for (CIMGEntry *pIMGEntry : getKGM()->getEntryListTab()->getIMGFile()->getEntries())
	{
		if (pIMGEntry->isDFFFile())
		{
			vecDFFNamesWithoutExtensionInIMG.push_back(CPathUtility::removeFileExtension(pIMGEntry->getEntryName()));
		}
	}
	vecDFFNamesWithoutExtensionInIMG = CVectorUtility::toUpperCase(vecDFFNamesWithoutExtensionInIMG);
	unordered_map<string, bool> umapDFFNamesWithoutExtensionInIMG = CVectorUtility::convertVectorToUnorderedMap(vecDFFNamesWithoutExtensionInIMG);
	vecDFFNamesWithoutExtensionInIMG.clear();

	// find DFF names found in IDE but not found in IMG
	vector<string> vecDFFNamesWithoutExtensionMissingFromIMG;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFNamesWithoutExtensionInIDE.size());
	for (string& strDFFName : vecDFFNamesWithoutExtensionInIDE)
	{
		if (umapDFFNamesWithoutExtensionInIMG.count(CStringUtility::toUpperCase(strDFFName)) == 0)
		{
			vecDFFNamesWithoutExtensionMissingFromIMG.push_back(strDFFName);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	string strIMGFileName = CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath());
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_134", vecDFFNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str()));
	getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_135"), true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecDFFNamesWithoutExtensionMissingFromIMG, "\n"), true);

	// popup
	string strInitialFilename = CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_9_InitialFilename", CPathUtility::replaceFileExtension(CPathUtility::getFileName(getKGM()->getEntryListTab()->getIMGFile()->getFilePath()), "txt").c_str());
	string strTitle = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_134", vecDFFNamesWithoutExtensionMissingFromIMG.size(), strIMGFileName.c_str());
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog("Missing Entries", strTitle, "TXD Name", vecDFFNamesWithoutExtensionMissingFromIMG, strInitialFilename, "MISSINGENTRIES");
	getKGM()->getTaskManager()->onTaskUnpause();
	
	getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIMGFoundInIDE");
}

void						CTaskDispatchManager::onRequestCloneIMG(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestCloneIMG");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCloneIMG", true);
		return;
	}

	CIMGFormat *pIMGFile = getKGM()->getEntryListTab()->getIMGFile();
	
	string strIMGFileExtension = CPathUtility::getFileExtension(pIMGFile->getFilePath());
	string strClonedIMGPath = CPathUtility::getDirectory(pIMGFile->getFilePath()) + CPathUtility::removeFileExtension(CPathUtility::getFileName(pIMGFile->getFilePath())) + "-cloned." + strIMGFileExtension;

	CIMGFormat *pClonedIMGFile = pIMGFile->clone(strClonedIMGPath);
	if (!pClonedIMGFile)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestCloneIMG", true);
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(pIMGFile->getEntryCount() * 3);

	CEntryListWindowTab *pNewTab = getKGM()->getEntryListWindow()->addTab(strClonedIMGPath, pIMGFile->getIMGVersion());
	getKGM()->getEntryListWindow()->setActiveTab(pNewTab);

	getKGM()->getTaskManager()->onTaskEnd("onRequestCloneIMG");
}

void						CTaskDispatchManager::onRequestOpenIMGFolder(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestOpenIMGFolder");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestOpenIMGFolder", true);
		return;
	}

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(2);

	CIMGFormat *pIMGFile = getKGM()->getEntryListTab()->getIMGFile();
	string strFolderPath = CPathUtility::getDirectory(pIMGFile->getFilePath());
	getKGM()->getTaskManager()->onTaskProgressTick();

	ShellExecute(NULL, NULL, CStringUtility::convertStdStringToStdWString(strFolderPath).c_str(), NULL, NULL, SW_SHOWNORMAL);
	getKGM()->getTaskManager()->onTaskProgressTick();

	getKGM()->getTaskManager()->onTaskEnd("onRequestOpenIMGFolder");
}

void						CTaskDispatchManager::onRequestRemoveOrphanTexturesFromModel(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestRemoveOrphanTexturesFromModel");
	if (getKGM()->getEntryListTab() == nullptr)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveOrphanTexturesFromModel", true);
		return;
	}

	// input - txd
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecTXDPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("REMOVEORPHANTEXTURESFROMMODEL_TXD"), "TXD");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecTXDPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveOrphanTexturesFromModel", true);
		return;
	}
	getKGM()->setLastUsedDirectory("REMOVEORPHANTEXTURESFROMMODEL_TXD", CPathUtility::getDirectory(vecTXDPaths[0]));

	// progess bar
	uint32 uiSelectedDFFCount = 0;
	for (CIMGEntry *pIMGEntry : getKGM()->getEntryListTab()->getSelectedEntries())
	{
		if (pIMGEntry->isDFFFile())
		{
			uiSelectedDFFCount++;
		}
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount((uiSelectedDFFCount  * 2) + vecTXDPaths.size());

	// fetch texture names from DFF files (selected IMG entries)
	vector<string> vecDFFTextureNames;
	vector<CDFFFormat*> vecDFFFormatsInput;
	vector<CIMGEntry*> vecIMGEntries;
	uint32 uiProgressMaxTicksDecudctionForCorruptDFFFiles = 0;
	for (CIMGEntry *pIMGEntry : getKGM()->getEntryListTab()->getSelectedEntries())
	{
		if (pIMGEntry->isDFFFile())
		{
			CDFFFormat *pDFFFile = CDFFManager::getInstance()->parseViaMemory(pIMGEntry->getEntryData());
			if (pDFFFile->doesHaveError())
			{
				pDFFFile->unload();
				delete pDFFFile;
				getKGM()->getTaskManager()->onTaskProgressTick();
				uiProgressMaxTicksDecudctionForCorruptDFFFiles++;
				continue;
			}

			CVectorUtility::addToVector(vecDFFTextureNames, pDFFFile->getTextureNames());
			vecDFFFormatsInput.push_back(pDFFFile);
			vecIMGEntries.push_back(pIMGEntry);

			getKGM()->getTaskManager()->onTaskProgressTick();
		}
	}
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(getKGM()->getTaskManager()->getTaskMaxProgressTickCount() - uiProgressMaxTicksDecudctionForCorruptDFFFiles, false); // for below

	// fetch texture names from TXD files (files input)
	vector<string> vecTXDTextureNames;
	for (string strTXDPath : vecTXDPaths)
	{
		CTXDFormat *pTXDFile = CTXDManager::getInstance()->parseViaFile(strTXDPath);
		if (pTXDFile->doesHaveError())
		{
			pTXDFile->unload();
			delete pTXDFile;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		CVectorUtility::addToVector(vecTXDTextureNames, pTXDFile->getTextureNames());

		pTXDFile->unload();
		delete pTXDFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// find texture names found in DFF files getKGM()->getEntryListTab()but not found in TXD files
	vecTXDTextureNames = CVectorUtility::toUpperCase(vecTXDTextureNames);
	unordered_map<string, bool> umapTXDTextureNames = CVectorUtility::convertVectorToUnorderedMap(vecTXDTextureNames);
	vecTXDTextureNames.clear();

	vector<string> vecTexturesInDFFMissingFromTXD;
	unordered_map<string, bool> umapTexturesInDFFMissingFromTXD;
	for (string& strDFFTextureName : vecDFFTextureNames)
	{
		string strDFFTextureNameUpper = CStringUtility::toUpperCase(strDFFTextureName);
		if (umapTXDTextureNames.count(strDFFTextureNameUpper) == 0)
		{
			if (umapTexturesInDFFMissingFromTXD.count(strDFFTextureNameUpper) == 0)
			{
				umapTexturesInDFFMissingFromTXD[strDFFTextureNameUpper] = true;
				vecTexturesInDFFMissingFromTXD.push_back(strDFFTextureName);
			}
		}
	}
	umapTexturesInDFFMissingFromTXD.clear();

	// remove textures entries from DFF files that are not found in TXD files
	uint32 i = 0;
	uint32 uiDFFFileCountWithRemovedSections = 0;
	for (CDFFFormat *pDFFFile : vecDFFFormatsInput)
	{
		bool bRemovedSection = false;
		for (string& strDFFTextureName : vecTexturesInDFFMissingFromTXD)
		{
			CTextureEntry *pDFFTextureEntry = pDFFFile->getTextureByDiffuseOrAlphaName(strDFFTextureName);
			if (pDFFTextureEntry != nullptr)
			{
				CRWSection_Material *pMaterial = (CRWSection_Material*)pDFFTextureEntry->getRWTextureSection()->getParentNode();
				pMaterial->removeSection();
				pDFFFile->removeTextureEntry(pDFFTextureEntry);
				bRemovedSection = true;
			}
		}

		CIMGEntry *pIMGEntry = vecIMGEntries[i];
		pIMGEntry->setEntryData(pDFFFile->storeViaMemory());
		getKGM()->getEntryListTab()->updateEntryInMainListView(pIMGEntry);

		if (bRemovedSection)
		{
			uiDFFFileCountWithRemovedSections++;
		}

		getKGM()->getTaskManager()->onTaskProgressTick();

		i++;
	}

	// log
	getKGM()->getEntryListTab()->log("Removed orphan textures from " + CStringUtility::toString(uiDFFFileCountWithRemovedSections) + " of " + CStringUtility::toString(vecDFFFormatsInput.size()) + " DFF files.");
	getKGM()->getEntryListTab()->log("Textures in DFF files missing from TXD files:", true);
	getKGM()->getEntryListTab()->log(CStringUtility::join(vecTexturesInDFFMissingFromTXD, "\n"), true);

	// clean up
	for (auto pDFFFile : vecDFFFormatsInput)
	{
		pDFFFile->unload();
		delete pDFFFile;
	}
	getKGM()->getTaskManager()->onTaskEnd("onRequestRemoveOrphanTexturesFromModel");
}
void						CTaskDispatchManager::onRequestNewWindow(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestNewWindow");

	TCHAR wszRunningProgramPath[MAX_PATH];
	wmemset(wszRunningProgramPath, 0, MAX_PATH);
	GetModuleFileName(NULL, wszRunningProgramPath, MAX_PATH);

	ShellExecute(NULL, L"open", wszRunningProgramPath, NULL, NULL, SW_SHOWNORMAL);

	getKGM()->getTaskManager()->onTaskEnd("onRequestNewWindow");
}

void						CTaskDispatchManager::onRequestFindDFFMissingFromIDEFoundInIPL(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestFindDFFMissingFromIDEFoundInIPL");

	// fetch DFF names in IDE files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIDEPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IPL_IDE_DFF__IDE"), "IDE");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIDEFoundInIPL", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IPL_IDE_DFF__IDE", CPathUtility::getDirectory(vecIDEPaths[0]));

	vector<string> vecDFFNamesWithoutExtensionInIDE = CIDEManager::getIDEEntryNamesWithoutExtension(vecIDEPaths, true, false);

	// fetch DFF names in IPL files
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecIPLPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("MISSINGENTRIES_IPL_IDE_DFF__IPL"), "IPL");
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecIDEPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIDEFoundInIPL", true);
		return;
	}
	getKGM()->setLastUsedDirectory("MISSINGENTRIES_IPL_IDE_DFF__IPL", CPathUtility::getDirectory(vecIPLPaths[0]));

	vector<string> vecDFFNamesWithoutExtensionInIPL;
	for (string& strIPLPath : vecIPLPaths)
	{
		CIPLFormat *pIPLFile = CIPLManager::getInstance()->parseViaFile(strIPLPath);

		if (!pIPLFile->doesHaveError())
		{
			CVectorUtility::addToVector(vecDFFNamesWithoutExtensionInIPL, pIPLFile->getModelNames());
		}

		pIPLFile->unload();
		delete pIPLFile;
	}
	
	vecDFFNamesWithoutExtensionInIDE = CVectorUtility::toUpperCase(vecDFFNamesWithoutExtensionInIDE);
	unordered_map<string, bool> umapDFFNamesWithoutExtensionInIDE = CVectorUtility::convertVectorToUnorderedMap(vecDFFNamesWithoutExtensionInIDE);
	vecDFFNamesWithoutExtensionInIDE.clear();

	// find DFF names found in IPL but not found in IDE
	vector<string> vecDFFNamesWithoutExtensionMissingFromIDE;
	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFNamesWithoutExtensionInIDE.size());
	for (string& strDFFName : vecDFFNamesWithoutExtensionInIPL)
	{
		if (umapDFFNamesWithoutExtensionInIDE.count(CStringUtility::toUpperCase(strDFFName)) == 0)
		{
			vecDFFNamesWithoutExtensionMissingFromIDE.push_back(strDFFName);
		}
		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	if (getKGM()->getEntryListTab())
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_136", vecDFFNamesWithoutExtensionMissingFromIDE.size()));
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedText("Log_135"), true);
		getKGM()->getEntryListTab()->log(CStringUtility::join(vecDFFNamesWithoutExtensionMissingFromIDE, "\n"), true);
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_136", vecDFFNamesWithoutExtensionMissingFromIDE.size()));
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedText("Log_135"), true);
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CStringUtility::join(vecDFFNamesWithoutExtensionMissingFromIDE, "\n"), true);
	}

	// popup
	string strInitialFilename = CLocalizationManager::getInstance()->getTranslatedFormattedText("SaveFilePopup_9_InitialFilename", "DFF not in IDE found in IPL");
	string strTitle = CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_136", vecDFFNamesWithoutExtensionMissingFromIDE.size());
	getKGM()->getTaskManager()->onTaskPause();
	getKGM()->getGUIManager()->showListViewDialog("Missing Entries", strTitle, "TXD Name", vecDFFNamesWithoutExtensionMissingFromIDE, strInitialFilename, "MISSINGENTRIES");
	getKGM()->getTaskManager()->onTaskUnpause();

	getKGM()->getTaskManager()->onTaskEnd("onRequestFindDFFMissingFromIDEFoundInIPL");
}

void				CTaskDispatchManager::onRequestSortIDEAndIPLFilesByObjectId(void)
{
	getKGM()->getTaskManager()->onTaskBegin("onRequestSortIDEAndIPLFilesByObjectId");

	// choose DAT file
	getKGM()->getTaskManager()->onTaskPause();
	vector<string> vecDATPaths = CGUIUtility::openFileDialog(getKGM()->getLastUsedDirectory("SORTIDEANDIPL_OBJECTID_DAT"), "DAT", false);
	getKGM()->getTaskManager()->onTaskUnpause();
	if (vecDATPaths.size() == 0)
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortIDEAndIPLFilesByObjectId", true);
		return;
	}
	getKGM()->setLastUsedDirectory("SORTIDEANDIPL_OBJECTID_DAT", CPathUtility::getDirectory(vecDATPaths[0]));
	string strDATFilePath = vecDATPaths[0];

	// choose game folder
	getKGM()->getTaskManager()->onTaskPause();
	string strGameDirectoryPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_3"), getKGM()->getLastUsedDirectory("SORTIDEANDIPL_OBJECTID_GAME"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strGameDirectoryPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortIDEAndIPLFilesByObjectId", true);
		return;
	}
	strGameDirectoryPath = CPathUtility::addSlashToEnd(strGameDirectoryPath);
	getKGM()->setLastUsedDirectory("SORTIDEANDIPL_OBJECTID_GAME", strGameDirectoryPath);

	// choose output folder
	getKGM()->getTaskManager()->onTaskPause();
	string strOutputFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_23"), getKGM()->getLastUsedDirectory("SORTIDEANDIPL_OBJECTID_OUTPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strOutputFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortIDEAndIPLFilesByObjectId", true);
		return;
	}
	strOutputFolderPath = CPathUtility::addSlashToEnd(strOutputFolderPath);
	getKGM()->setLastUsedDirectory("SORTIDEANDIPL_OBJECTID_OUTPUT", strOutputFolderPath);

	// parse DAT file
	CDATLoaderFormat *pDATFile_GTA = CDATLoaderManager::getInstance()->parseViaFile(strDATFilePath);
	if (pDATFile_GTA->doesHaveError())
	{
		pDATFile_GTA->unload();
		delete pDATFile_GTA;
		getKGM()->getTaskManager()->onTaskEnd("onRequestSortIDEAndIPLFilesByObjectId", true);
		return;
	}

	// parse IDE and IPL files listed in DAT file
	vector<CIDEFormat*> veCIDEFormats = pDATFile_GTA->parseIDEFiles(strGameDirectoryPath);
	vector<CIPLFormat*> veCIPLFormats = pDATFile_GTA->parseIPLFiles(strGameDirectoryPath);

	pDATFile_GTA->unload();
	delete pDATFile_GTA;

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(veCIDEFormats.size() + veCIPLFormats.size());

	// sort and store IDE files
	for (CIDEFormat *pIDEFile : veCIDEFormats)
	{
		pIDEFile->sortAllSectionsByObjectId();
		pIDEFile->storeViaFile(strOutputFolderPath + CPathUtility::getFileName(pIDEFile->getFilePath()));

		pIDEFile->unload();
		delete pIDEFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// sort and store IPL files
	for (CIPLFormat *pIPLFile : veCIPLFormats)
	{
		pIPLFile->sortAllSectionsByObjectId();
		pIPLFile->storeViaFile(strOutputFolderPath + CPathUtility::getFileName(pIPLFile->getFilePath()));

		pIPLFile->unload();
		delete pIPLFile;

		getKGM()->getTaskManager()->onTaskProgressTick();
	}

	// log
	if (getKGM()->getEntryListTab())
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_137", veCIDEFormats.size(), veCIPLFormats.size(), CPathUtility::getFileName(strDATFilePath).c_str()));
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_137", veCIDEFormats.size(), veCIPLFormats.size(), CPathUtility::getFileName(strDATFilePath).c_str()));
	}

	getKGM()->getTaskManager()->onTaskEnd("onRequestSortIDEAndIPLFilesByObjectId");
}

void				CTaskDispatchManager::onRequestExtractDVCAndNVCColoursIntoDFFs(void)
{
	// begin
	getKGM()->getTaskManager()->onTaskBegin("onRequestExtractDVCAndNVCColoursIntoDFFs");

	// choose DVC, NVC, or both
	uint32 uiUpdateType = getKGM()->getGUIManager()->show3ButtonDialog("DVC / NVC Action", "Extract DVC, NVC, or both?", "DVC", "NVC", "Both");
	if (uiUpdateType == 0)
	{
		// user pressed cancel
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtractDVCAndNVCColoursIntoDFFs", true);
		return;
	}

	// choose DFF input folder for colours
	getKGM()->getTaskManager()->onTaskPause();
	string strDFFInputFolderForColours = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_24"), getKGM()->getLastUsedDirectory("EXTRACTNVCDVC_DFFCOLOURINPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strDFFInputFolderForColours == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtractDVCAndNVCColoursIntoDFFs", true);
		return;
	}
	strDFFInputFolderForColours = CPathUtility::addSlashToEnd(strDFFInputFolderForColours);
	getKGM()->setLastUsedDirectory("EXTRACTNVCDVC_DFFCOLOURINPUT", strDFFInputFolderForColours);

	// choose DFF input folder for models
	getKGM()->getTaskManager()->onTaskPause();
	string strDFFInputFolderForModels = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_25"), getKGM()->getLastUsedDirectory("EXTRACTNVCDVC_DFFMODELINPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strDFFInputFolderForModels == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtractDVCAndNVCColoursIntoDFFs", true);
		return;
	}
	strDFFInputFolderForModels = CPathUtility::addSlashToEnd(strDFFInputFolderForModels);
	getKGM()->setLastUsedDirectory("EXTRACTNVCDVC_DFFMODELINPUT", strDFFInputFolderForModels);

	// choose output folder
	getKGM()->getTaskManager()->onTaskPause();
	string strOutputFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_23"), getKGM()->getLastUsedDirectory("EXTRACTNVCDVC_OUTPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strOutputFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtractDVCAndNVCColoursIntoDFFs", true);
		return;
	}
	strOutputFolderPath = CPathUtility::addSlashToEnd(strOutputFolderPath);
	getKGM()->setLastUsedDirectory("EXTRACTNVCDVC_OUTPUT", strOutputFolderPath);

	// fetch DFF file paths
	vector<string> vecDFFInputFilenamesForColours = CFileUtility::getFileNamesByExtension(strDFFInputFolderForColours, "DFF");
	vector<string> vecDFFInputFilenamesForModels = CFileUtility::getFileNamesByExtension(strDFFInputFolderForModels, "DFF");
	
	//vecDFFInputFilenamesForColours = CVectorUtility::sortAZCaseInsensitive(vecDFFInputFilenamesForColours);
	//vecDFFInputFilenamesForModels = CVectorUtility::sortAZCaseInsensitive(vecDFFInputFilenamesForModels);

	unordered_map<string, bool> umapDFFInputFilenamesUpperForModels = CVectorUtility::convertVectorToUnorderedMap(CVectorUtility::toUpperCase(vecDFFInputFilenamesForModels));

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFInputFilenamesForColours.size());

	// iterate around input DFFs
	uint32 uiDFFUpdatedFileCount = 0;
	for (string& strDFFFilename_Colours : vecDFFInputFilenamesForColours)
	{
		if (umapDFFInputFilenamesUpperForModels.count(CStringUtility::toUpperCase(strDFFFilename_Colours)) == 0)
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		string& strDFFFilename_Model = strDFFFilename_Colours;

		CDFFFormat *pDFFFile_Colours = CDFFManager::getInstance()->parseViaFile(strDFFInputFolderForColours + strDFFFilename_Colours);
		if (pDFFFile_Colours->doesHaveError())
		{
			pDFFFile_Colours->unload();
			delete pDFFFile_Colours;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		vector<CVector4ui8> vecDFFColours_DVC;
		vector<CVector4ui8> vecDFFColours_NVC;
		if (uiUpdateType == 1 || uiUpdateType == 3) // DVC or both
		{
			vecDFFColours_DVC = pDFFFile_Colours->getDVCColours();
		}
		if (uiUpdateType == 2 || uiUpdateType == 3) // NVC or both
		{
			vecDFFColours_NVC = pDFFFile_Colours->getNVCColours();
			if (vecDFFColours_NVC.size() == 0)
			{
				vecDFFColours_NVC = vecDFFColours_DVC;
			}
		}

		pDFFFile_Colours->unload();
		delete pDFFFile_Colours;

		//CDFFFormat *pDFFFile_Model = CDFFManager::getInstance()->parseViaFile(strDFFInputFolderForModels + strDFFFilename_Model);
		CDFFFormat *pDFFFile_Model = CDFFManager::getInstance()->parseViaMemory(CFileUtility::getFileContent(strDFFInputFolderForModels + strDFFFilename_Model, true));
		if (pDFFFile_Model->doesHaveError())
		{
			pDFFFile_Model->unload();
			delete pDFFFile_Model;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		if (uiUpdateType == 1 || uiUpdateType == 3) // DVC or both
		{
			if (vecDFFColours_DVC.size() > 0)
			{
				pDFFFile_Model->setDVCColours(vecDFFColours_DVC);
			}
		}
		if (uiUpdateType == 2 || uiUpdateType == 3) // NVC or both
		{
			if (vecDFFColours_NVC.size() > 0)
			{
				pDFFFile_Model->setNVCColours(vecDFFColours_NVC);
			}
		}

		pDFFFile_Model->storeViaFile(strOutputFolderPath + strDFFFilename_Colours);

		pDFFFile_Model->unload();
		delete pDFFFile_Model;
		getKGM()->getTaskManager()->onTaskProgressTick();
		uiDFFUpdatedFileCount++;
	}

	// log
	if (getKGM()->getEntryListTab())
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_138", uiDFFUpdatedFileCount));
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_138", uiDFFUpdatedFileCount));
	}

	// end
	getKGM()->getTaskManager()->onTaskEnd("onRequestExtractDVCAndNVCColoursIntoDFFs");
}

void				CTaskDispatchManager::onRequestExtract2DFXIntoDFFs(void)
{
	// begin
	getKGM()->getTaskManager()->onTaskBegin("onRequestExtract2DFXIntoDFFs");

	// choose DFF input folder for colours
	getKGM()->getTaskManager()->onTaskPause();
	string strDFFInputFolderFor2DFX = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_26"), getKGM()->getLastUsedDirectory("EXTRACTDFF2DFX_INPUT2DFX"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strDFFInputFolderFor2DFX == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtract2DFXIntoDFFs", true);
		return;
	}
	strDFFInputFolderFor2DFX = CPathUtility::addSlashToEnd(strDFFInputFolderFor2DFX);
	getKGM()->setLastUsedDirectory("EXTRACTDFF2DFX_INPUT2DFX", strDFFInputFolderFor2DFX);

	// choose DFF input folder for models
	getKGM()->getTaskManager()->onTaskPause();
	string strDFFInputFolderForModels = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_25"), getKGM()->getLastUsedDirectory("EXTRACTDFF2DFX_INPUTMODEL"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strDFFInputFolderForModels == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtract2DFXIntoDFFs", true);
		return;
	}
	strDFFInputFolderForModels = CPathUtility::addSlashToEnd(strDFFInputFolderForModels);
	getKGM()->setLastUsedDirectory("EXTRACTDFF2DFX_INPUTMODEL", strDFFInputFolderForModels);

	// choose output folder
	getKGM()->getTaskManager()->onTaskPause();
	string strOutputFolderPath = CGUIUtility::chooseFolderDialog(CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_23"), getKGM()->getLastUsedDirectory("EXTRACTDFF2DFX_OUTPUT"));
	getKGM()->getTaskManager()->onTaskUnpause();
	if (strOutputFolderPath == "")
	{
		getKGM()->getTaskManager()->onTaskEnd("onRequestExtract2DFXIntoDFFs", true);
		return;
	}
	strOutputFolderPath = CPathUtility::addSlashToEnd(strOutputFolderPath);
	getKGM()->setLastUsedDirectory("EXTRACTDFF2DFX_OUTPUT", strOutputFolderPath);

	// fetch DFF file paths
	vector<string> vecDFFInputFilenamesFor2DFX = CFileUtility::getFileNamesByExtension(strDFFInputFolderFor2DFX, "DFF");
	vector<string> vecDFFInputFilenamesForModels = CFileUtility::getFileNamesByExtension(strDFFInputFolderForModels, "DFF");

	//vecDFFInputFilenamesForColours = CVectorUtility::sortAZCaseInsensitive(vecDFFInputFilenamesForColours);
	//vecDFFInputFilenamesForModels = CVectorUtility::sortAZCaseInsensitive(vecDFFInputFilenamesForModels);

	unordered_map<string, bool> umapDFFInputFilenamesUpperForModels = CVectorUtility::convertVectorToUnorderedMap(CVectorUtility::toUpperCase(vecDFFInputFilenamesForModels));

	getKGM()->getTaskManager()->setTaskMaxProgressTickCount(vecDFFInputFilenamesFor2DFX.size());

	// iterate around input DFFs
	uint32 uiDFFUpdatedFileCount = 0;
	for (string& strDFFFilename_2DFX : vecDFFInputFilenamesFor2DFX)
	{
		if (umapDFFInputFilenamesUpperForModels.count(CStringUtility::toUpperCase(strDFFFilename_2DFX)) == 0)
		{
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}
		string& strDFFFilename_Model = strDFFFilename_2DFX;

		CDFFFormat *pDFFFile_2DFX = CDFFManager::getInstance()->parseViaFile(strDFFInputFolderFor2DFX + strDFFFilename_2DFX);
		if (pDFFFile_2DFX->doesHaveError())
		{
			pDFFFile_2DFX->unload();
			delete pDFFFile_2DFX;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		vector<vector<C2dEffect*>> vec2dEffects = pDFFFile_2DFX->get2dEffects();

		CDFFFormat *pDFFFile_Model = CDFFManager::getInstance()->parseViaFile(strDFFInputFolderForModels + strDFFFilename_Model);
		if (pDFFFile_Model->doesHaveError())
		{
			pDFFFile_2DFX->unload();
			delete pDFFFile_2DFX;
			pDFFFile_Model->unload();
			delete pDFFFile_Model;
			getKGM()->getTaskManager()->onTaskProgressTick();
			continue;
		}

		pDFFFile_Model->set2dEffects(vec2dEffects);

		pDFFFile_Model->storeViaFile(strOutputFolderPath + strDFFFilename_2DFX);

		pDFFFile_2DFX->unload();
		delete pDFFFile_2DFX;
		pDFFFile_Model->unload();
		delete pDFFFile_Model;
		getKGM()->getTaskManager()->onTaskProgressTick();
		uiDFFUpdatedFileCount++;
	}

	// log
	if (getKGM()->getEntryListTab())
	{
		getKGM()->getEntryListTab()->log(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_139", uiDFFUpdatedFileCount));
	}
	else
	{
		getKGM()->getEntryListWindow()->logWithNoTabsOpen(CLocalizationManager::getInstance()->getTranslatedFormattedText("Log_139", uiDFFUpdatedFileCount));
	}

	// end
	getKGM()->getTaskManager()->onTaskEnd("onRequestExtract2DFXIntoDFFs");
}