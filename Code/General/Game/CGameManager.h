#ifndef CGameManager_H
#define CGameManager_H

#include "CManager.h"
#include "CSingleton.h"
#include "Pool/CVectorPool.h"
#include "CGame.h"
#include "eGame.h"
#include <string>

class CGameManager : public CManager, public CSingleton<CGameManager>, public CVectorPool<CGame*>
{
public:
	void											init(void);
	void											uninit(void);

	std::string										getGameName(eGame eGameValue);

private:
	void											initGames(void);
	void											uninitGames(void);
};

#endif