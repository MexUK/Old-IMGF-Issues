#include "CSession.h"
#include "String/CStringUtility.h"
#include "Registry/CRegistryUtility.h"
#include "Globals.h"
#include "CKGM.h"
#include "CSessionManager.h"
#include "IMG/CIMGEntry.h"

using namespace std;

string			CSession::serialize(void)
{
	return m_strName + "; " + CStringUtility::join(m_vecPaths, "; ");
}

void			CSession::setName(string strSessionName)
{
	m_strName = strSessionName;
	uint32 uiSessionIndex = getKGM()->getSessionManager()->getIndexByEntry(this);
	CRegistryUtility::setSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(uiSessionIndex), serialize());
}

void			CSession::setPaths(vector<string> vecPaths)
{
	m_vecPaths = vecPaths;
	uint32 uiSessionIndex = getKGM()->getSessionManager()->getIndexByEntry(this);
	CRegistryUtility::setSoftwareValueString("KGM\\Sessions", "Data_" + CStringUtility::toString(uiSessionIndex), serialize());
}