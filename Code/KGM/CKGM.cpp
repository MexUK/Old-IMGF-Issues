#pragma warning(disable : 4005)

#include "CKGM.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Engine/RAGE/CRageManager.h"
#include "Recently Open/CRecentlyOpenManager.h"
#include "Session/CSessionManager.h"
#include "CPopupGUIManager.h"
#include "CLSTProcessingManager.h"
#include "Task/CTaskManager.h"
#include "Task/CTaskDispatchManager.h"
#include "CSettingsManager.h"
#include "Sort/CSortManager.h"
#include "IMG/CIMGManager.h"
#include "Event/CEventManager.h"
#include "Event/eEvent.h"
#include "GUI/CGUIManager.h"
#include "GUI/Window/CWindow.h"
#include "GUI/Controls/CWindowControl_Text.h"
#include "GUI/Controls/CWindowControl_List.h"
#include "GUI/Controls/CWindowControl_Button.h"
#include "GUI/Controls/CWindowControl_Check.h"
#include "GUI/Controls/CWindowControl_Drop.h"
#include "GUI/Controls/CWindowControl_Scroll.h"
#include "GUI/Controls/CWindowControl_Progress.h"
#include "GUI/Controls/CWindowControl_Radio.h"
#include "GUI/Controls/CWindowControl_Edit.h"
#include "GUI/CWindowManager.h"
#include "GUI/Editors/CIMGEditor.h"
#include "GUI/Editors/Tab/CIMGEditorTab.h"
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
#include "GUI/CGUIManager.h"
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
#include "GUI/Window/CKGMWindow.h"

using namespace std;

// construct/destruct
CKGM::CKGM(void)
{
	// construct objects stored by CKGM
	m_pEntryViewerManager	= new CEntryViewerManager;
	m_pPopupGUIManager		= new CPopupGUIManager;
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
	delete m_pPopupGUIManager;
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
void				CKGM::init(void)
{
	m_pWindowManager->init();
	initInitializationThread();
}

void				CKGM::uninit(void)
{
}

// init tasks (ocurs in original thread)
void				CKGM::initInitializationThread(void)
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

void				CKGM::_init(void)
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
void				CKGM::initBuildMeta(void)
{
	getBuildMeta().setCurrentVersion(1.3f);
	getBuildMeta().setIsAlphaBuild(true);
	getBuildMeta().setCurrentVersionString("1.3" + string(getBuildMeta().isAlphaBuild() ? " Alpha" : ""));
}

void				CKGM::initInstallationMeta(void)
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

void				CKGM::initStoredObjects(void)
{
	// initialize objects stored by CKGM
	// Excludes: CWindowManager and CSortManager
	m_pEntryViewerManager->init();
	m_pPopupGUIManager->init();
	m_pLanguageManager->init();
	m_pLSTProcessingManager->init();
	m_pRecentlyOpenManager->init();
	m_pTaskManager->init();
	m_pSessionManager->init();
	m_pSettingsManager->init();
	m_pUpdateManager->init();
}

void				CKGM::initSingletonObjects(void)
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
	CGUIManager::getInstance()->init();
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

void				CKGM::initStaticData(void)
{
	CRWSection::initStatic();
}

void				CKGM::initEventBinding(void)
{
	auto pOnEntriesExtensionChange = [](void *pData)
	{
		if (getKGM()->getEntryListTab() != nullptr) // When opening an IMG parseIMG() will call addEntry() which will trigger this callback, but the active view instance won't be set yet.
		{
			getKGM()->getEntryListTab()->loadFilter_Type();
		}
	};
	
	auto pOnTaskProgress = [](void *pData)
	{
		getKGM()->getTaskManager()->onTaskProgressTick();
	};
	
	CEventManager::getInstance()->bindEvent(EVENT_onTaskProgress,				pOnTaskProgress);
	CEventManager::getInstance()->bindEvent(EVENT_onParseIMGEntry,		pOnTaskProgress);
	CEventManager::getInstance()->bindEvent(EVENT_onStoreIMGEntry,		pOnTaskProgress);
	CEventManager::getInstance()->bindEvent(EVENT_onAddIMGEntryExtension,		pOnEntriesExtensionChange);
	CEventManager::getInstance()->bindEvent(EVENT_onRemoveIMGEntryExtension,	pOnEntriesExtensionChange);
}

void				CKGM::initSettings(void)
{
	getSettingsManager()->loadSettings();
}

void				CKGM::initLocalization(void)
{
	eLanguage eActiveLanguage = (eLanguage)getKGM()->getSettingsManager()->getSettingInt("Language");
	CLocalizationManager::getInstance()->setActiveLanguage(eActiveLanguage);
	CLocalizationManager::getInstance()->setActiveLanguageName(getKGM()->getLanguageManager()->getLanguageById(eActiveLanguage)->getLanguageName());
	CLocalizationManager::getInstance()->setInstallationPath(CRegistryUtility::getSoftwareValueString("KGM\\InternalSettings", "InstallationPath"));
	CLocalizationManager::getInstance()->loadTranslatedText();
}

void				CKGM::initSorting(void)
{
	m_pSortManager->init();
}

void				CKGM::initOldVersionMigration(void)
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

void				CKGM::initCommandLine(void)
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

void				CKGM::initAutoUpdateCheck(void)
{
	/*
	todo
	This currently calls a onRequestBlah which eventualls calls CTaskManager::onFeatureEnd which can crash before the CIMGEditor object has been created.
	So move this to like lambda: onWindowOpen()
	getKGM()->getTaskManager()->getDispatch()->onRequestAutoUpdate();
	*/
}

void				CKGM::initTempStuff(void)
{
}

// windows/tabs
void				CKGM::openWindow(void)
{
	getWindowManager()->openWindow();
}

void				CKGM::processWindows(void)
{
	getWindowManager()->processWindows();
}

CWindow*			CKGM::getActiveWindow(void)
{
	return CGUIManager::getInstance()->getActiveWindow();
}

CEditorTab*			CKGM::getActiveTab(void)
{
	CKGMWindow *pKGMWindow = (CKGMWindow*) CGUIManager::getInstance()->getEntryByIndex(0);
	CIMGEditor *pIMGEditor = (CIMGEditor*) pKGMWindow->getEntryByIndex(0);
	CEditorTab *pEditorTab = pIMGEditor->getActiveTab();
	return pEditorTab;
}

CIMGEditor*			CKGM::getIMGEditor(void)
{
	CKGMWindow *pKGMWindow = (CKGMWindow*) CGUIManager::getInstance()->getEntryByIndex(0);
	CIMGEditor *pIMGEditor = (CIMGEditor*) pKGMWindow->getEntryByIndex(0);
	return pIMGEditor;
}

CIMGEditorTab*		CKGM::getEntryListTab(void)
{
	CKGMWindow *pKGMWindow = (CKGMWindow*) CGUIManager::getInstance()->getEntryByIndex(0);
	CIMGEditor *pIMGEditor = (CIMGEditor*) pKGMWindow->getEntryByIndex(0);
	CIMGEditorTab *pIMGEditorTab = (CIMGEditorTab*) pIMGEditor->getEntryByIndex(0);
	return pIMGEditorTab;
}

// last used directory
void				CKGM::setLastUsedDirectory(string strHandleName, string strDirectory)
{
	CRegistryUtility::setSoftwareValueString("KGM\\LastUsedDirectories", strHandleName, strDirectory);
}
string				CKGM::getLastUsedDirectory(string strHandleName)
{
	return CRegistryUtility::getSoftwareValueString("KGM\\LastUsedDirectories", strHandleName);
}