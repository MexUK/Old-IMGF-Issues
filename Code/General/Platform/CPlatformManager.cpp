#include "CPlatformManager.h"
#include "Localization/CLocalizationManager.h"

using namespace std;

void										CPlatformManager::init(void)
{
	initPlatforms();
}
void										CPlatformManager::uninit(void)
{
	uninitPlatforms();
}

void										CPlatformManager::initPlatforms(void)
{
	CPlatform *pPlatform = nullptr;

	pPlatform = new CPlatform;
	pPlatform->setPlatformId(PLATFORM_PC);
	pPlatform->setName("PC");
	addEntry(pPlatform);

	pPlatform = new CPlatform;
	pPlatform->setPlatformId(PLATFORM_XBOX);
	pPlatform->setName("XBOX");
	addEntry(pPlatform);

	pPlatform = new CPlatform;
	pPlatform->setPlatformId(PLATFORM_PS2);
	pPlatform->setName("PS2");
	addEntry(pPlatform);
}
void										CPlatformManager::uninitPlatforms(void)
{
	removeAllEntries();
}

string										CPlatformManager::getPlatformName(ePlatform ePlatformValue)
{
	for (CPlatform *pPlatform : getEntries())
	{
		if (pPlatform->getPlatformId() == ePlatformValue)
		{
			return pPlatform->getName();
		}
	}
	return CLocalizationManager::getInstance()->getTranslatedText("UnknownPlatform");
}