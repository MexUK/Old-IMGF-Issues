#include "CCOLFormat.h"
#include "String/CStringUtility.h"
#include "File/CFileUtility.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CCOLManager.h"
#include "Testing/CDebugger.h"
#include <algorithm>

using namespace std;

void				CCOLFormat::unload(void)
{
	removeAllEntries();
}

void				CCOLFormat::unserialize(void)
{
	while (!CDataReader::getInstance()->isEOF())
	{
		CCOLEntry *pCOLEntry = new CCOLEntry(this);
		m_vecEntries.push_back(pCOLEntry);
		pCOLEntry->unserialize();
	}
}

void				CCOLFormat::serialize(void)
{
	for (CCOLEntry *pCOLEntry : getEntries())
	{
		pCOLEntry->serialize();
	}
}

void				CCOLFormat::setCOLVersion(CCOLVersion *pCOLVersion)
{
	for (auto pCOLEntry : getEntries())
	{
		pCOLEntry->setCOLVersion(pCOLVersion);
	}
}

vector<string>		CCOLFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (auto pCOLEntry : getEntries())
	{
		vecModelNames.push_back(pCOLEntry->getModelName());
	}
	return vecModelNames;
}

CCOLEntry*			CCOLFormat::getEntryByName(string strName)
{
	strName = CStringUtility::toUpperCase(strName);
	for (auto pCOLEntry : getEntries())
	{
		if (strName == CStringUtility::toUpperCase(pCOLEntry->getModelName()))
		{
			return pCOLEntry;
		}
	}
	return nullptr;
}

vector<CCOLEntry*>	CCOLFormat::getEntriesByModelName(string strModelName)
{
	vector<CCOLEntry*> vecCOLEntries;
	strModelName = CStringUtility::toUpperCase(strModelName);
	for (auto pCOLEntry : getEntries())
	{
		if (CStringUtility::toUpperCase(pCOLEntry->getModelName()) == strModelName)
		{
			vecCOLEntries.push_back(pCOLEntry);
		}
	}
	return vecCOLEntries;
}

void				CCOLFormat::sort(void)
{
	std::sort(getEntries().begin(), getEntries().end(), sortFunction);
}

bool				CCOLFormat::sortFunction(CCOLEntry *pCOLEntry1, CCOLEntry *pCOLEntry2)
{
	return pCOLEntry1->getModelName() < pCOLEntry2->getModelName();
}