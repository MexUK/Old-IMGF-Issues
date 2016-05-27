#include "CRWVersionManager.h"

using namespace std;

void											CRWVersionManager::init(void)
{
	initRWVersions();
}
void											CRWVersionManager::uninit(void)
{
	removeAllEntries();
}

void											CRWVersionManager::initRWVersions(void)
{
	CRWVersion *pRWVersion;

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_0_0_0_2);
	pRWVersion->setVersionCC(0x00000302);
	pRWVersion->setVersionName("3.0.0.0.2");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III Alpha [PS2]");
	pRWVersion->setLocalizationKey("RWVersion_1");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_0_0_0_4);
	pRWVersion->setVersionCC(0x00000304);
	pRWVersion->setVersionName("3.0.0.0.4");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III [PS2]");
	pRWVersion->setLocalizationKey("RWVersion_2");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_0_0_0_10);
	pRWVersion->setVersionCC(0x00000310);
	pRWVersion->setVersionName("3.0.0.0.10");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III [PC/PS2]");
	pRWVersion->setLocalizationKey("RWVersion_3");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_1_0_0);
	pRWVersion->setVersionCC(0x0400CFFF);
	pRWVersion->setVersionName("3.1.0.0");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III");
	pRWVersion->setLocalizationKey("RWVersion_4");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_1_0_1);
	pRWVersion->setVersionCC(0x0401FFFF);
	pRWVersion->setVersionName("3.1.0.1");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III [PC/PS2]");
	pRWVersion->setLocalizationKey("RWVersion_5");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_2_0_0);
	pRWVersion->setVersionCC(0x0800FFFF);
	pRWVersion->setVersionName("3.2.0.0");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III);
	pRWVersion->setVersionGamesStr("III [PC]");
	pRWVersion->setLocalizationKey("RWVersion_6");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_3_0_2);
	pRWVersion->setVersionCC(0x0C02FFFF);
	pRWVersion->setVersionName("3.3.0.2");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III | PLATFORMED_GAME_PC_GTA_VC);
	pRWVersion->setVersionGamesStr("III & VC [PC/XBOX]");
	pRWVersion->setLocalizationKey("RWVersion_7");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_4_0_3);
	pRWVersion->setVersionCC(0x1003FFFF);
	pRWVersion->setVersionName("3.4.0.3");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_VC | PLATFORMED_GAME_PC_GTA_SA);
	pRWVersion->setVersionGamesStr("VC & SA [PC/XBOX]");
	pRWVersion->setLocalizationKey("RWVersion_8");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_4_0_5);
	pRWVersion->setVersionCC(0x1005FFFF);
	pRWVersion->setVersionName("3.4.0.5");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_VC);
	pRWVersion->setVersionGamesStr("VC [Android]");
	pRWVersion->setLocalizationKey("RWVersion_9");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_5_0_0);
	pRWVersion->setVersionCC(0x1400FFFF);
	pRWVersion->setVersionName("3.5.0.0");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III | PLATFORMED_GAME_PC_GTA_SA);
	pRWVersion->setVersionGamesStr("III & SA Beta [Android]");
	pRWVersion->setLocalizationKey("RWVersion_10");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_6_0_3);
	pRWVersion->setVersionCC(0x1803FFFF);
	pRWVersion->setVersionName("3.6.0.3");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_SA | PLATFORMED_GAME_PC_MANHUNT);
	pRWVersion->setVersionGamesStr("SA & MH [PC]");
	pRWVersion->setLocalizationKey("RWVersion_11");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_7_0_0);
	pRWVersion->setVersionCC(0x1C000000);
	pRWVersion->setVersionName("3.7.0.0");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_SA);
	pRWVersion->setVersionGamesStr("SA");
	pRWVersion->setLocalizationKey("RWVersion_12");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_MODEL);
	pRWVersion->setVersionId(RW_VERSION_3_7_0_2);
	pRWVersion->setVersionCC(0x1C020037);
	pRWVersion->setVersionName("3.7.0.2");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PS2_BULLY);
	pRWVersion->setVersionGamesStr("Bully [PS2]");
	pRWVersion->setLocalizationKey("RWVersion_14");
	addEntry(pRWVersion);

	pRWVersion = new CRWVersion;
	pRWVersion->setVersionType(TYPE_ANIMATION);
	pRWVersion->setVersionId(RW_VERSION_4_3_3_46);
	pRWVersion->setVersionCC(0x4F464E49);
	pRWVersion->setVersionName("4.3.3.46");
	pRWVersion->setVersionGames(PLATFORMED_GAME_PC_GTA_III | PLATFORMED_GAME_PC_GTA_VC);
	pRWVersion->setVersionGamesStr("III & VC Anim [PC]");
	pRWVersion->setLocalizationKey("RWVersion_13");
	addEntry(pRWVersion);
}

CRWVersion*										CRWVersionManager::getEntryByVersionCC(uint32 uiVersionCC)
{
	for (auto pRWVersion : getEntries())
	{
		if (pRWVersion->getVersionCC() == uiVersionCC)
		{
			return pRWVersion;
		}
	}
	return nullptr;
}

CRWVersion*										CRWVersionManager::getEntryByVersionId(eRWVersion eRWVersion)
{
	for (auto pRWVersion : getEntries())
	{
		if (pRWVersion->getVersionId() == eRWVersion)
		{
			return pRWVersion;
		}
	}
	return nullptr;
}

CRWVersion*										CRWVersionManager::getRWVersionFromGame(ePlatformedGame ePlatformedGame)
{
	switch (ePlatformedGame)
	{
	case PLATFORMED_GAME_PC_GTA_III:	return getEntryByVersionId(RW_VERSION_3_2_0_0);
	case PLATFORMED_GAME_PC_GTA_VC:		return getEntryByVersionId(RW_VERSION_3_4_0_3);
	case PLATFORMED_GAME_PC_GTA_SA:		return getEntryByVersionId(RW_VERSION_3_6_0_3);
	case PLATFORMED_GAME_ANDROID_GTA:	return getEntryByVersionId(RW_VERSION_3_5_0_0);
	case PLATFORMED_GAME_XBOX_GTA:		return getEntryByVersionId(RW_VERSION_3_3_0_2);
	default:							return nullptr;
	}
}

unordered_map<uint32, vector<string>>	CRWVersionManager::getVersionNames(void)
{
	unordered_map<uint32, vector<string>> umapRWVersionNames;
	for (auto pRWVersion : getEntries())
	{
		umapRWVersionNames[pRWVersion->getVersionCC()] = { pRWVersion->getVersionName(), pRWVersion->getLocalizationKey() };
	}
	return umapRWVersionNames;
}