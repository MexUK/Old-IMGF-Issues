#include "CRageResourceType.h"

using namespace std;

string				CRageResourceType::getResourceName(void)
{
	if (this == nullptr)
	{
		return "Unknown";
	}
	else
	{
		return m_strName;
	}
}

eRageResourceType	CRageResourceType::getResourceId(void)
{
	if (this == nullptr)
	{
		return RAGE_UNKNOWN;
	}
	else
	{
		return m_eResourceTypeId;
	}
}