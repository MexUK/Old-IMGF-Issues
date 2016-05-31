#pragma warning(disable : 4005)

#include "CKGM.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Engine/RAGE/CRageManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Session/CSessionManager.h"
#include "CGUIManager.h"
#include "CLSTProcessingManager.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "CSettingsManager.h"
#include "Sort/CSortManager.h"
#include "IMG/CIMGManager.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "Window/CTabbedWindow.h"
#include "Window/Window Controls/Controls/CWindowControl_Text.h"
#include "Window/Window Controls/Controls/CWindowControl_List.h"
#include "Window/Window Controls/Controls/CWindowControl_Button.h"
#include "Window/Window Controls/Controls/CWindowControl_Check.h"
#include "Window/Window Controls/Controls/CWindowControl_Drop.h"
#include "Window/Window Controls/Controls/CWindowControl_Scroll.h"
#include "Window/Window Controls/Controls/CWindowControl_Progress.h"
#include "Window/Window Controls/Controls/CWindowControl_Radio.h"
#include "Window/Window Controls/Controls/CWindowControl_Edit.h"
#include "Window/Window Types/CEntryListWindow.h"
#include "Window/CWindowManager.h"
#include "String/CStringUtility.h"
#include "File/CFileUtility.h"
#include "Path/CPathUtility.h"
#include "Registry/CRegistryUtility.h"
#include "Drag Drop/CDropTarget.h"
#include "DB/CDBManager.h"
#include "DB/CDBFormat.h"
#include "Testing/CDebugger.h"
#include "IMG/CIMGEntry.h"
#include "COL/CCOLManager.h"
#include "COL/CCOLVersionManager.h"
#include "COL/CCOLVersion.h"
#include "GUI/CGUIUtility.h"
#include "CSearchEntry.h"
#include "Entry Viewer/CEntryViewerManager.h"
#include "Vector/CVectorUtility.h"
#include "INI/CINIUtility.h"
#include "Update/CUpdateManager.h"
#include "buildnumber.h"
#include "eCompressionAlgorithm.h"
#include "DAT/Loader/CDATLoaderManager.h"
#include "DAT/Path/CDATPathManager.h"
#include "Localization/CLocalizationManager.h"
#include "Language/CLanguageManager.h"
#include "BMP/CBMPManager.h"
#include "CUR/CCURManager.h"
#include "DFF/CDFFManager.h"
#include "DB/CDBManager.h"
#include "ICO/CICOManager.h"
#include "IDE/CIDEManager.h"
#include "Image/CImageManager.h"
#include "IPL/CIPLManager.h"
#include "LST/CLSTManager.h"
#include "TXD/CTXDManager.h"
#include "WDR/CWDRManager.h"
#include "WTD/CWTDManager.h"
#include "Localization/eLanguage.h"
#include "Language/CLanguage.h"
#include "Game/CGameManager.h"
#include "Platform/CPlatformManager.h"
#include "RW/CRWSection.h"
#include "CTiming.h"
#include "CLastUsedValueManager.h"

using namespace std;

// construct/destruct
CKGM::CKGM(void)
{
	// construct objects stored by CKGM
	m_pEntryViewerManager	= new CEntryViewerManager;
	m_pGUIManager			= new CGUIManager;
	m_pLanguageManager		= new CLanguageManager;
	m_pLSTProcessingManager	= new CLSTProcessingManager;
	m_pRecentlyOpenManager	= new CRecentlyOpenManager;
	m_pTaskManager			= new CTaskManager;
	m_pSessionManager		= new CSessionManager;
	m_pSettingsManager		= new CSettingsManager;
	m_pSortManager			= new CSortManager;
	m_pUpdateManager		= new CUpdateManager;
	m_pWindowManager		= new CWindowManager;
	m_pLastUsedValueManager	= new CLastUsedValueManager;
}
CKGM::~CKGM(void)
{
	// destruct objects stored by CKGM
	delete m_pEntryViewerManager;
	delete m_pGUIManager;
	delete m_pLanguageManager;
	delete m_pLSTProcessingManager;
	delete m_pRecentlyOpenManager;
	delete m_pTaskManager;
	delete m_pSessionManager;
	delete m_pSettingsManager;
	delete m_pSortManager;
	delete m_pUpdateManager;
	delete m_pWindowManager;
	delete m_pLastUsedValueManager;
}

// init/uninit (ocurs in original thread)
void		CKGM::init(void)
{
	m_pWindowManager->init();
	initInitializationThread();
}

void		CKGM::uninit(void)
{
}

// init tasks (ocurs in original thread)
void		CKGM::initInitializationThread(void)
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	CreateThread(&sa, 0, onInitializationThreadStart, this, 0, NULL);
}

// init (occurs in different thread)
uint32 WINAPI		onInitializationThreadStart(void *pThreadParameter)
{
	((CKGM*)pThreadParameter)->_init();
	return 0;
}

void		CKGM::_init(void)
{
	initBuildMeta();
	initInstallationMeta();
	initStoredObjects();
	initSingletonObjects();
	initStaticData();
	initEventBinding();
	initSettings();
	initLocalization();
	initSorting();
	initOldVersionMigration();
	initTempStuff();
	initCommandLine();
	initAutoUpdateCheck();
}

// init tasks (occurs in different thread)
void		CKGM::initBuildMeta(void)
{
	getBuildMeta().setCurrentVersion(1.3f);
	getBuildMeta().setIsAlphaBuild(true);
	getBuildMeta().setCurrentVersionString("1.3" + string(getBuildMeta().isAlphaBuild() ? " Alpha" : ""));
}

void		CKGM::initInstallationMeta(void)
{
	// choose installation folder
	string strInstallationPath = CRegistryUtility::getSoftwareValueString("KGM\\InternalSettings", "InstallationPath");
	if (strInstallationPath == "")
	{
		string strPotentialInstallationPath = "C:\\Program Files (x86)\\KGM\\" + getKGM()->getBuildMeta().getCurrentVersionString() + "\\";
		if (CFileUtility::doesFolderExist(strPotentialInstallationPath))
		{
			strInstallationPath = strPotentialInstallationPath;
			CRegistryUtility::setSoftwareValueString("KGM\\InternalSettings", "InstallationPath", strPotentialInstallationPath);
		}
		else
		{
			//string strChosenInstallationFolder = CGUIUtility::chooseFolderDialog(getDialog()->GetSafeHwnd(), CLocalizationManager::getInstance()->getTranslatedText("ChooseFolderPopup_11"), getKGM()->getLastUsedDirectory("INSTALLATION"));
			string strChosenInstallationFolder = CGUIUtility::chooseFolderDialog(getActiveWindow()->getWindowHandle(), "Choose the installation folder for KGM. (e.g. In program files x86)", getKGM()->getLastUsedDirectory("INSTALLATION"));
			if (strChosenInstallationFolder == "")
			{
			}
			else
			{
				strInstallationPath = strChosenInstallationFolder;
				getKGM()->setLastUsedDirectory("INSTALLATION", strChosenInstallationFolder);
				CRegistryUtility::setSoftwareValueString("KGM\\InternalSettings", "InstallationPath", strChosenInstallationFolder);
			}
		}
	}
}

void		CKGM::initStoredObjects(void)
{
	// initialize objects stored by CKGM
	// Excludes: CWindowManager and CSortManager
	m_pEntryViewerManager->init();
	m_pGUIManager->init();
	m_pLanguageManager->init();
	m_pLSTProcessingManager->init();
	m_pRecentlyOpenManager->init();
	m_pTaskManager->init();
	m_pSessionManager->init();
	m_pSettingsManager->init();
	m_pUpdateManager->init();
}

void		CKGM::initSingletonObjects(void)
{
	// initialize singleton objects
	CBMPManager::getInstance()->init();
	CCOLManager::getInstance()->init();
	CCURManager::getInstance()->init();
	CDATLoaderManager::getInstance()->init();
	CDATPathManager::getInstance()->init();
	CDBManager::getInstance()->init();
	CDFFManager::getInstance()->init();
	CGameManager::getInstance()->init();
	CICOManager::getInstance()->init();
	CIDEManager::getInstance()->init();
	CImageManager::getInstance()->init();
	CIMGManager::getInstance()->init();
	CIPLManager::getInstance()->init();
	CLSTManager::getInstance()->init();
	CPlatformManager::getInstance()->init();
	CRageManager::getInstance()->init();
	CRWManager::getInstance()->init();
	CTXDManager::getInstance()->init();
	CWDRManager::getInstance()->init();
	CWTDManager::getInstance()->init();
}

void		CKGM::initStaticData(void)
{
	CRWSection::initStatic();
}

void		CKGM::initEventBinding(void)
{
	auto fOnEntriesExtensionChange = [](void *pData)
	{
		if (getKGM()->getEntryListTab() != nullptr) // When opening an IMG parseIMG() will call addEntry() which will trigger this callback, but the active view instance won't be set yet.
		{
			getKGM()->getEntryListTab()->loadFilter_Type();
		}
	};

	CEventManager::getInstance()->bindEvent(EVENT_onProgressTick,			[](void *pData) { getKGM()->getTaskManager()->onTaskProgressTick(); });
	CEventManager::getInstance()->bindEvent(EVENT_onParseEntry,				[](void *pData) { getKGM()->getTaskManager()->onTaskProgressTick(); });
	CEventManager::getInstance()->bindEvent(EVENT_onAddEntryExtension,		fOnEntriesExtensionChange);
	CEventManager::getInstance()->bindEvent(EVENT_onRemoveEntryExtension,	fOnEntriesExtensionChange);
	CEventManager::getInstance()->bindEvent(EVENT_onRebuildSerializeEntry,	[](void *pData) { getKGM()->getTaskManager()->onTaskProgressTick(); });
	CEventManager::getInstance()->bindEvent(EVENT_onLoadRWVersion,			[](void *pData) { getKGM()->getTaskManager()->onTaskProgressTick(); });
	CEventManager::getInstance()->bindEvent(EVENT_onLoadResourceType,		[](void *pData) { getKGM()->getTaskManager()->onTaskProgressTick(); });
}

void		CKGM::initSettings(void)
{
	getSettingsManager()->loadSettings();
}

void		CKGM::initLocalization(void)
{
	eLanguage eActiveLanguage = (eLanguage)getKGM()->getSettingsManager()->getSettingInt("Language");
	CLocalizationManager::getInstance()->setActiveLanguage(eActiveLanguage);
	CLocalizationManager::getInstance()->setActiveLanguageName(getKGM()->getLanguageManager()->getLanguageById(eActiveLanguage)->getLanguageName());
	CLocalizationManager::getInstance()->setInstallationPath(CRegistryUtility::getSoftwareValueString("KGM\\InternalSettings", "InstallationPath"));
	CLocalizationManager::getInstance()->loadTranslatedText();
}

void		CKGM::initSorting(void)
{
	m_pSortManager->init();
}

void		CKGM::initOldVersionMigration(void)
{
	// delete previous version's exe file
	string strPreviousVersionExePath = CRegistryUtility::getSoftwareValueString("KGM\\InternalSettings", "DeletePreviousVersionOnNextLaunch");
	if (strPreviousVersionExePath != "")
	{
		int iResult;
		bool bFileExists;
		uint32 uiIterationCount = 0;
		do
		{
			bFileExists = CFileUtility::doesFileExist(strPreviousVersionExePath);
			if (bFileExists)
			{
				iResult = DeleteFile(CStringUtility::convertStdStringToStdWString(strPreviousVersionExePath).c_str());
				if (iResult == 0)
				{
					Sleep(250);
				}
			}
			uiIterationCount++;
			if (uiIterationCount == 10)
			{
				break; // 2.5 seconds max
			}
		}
		while (bFileExists && iResult == 0);
		CRegistryUtility::removeSoftwareValue("KGM\\InternalSettings", "DeletePreviousVersionOnNextLaunch");
	}
}

void		CKGM::initCommandLine(void)
{
	// command line
	wchar_t *pCommandLine = GetCommandLine();
	string strCommandLine = CStringUtility::convertStdWStringToStdString(pCommandLine);
	int uiPos1 = strCommandLine.find('"', 0);
	int uiPos2 = strCommandLine.find('"', uiPos1 + 1);
	int uiPos3 = strCommandLine.find('"', uiPos2 + 1);
	if (uiPos3 != string::npos)
	{
		int uiPos4 = strCommandLine.find('"', uiPos3 + 1);
		string strPath = strCommandLine.substr(uiPos3 + 1, (uiPos4 - uiPos3) - 1);
		//MessageBox(NULL, CStringUtility::convertStdStringToStdWString(strCommandLine).c_str(), L"S", MB_OK);
		getTaskManager()->getDispatch()->onRequestOpen2(strPath);
	}
}

void		CKGM::initAutoUpdateCheck(void)
{
	/*
	todo
	This currently calls a onRequestBlah which eventualls calls CTaskManager::onFeatureEnd which can crash before the CEntryListWindow object has been created.
	So move this to like lambda: onWindowOpen()
	getKGM()->getTaskManager()->getDispatch()->onRequestAutoUpdate();
	*/
}

void		CKGM::initTempStuff(void)
{
	return;

	CMainWindow *pWindow = getWindowManager()->addMainWindow();
	getWindowManager()->setMainWindow(pWindow);

	//CWindow *pWindow = nullptr;

	CWindowControl_List *pList = new CWindowControl_List;
	pWindow->getControls().addEntry(pList);
	pList->setWindow(pWindow);
	pList->setPosition(CVector2ui32(0,0));
	pList->setSize(CVector2ui32(640,850));
	pList->setRowHeight(50);
	pList->setFontSize(13);
	pList->setRowBackgroundColour1(0xFFFFFFFF);
	pList->setRowBackgroundColour2(0xDDDDDDFF);
	pList->setColumnWidth(100);
	pList->setHasVerticalScrollBar(true);

	CWindowControlEntry_List *pListEntry = new CWindowControlEntry_List[10];
	pList->getEntries().resize(10);
	for (int i = 0; i < 10; i++)
	{
		pListEntry->setList(pList);
		pList->getEntries()[i] = pListEntry;

		for (uint32 i2 = 0; i2 < 2; i2++)
		{
			vector<string> vecText;
			for (uint32 i3 = 0; i3 < 2; i3++)
			{
				vecText.push_back("sehufihrsiufhd");
			}
			pListEntry[i].getText().push_back(vecText);
		}
	}

	CWindowControl_Text *pText = new CWindowControl_Text;
	pText->setWindow(pWindow);
	pWindow->getControls().addEntry(pText);
	string strText = "sdiogfhrsui uihesuifhsiufd esfsuinfesiunqawuidnsdziunfsdjxckfn jsdinfiusejdkz";
	pText->setText(strText);
	pText->setTextColour(0xFF8800FF);
	pText->setPosition(CVector2ui32(100, 200));
	pText->setSize(CVector2ui32(800, 20));
	pText->setFontSize(13);
	pText->setBold(true);

	/*
	CWindowControl_Button *pButton = new CWindowControl_Button;
	pButton->setWindow(pWindow);
	pWindow->getControls().addEntry(pButton);
	pButton->setText("test1");
	*/

	CWindowControl_Check *pCheck = new CWindowControl_Check;
	pCheck->setWindow(pWindow);
	pWindow->getControls().addEntry(pCheck);
	strText = "test1";
	pCheck->setText(strText);
	pCheck->setPosition(CVector2ui32(400, 400));
	pCheck->setSize(CVector2ui32(20, 20));
	pCheck->setFillColour(0xFFFFFF00);
	pCheck->setChecked(true);

	CWindowControl_Drop *pDrop = new CWindowControl_Drop;
	pDrop->setWindow(pWindow);
	pWindow->getControls().addEntry(pDrop);
	strText = "test2";
	pDrop->setText(strText);
	pDrop->setPosition(CVector2ui32(600, 200));
	pDrop->setSize(CVector2ui32(100, 30));
	pDrop->setFillColour(0xFFFFFF00);
	pDrop->setLineColour(0xAA0000FF);
	pDrop->setSelectionListOpen(false);

	for (int i8 = 0; i8 < 10; i8++)
	{
		CWindowControlEntry_Drop *pDropEntry = new CWindowControlEntry_Drop;
		pDrop->addEntry(pDropEntry);
		pDropEntry->setText("drop entry " + CStringUtility::toString(i8));
	}

	CWindowControl_Scroll *pScroll = new CWindowControl_Scroll;
	pScroll->setWindow(pWindow);
	pWindow->getControls().addEntry(pScroll);
	pScroll->setPosition(CVector2ui32(720, 200));
	pScroll->setSize(CVector2ui32(20, 500));
	pScroll->setFillColour(0xFF0000FF);
	pScroll->setLineColour(0x00AA00FF);
	pScroll->setScrollOrientation(WINDOW_CONTROL_ORIENTATION_VERTICAL);
	pScroll->setSeekBarLength(50);
	pScroll->setSeekBarLineColour(0x352800FF);
	pScroll->setSeekBarFillColour(0x008800FF);
	pScroll->setProgress(0.0f);

	CWindowControl_Button *pButton = new CWindowControl_Button;
	pButton->setWindow(pWindow);
	pWindow->getControls().addEntry(pButton);
	pButton->setPosition(CVector2ui32(780, 200));
	pButton->setSize(CVector2ui32(100, 30));
	strText = "zzzzzzzzzzzz";
	pButton->setText(strText);
	pButton->setFillColour(0x0000DFFF);
	pButton->setLineColour(0x000088FF);

	CWindowControl_Progress *pProgress = new CWindowControl_Progress;
	pProgress->setWindow(pWindow);
	pWindow->getControls().addEntry(pProgress);
	pProgress->setPosition(CVector2ui32(800, 500));
	pProgress->setSize(CVector2ui32(150, 20));
	pProgress->setFillColour(0xBA0000FF);
	pProgress->setLineColour(0x000000FF);
	pProgress->setProgressFillColour(0x0000baff);
	pProgress->setCurrentTicks(50);
	pProgress->setMaxTicks(80);

	for (int aa = 0; aa < 2; aa++)
	{
		for (int bb = 0; bb < 4; bb++)
		{
			CWindowControl_Radio *pRadio = new CWindowControl_Radio;
			pRadio->setWindow(pWindow);
			pWindow->getControls().addEntry(pRadio);
			pRadio->setGroupId(aa);
			pRadio->setPosition(CVector2ui32(1000 + 150*aa, 250 + 30*bb));
			pRadio->setSize(CVector2ui32(20, 20));
			pRadio->setFillColour(0xBA0000FF);
			pRadio->setLineColour(0x000000FF);
			strText = "radio text";
			pRadio->setText(strText);
		}
	}

	CWindowControl_Edit *pEdit = new CWindowControl_Edit;
	pEdit->setWindow(pWindow);
	pWindow->getControls().addEntry(pEdit);
	pEdit->setPosition(CVector2ui32(1150, 750));
	pEdit->setSize(CVector2ui32(200, 200));
	pEdit->setFillColour(0xBA0000FF);
	pEdit->setLineColour(0x000000FF);
	strText = "edit text";
	pEdit->setText(strText);

	vector<CWindowControl*> vecWindowControls = pWindow->getControls().getEntries();
	for (CWindowControl *pWindowControl : vecWindowControls)
	{
		if (pWindowControl->getControlType() != WINDOW_CONTROL_EDIT)
		{
			//pWindow->getControls().removeEntry(pWindowControl);
		}
	}

	pWindow->render();

	return;
	string strWTDFilePath = "C:\\Users\\James\\Desktop\\Original Files\\WTD\\loadingscreens.wtd";
	//string strIMGFilePath = "C:\\Users\\James\\Desktop\\Original Files\\IMG\\VC\\gta3.dir";
	//string strIMGFilePath = "C:\\Users\\James\\Desktop\\Original Files\\IMG\\SA\\gta3.img";
	//string strIMGFilePath = "C:\\Users\\James\\Desktop\\IMG.img";
	for (uint32 i = 0; i < 10; i++)
	{
		///*
		CTiming::getInstance()->start("parse wtd");
		CWTDFormat *pWTDFormat = CWTDManager::getInstance()->parseViaFile(strWTDFilePath);
		CTiming::getInstance()->stop();
		delete pWTDFormat;
		//*/

		/*
		CTiming::getInstance()->start("parse img");
		CIMGFormat *pIMGFormat = CIMGManager::getInstance()->parseViaFile(strIMGFilePath);
		CTiming::getInstance()->stop();
		delete pIMGFormat;
		*/
	}

	string strTimingsFilePath = "C:\\Users\\James\\Desktop\\Timings.txt";
	CFileUtility::removeFile(strTimingsFilePath);
	for (auto it : CTiming::getInstance()->getTimings())
	{
		CFileUtility::storeFile(strTimingsFilePath, it.first + " " + CTiming::getInstance()->joinTimings(it.second) + "\n", true, false);
	}
}

// windows/tabs
void		CKGM::openMainWindow(void)
{
	CMainWindow *pMainWindow = getWindowManager()->addMainWindow();
	getWindowManager()->setMainWindow(pMainWindow);
	pMainWindow->setBackgroundColour(0x214E67FF);
	pMainWindow->setSize(CVector2ui32(1025, 698));
}

void		CKGM::processWindows(void)
{
	getWindowManager()->getMainWindow()->processWindow();
	// todo - also process windows for vector of CWindow*s
}

CMainWindow*				CKGM::getMainWindow(void)
{
	return m_pWindowManager->getMainWindow();
}

CWindow*					CKGM::getActiveWindow(void)
{
	return m_pWindowManager->getActiveWindow();
}

CWindowTab*					CKGM::getActiveTab(void)
{
	return m_pWindowManager->getActiveWindow() == nullptr ? nullptr : m_pWindowManager->getActiveWindow()->getActiveTab();
	/*
	todo
	CWindow *pWindow = m_pWindowManager->getActiveWindow();
	switch (pWindow->getWindowType())
	{
	case WINDOW_TYPE_ENTRY_LIST:	return ((CTabbedWindow<CEntryListWindow*>*)pWindow)->getActiveTab();
	case WINDOW_TYPE_ENTRY_EDITOR:	return ((CTabbedWindow<CEntryEditorWindow*>*)pWindow)->getActiveTab();
	default:						return nullptr;
	}
	*/
	return nullptr;
}

CEntryListWindow*			CKGM::getEntryListWindow(void)
{
	// todo
	return getMainWindow() == nullptr ? nullptr : ((CEntryListWindow*)getWindowManager()->getEntryByIndex(0));
}

CEntryListWindowTab*		CKGM::getEntryListTab(void)
{
	// todo
	return getActiveTab() == nullptr ? nullptr : ((CEntryListWindowTab*)getWindowManager()->getEntryByIndex(0)->getEntryByIndex(0));
}

// last used directory
void			CKGM::setLastUsedDirectory(string strHandleName, string strDirectory)
{
	CRegistryUtility::setSoftwareValueString("KGM\\LastUsedDirectories", strHandleName, strDirectory);
}
string			CKGM::getLastUsedDirectory(string strHandleName)
{
	return CRegistryUtility::getSoftwareValueString("KGM\\LastUsedDirectories", strHandleName);
}