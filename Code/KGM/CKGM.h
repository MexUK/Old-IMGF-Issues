#ifndef CKGM_H
#define CKGM_H

#include "Types.h"
#include "Globals.h"
#include "CManager.h"
#include "CBuildMeta.h"
#include "CInstallationMeta.h"

#define WINAPI __stdcall
uint32 WINAPI		onInitializationThreadStart(void *pThreadParameter);

class CMainWindow;
class CWindow;
class CWindowTab;
class CEntryListWindow;
class CEntryListWindowTab;

class CSessionManager;
class CRecentlyOpenManager;
class CGUIManager;
class CTaskManager;
class CLSTProcessingManager;
class CLocalizationManager;
class CLanguageManager;
class CDumpManager;
class CSettingsManager;
class CSortManager;
class CEntryViewerManager;
class CUpdateManager;
class CLastUsedValueManager;
class CWindowManager;

class CWnd; // temp

class CKGM : public CManager
{
public:
	CKGM(void);
	~CKGM(void);

	void						init(void);
	void						uninit(void);

	void						_init(void);

	void						openMainWindow(void);
	void						processWindows(void);
	
	CBuildMeta&					getBuildMeta(void) { return m_buildMeta; }
	CInstallationMeta&			getInstallationMeta(void) { return m_installationMeta; }
	
	CMainWindow*				getMainWindow(void);
	CWindow*					getActiveWindow(void);
	CWindowTab*					getActiveTab(void);
	CEntryListWindow*			getEntryListWindow(void);
	CEntryListWindowTab*		getEntryListTab(void);
	
	CDumpManager*				getDumpManager(void) { return m_pDumpManager; }
	CEntryViewerManager*		getEntryViewerManager(void) { return m_pEntryViewerManager; }
	CGUIManager*				getGUIManager(void) { return m_pGUIManager; }
	CLanguageManager*			getLanguageManager(void) { return m_pLanguageManager; }
	CLSTProcessingManager*		getLSTProcessingManager(void) { return m_pLSTProcessingManager; }
	CWindowManager*				getWindowManager(void) { return m_pWindowManager; }
	CRecentlyOpenManager*		getRecentlyOpenManager(void) { return m_pRecentlyOpenManager; }
	CTaskManager*				getTaskManager(void) { return m_pTaskManager; }
	CSessionManager*			getSessionManager(void) { return m_pSessionManager; }
	CSettingsManager*			getSettingsManager(void) { return m_pSettingsManager; }
	CSortManager*				getSortManager(void) { return m_pSortManager; }
	CUpdateManager*				getUpdateManager(void) { return m_pUpdateManager; }
	CLastUsedValueManager*		getLastUsedValueManager(void) { return m_pLastUsedValueManager; }

	void						setLastUsedDirectory(std::string strHandleName, std::string strDirectory);
	std::string					getLastUsedDirectory(std::string strHandleName);

	CWnd*						getDialog(void) { return nullptr; } // temp

private:
	void						initInitializationThread(void);

	void						initBuildMeta(void);
	void						initInstallationMeta(void);
	void						initStoredObjects(void);
	void						initSingletonObjects(void);
	void						initStaticData(void);
	void						initEventBinding(void);
	void						initSettings(void);
	void						initLocalization(void);
	void						initSorting(void);
	void						initOldVersionMigration(void);
	void						initCommandLine(void);
	void						initAutoUpdateCheck(void);
	void						initTempStuff(void);

private:
	CBuildMeta					m_buildMeta;
	CInstallationMeta			m_installationMeta;

	CDumpManager*				m_pDumpManager;
	CEntryViewerManager*		m_pEntryViewerManager;
	CGUIManager*				m_pGUIManager;
	CLanguageManager*			m_pLanguageManager;
	CLSTProcessingManager*		m_pLSTProcessingManager;
	CWindowManager*				m_pWindowManager;
	CRecentlyOpenManager*		m_pRecentlyOpenManager;
	CTaskManager*				m_pTaskManager;
	CSessionManager*			m_pSessionManager;
	CSettingsManager*			m_pSettingsManager;
	CSortManager*				m_pSortManager;
	CUpdateManager*				m_pUpdateManager;
	CLastUsedValueManager*		m_pLastUsedValueManager;
};

#endif