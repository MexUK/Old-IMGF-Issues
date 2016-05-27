#include "CGameManager.h"
#include "Localization/CLocalizationManager.h"

using namespace std;

void										CGameManager::init(void)
{
	initGames();
}
void										CGameManager::uninit(void)
{
	uninitGames();
}

void										CGameManager::initGames(void)
{
	CGame *pGame = nullptr;

	pGame = new CGame;
	pGame->setGameId(GAME_GTA_III);
	pGame->setName("GTA III");
	pGame->setLocalizationKey("Game_III");
	addEntry(pGame);

	pGame = new CGame;
	pGame->setGameId(GAME_GTA_VC);
	pGame->setName("GTA VC");
	pGame->setLocalizationKey("Game_VC");
	addEntry(pGame);

	pGame = new CGame;
	pGame->setGameId(GAME_GTA_SA);
	pGame->setName("GTA SA");
	pGame->setLocalizationKey("Game_SA");
	addEntry(pGame);

	pGame = new CGame;
	pGame->setGameId(GAME_MANHUNT);
	pGame->setName("Manhunt");
	pGame->setLocalizationKey("Game_Manhunt");
	addEntry(pGame);

	pGame = new CGame;
	pGame->setGameId(GAME_BULLY);
	pGame->setName("BULLY");
	pGame->setLocalizationKey("Game_Bully");
	addEntry(pGame);

	pGame = new CGame;
	pGame->setGameId(GAME_SOL);
	pGame->setName("SOL");
	pGame->setLocalizationKey("Game_SOL");
	addEntry(pGame);
}
void										CGameManager::uninitGames(void)
{
	removeAllEntries();
}

string										CGameManager::getGameName(eGame eGameValue)
{
	for (CGame *pGame : getEntries())
	{
		if (pGame->getGameId() == eGameValue)
		{
			return pGame->getName();
		}
	}
	return CLocalizationManager::getInstance()->getTranslatedText("UnknownGame");
}