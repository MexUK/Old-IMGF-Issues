#include "CRWVersion.h"

using namespace std;

string			CRWVersion::getText(void)
{
	return "RW " + getVersionName() + " (" + getVersionGamesStr() + ")";
}

ePlatformedGame			CRWVersion::getFirstGame(void)
{
	for (uint32 i = 1, j = 0xFFFFFFFF; i < j; i+=i)
	{
		if ((m_uiVersionGames & i) == i)
		{
			return (ePlatformedGame)i;
		}
	}
	return PLATFORMED_GAME_UNKNOWN;
}

vector<ePlatformedGame>	CRWVersion::getGames(void)
{
	vector<ePlatformedGame> vecGames;
	uint32 uiGame = 1;
	for (uint32 i = 0, j = 32; i < j; i++)
	{
		if ((m_uiVersionGames & uiGame) == uiGame)
		{
			vecGames.push_back((ePlatformedGame) uiGame);
		}

		uiGame += uiGame;
	}
	return vecGames;
}