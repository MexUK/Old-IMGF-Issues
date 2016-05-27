#include "CRageManager.h"
#include "String/CStringUtility.h"
#include "CRageResourceTypeManager.h"

using namespace std;

CRageManager::CRageManager(void)
{
	m_pResourceTypeManager = new CRageResourceTypeManager;
}
CRageManager::~CRageManager(void)
{
	delete m_pResourceTypeManager;
}

void					CRageManager::init(void)
{
	getResourceTypeManager()->init();
}
void					CRageManager::uninit(void)
{
	getResourceTypeManager()->uninit();
}

uint32					CRageManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32					CRageManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}