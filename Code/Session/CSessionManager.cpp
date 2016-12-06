#include "CSessionManager.h"
#include "Type/String/CString2.h"
#include "Path/CPathManager.h"
#include "Type/StdVector/CStdVector.h"
#include "Registry/CRegistryManager.h"
#include "Globals.h"
#include "CKGM.h"
#include "Format/RockstarGames/IMG/CIMGEntry.h"
#include "Localization/CLocalizationManager.h"

using namespace std;
using namespace mcore;

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

	uint32 uiSessionCount = CRegistryManager::getSoftwareValueInt("KGM\\Sessions", "Count"); // todo - use like getKGM()->getInstallationMeta().getSessionsRegistryKey(); - same for all CRegistryUtility calls.
	for (int32 i = uiSessionCount; i >= 1; i--)
	{
		string strIMGPaths = CRegistryManager::getSoftwareValueString("KGM\\Sessions", "Data_" + CString2::toString(i));
		deque<string> deqIMGPaths = CStdVector::convertVectorToDeque(CString2::split(strIMGPaths, "; "));
		string strSessionName = deqIMGPaths[0];
		deqIMGPaths.pop_front();
		uint32 j2 = deqIMGPaths.size();
		vector<string> vecIMGPaths;
		for (uint32 i2 = 0; i2 < j2; i2++)
		{
			vecIMGPaths.push_back(deqIMGPaths[i2]);
			deqIMGPaths[i2] = CPathManager::getFileName(deqIMGPaths[i2]);
		}
		// todo AppendMenu(getKGM()->m_hSubMenu_File_Sessions, MF_STRING, 1900 + i, CString2::convertStdStringToStdWString(CString2::toString((uiSessionCount - i) + 1) + ") " + CString2::escapeMenuText(strSessionName) + " (" + CString2::toString(j2) + " tab" + (j2 == 1 ? "" : "s") + ")").c_str());

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

	CRegistryManager::setSoftwareValueInt("KGM\\Sessions", "Count", uiSessionIndex);
	CRegistryManager::setSoftwareValueString("KGM\\Sessions", "Data_" + CString2::toString(uiSessionIndex), pSession->serialize());

	return pSession;
}

void		CSessionManager::removeSession(CSession *pSession)
{
	removeEntry(pSession);
	
	uint32 uiSessionIndex = getIndexByEntry(pSession);
	CRegistryManager::removeSoftwareValue("KGM\\Sessions", "Data_" + CString2::toString(uiSessionIndex));

	uint32 uiSessionCount = CRegistryManager::getSoftwareValueInt("KGM\\Sessions", "Count");
	for (uint32 i = uiSessionIndex; i < uiSessionCount; i++)
	{
		string strIMGPaths2 = CRegistryManager::getSoftwareValueString("KGM\\Sessions", "Data_" + CString2::toString(i + 1));
		CRegistryManager::setSoftwareValueString("KGM\\Sessions", "Data_" + CString2::toString(i), strIMGPaths2);
	}
	CRegistryManager::removeSoftwareValue("KGM\\Sessions", "Data_" + CString2::toString(uiSessionCount));
	CRegistryManager::setSoftwareValueInt("KGM\\Sessions", "Count", uiSessionCount - 1);
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