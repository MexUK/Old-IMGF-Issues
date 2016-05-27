#include "CLSTSection.h"
#include "CLSTEntry.h"
#include "String/CStringUtility.h"

using namespace std;

void					CLSTSection::unload(void)
{
	removeAllEntries();
}

CLSTEntry*				CLSTSection::getEntryByName(string strName)
{
	strName = CStringUtility::toUpperCase(strName);
	for (CLSTEntry *pLSTEntry : m_vecEntries)
	{
		if (CStringUtility::toUpperCase(pLSTEntry->getName()) == strName)
		{
			return pLSTEntry;
		}
	}
	return nullptr;
}

bool					CLSTSection::doesEntryExistByName(string strName)
{
	return getEntryByName(strName) != nullptr;
}