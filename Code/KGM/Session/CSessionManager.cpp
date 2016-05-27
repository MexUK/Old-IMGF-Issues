#include "CSessionManager.h"
#include "String/CStringUtility.h"
#include "Path/CPathUtility.h"
#include "Vector/CVectorUtility.h"
#include "Registry/CRegistryUtility.h"
#include "Globals.h"
#include "CKGM.h"
#include "IMG/CIMGEntry.h"
#include "Localization/CLocalizationManager.h"

using namespace std;

void		CSessionManager::init(void)
{
	loadSessions();
}
void		CSessionManager::uninit(void)
{
	unloadSessions();
}

void		CSessionManager::loadSessions(void)
{
	// todo 
	return;

	removeAllEntries();

	for (auto it : getKGM()->getSessionManager()->getSessionsContainer())
	{
		// todo DeleteMenu(getKGM()->m_hSubMenu_File_Sessions, it.first, 0);
	}
	getKGM()->getSessionManager()->getSessionsContainer().clear();
	// todo DeleteMenu(getKGM()->m_hSubMenu_File_Sessions, 1981, 0);

	uint32 uiSessionCount = CRegistryUtility::getSoftwareValueInt("KGM\\Sessions", "Count"); // todo - use like getKGM()->getInstallationMeta().getSessionsRegistryKey(); - same for all CRegistryUtility calls.
	for (int32 i = uiSessionCount; i >= 1; i--)
	{
		string strIMGPaths = CRegistryUtility::getSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(i));
		deque<string> deqIMGPaths = CVectorUtility::convertVectorToDeque(CStringUtility::split(strIMGPaths, "; "));
		string strSessionName = deqIMGPaths[0];
		deqIMGPaths.pop_front();
		uint32 j2 = deqIMGPaths.size();
		vector<string> vecIMGPaths;
		for (uint32 i2 = 0; i2 < j2; i2++)
		{
			vecIMGPaths.push_back(deqIMGPaths[i2]);
			deqIMGPaths[i2] = CPathUtility::getFileName(deqIMGPaths[i2]);
		}
		// todo AppendMenu(getKGM()->m_hSubMenu_File_Sessions, MF_STRING, 1900 + i, CStringUtility::convertStdStringToStdWString(CStringUtility::toString((uiSessionCount - i) + 1) + ") " + CStringUtility::escapeMenuText(strSessionName) + " (" + CStringUtility::toString(j2) + " tab" + (j2 == 1 ? "" : "s") + ")").c_str());

		getKGM()->getSessionManager()->getSessionsContainer()[1900 + i] = strIMGPaths;

		CSession *pSession = new CSession;
		pSession->m_strName = strSessionName;
		pSession->m_vecPaths = vecIMGPaths;
		addEntry(pSession);
	}

	if (uiSessionCount == 0)
	{
		// todo AppendMenu(getKGM()->m_hSubMenu_File_Sessions, MF_STRING | MF_DISABLED, 1981, CLocalizationManager::getInstance()->getTranslatedTextW("Menu_Sessions_NoSessions").c_str());
	}
}
void		CSessionManager::unloadSessions(void)
{
	removeAllEntries();
}

CSession*	CSessionManager::addSession(string strSessionName, vector<string>& vecPaths)
{
	uint32 uiSessionIndex = getEntryCount() + 1;

	CSession *pSession = new CSession;
	pSession->m_strName = strSessionName;
	pSession->m_vecPaths = vecPaths;
	addEntry(pSession);

	CRegistryUtility::setSoftwareValueInt("KGM\\Sessions", "Count", uiSessionIndex);
	CRegistryUtility::setSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(uiSessionIndex), pSession->serialize());

	return pSession;
}

void		CSessionManager::removeSession(CSession *pSession)
{
	removeEntry(pSession);
	
	uint32 uiSessionIndex = getIndexByEntry(pSession);
	CRegistryUtility::removeSoftwareValue("KGM\\Sessions", "Data_" + CStringUtility::toString(uiSessionIndex));

	uint32 uiSessionCount = CRegistryUtility::getSoftwareValueInt("KGM\\Sessions", "Count");
	for (uint32 i = uiSessionIndex; i < uiSessionCount; i++)
	{
		string strIMGPaths2 = CRegistryUtility::getSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(i + 1));
		CRegistryUtility::setSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(i), strIMGPaths2);
	}
	CRegistryUtility::removeSoftwareValue("KGM\\Sessions", "Data_" + CStringUtility::toString(uiSessionCount));
	CRegistryUtility::setSoftwareValueInt("KGM\\Sessions", "Count", uiSessionCount - 1);
}

CSession*		CSessionManager::getSessionByName(string strSessionName)
{
	for(auto pSession : getEntries())
	{
		if(pSession->m_strName == strSessionName)
		{
			return pSession;
		}
	}
	return nullptr;
}