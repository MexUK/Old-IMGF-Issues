#include "CCOLManager.h"
#include "CCOLFormat.h"
#include "Data Stream/CDataReader.h"
#include "String/CStringUtility.h"
#include "CCOLVersionManager.h"
#include "File/CFileUtility.h"
#include "Localization/CLocalizationManager.h"
#include "Testing/CDebugger.h"

using namespace std;

CCOLManager::CCOLManager(void)
{
	m_pVersionManager = new CCOLVersionManager;
}
CCOLManager::~CCOLManager(void)
{
	delete m_pVersionManager;
}

void			CCOLManager::init(void)
{
	m_pVersionManager->init();
	initGameMaterials();
}
void			CCOLManager::uninit(void)
{
	m_pVersionManager->uninit();
}

void			CCOLManager::initGameMaterials(void)
{
	// todo
	/*
	umapGameMaterialIds[GAME_III] = {
		{ "grass", 2 },
		{ "dirt", 3 },
		{ "mud", 4 },
		{ "pavement", 5 },
		{ "metal", 6 },
		{ "sand", 18 },
		{ "hedge", 25 },
		{ "stone", 26 }
	};
	umapGameMaterialIds[GAME_VC] = {
		{ "grass", 2 },
		{ "dirt", 3 },
		{ "mud", 4 },
		{ "pavement", 5 },
		{ "metal", 6 },
		{ "glass", 7 },
		{ "metal door", 9 },
		{ "metal fence", 16 },
		{ "sand", 18 },
		{ "water", 19 },
		{ "wood", 20 },
		{ "wood box", 21 },
		{ "wood plank", 22 },
		{ "hedge", 25 },
		{ "stone", 26 },
		{ "bollard", 31 },
		{ "gate", 32 }
	};
	*/
}

CCOLVersion*		CCOLManager::getCOLVersionFromFourCC(string strFourCC)
{
	eCOLVersion eCOLVersionValue = COL_UNKNOWN;
	if (strFourCC == "COLL")
	{
		eCOLVersionValue = COL_1;
	}
	else if (strFourCC == "COL2")
	{
		eCOLVersionValue = COL_2;
	}
	else if (strFourCC == "COL3")
	{
		eCOLVersionValue = COL_3;
	}
	else if (strFourCC == "COL4")
	{
		eCOLVersionValue = COL_4;
	}
	return getInstance()->getVersionManager()->getEntryByVersionId(eCOLVersionValue);
}

string			CCOLManager::getCOLVersionText(eCOLVersion eCOLVersionValue)
{
	CCOLVersion *pCOLVersion = getInstance()->getVersionManager()->getEntryByVersionId(eCOLVersionValue);
	return pCOLVersion == nullptr ? CLocalizationManager::getInstance()->getTranslatedText("UnknownCOLVersion") : pCOLVersion->getText();
}

string			CCOLManager::getFourCCFromCOLVersion(eCOLVersion eCOLVersionValue)
{
	switch (eCOLVersionValue)
	{
	case COL_1: return "COLL";
	case COL_2: return "COL2";
	case COL_3: return "COL3";
	case COL_4: return "COL4";

	case COL_UNKNOWN:
	default:
		return "COLU";
	}
}

uint32		CCOLManager::getEntryHeaderSizeForPacking(eCOLVersion eCOLVersionValue)
{
	uint32 uiHeaderPart1Size = 4 + 4 + 22 + 2 + 40;
	uint32 uiHeaderPart2Size_COLVersion2 = (3 * 2) + (2 * 1) + (7 * 4);
	uint32 uiFinalValue = 0;
	switch (eCOLVersionValue)
	{
	case COL_1:
		uiFinalValue = uiHeaderPart1Size;
		break;
	case COL_2:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2; // 108
		break;
	case COL_3:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2 + 12;
		break;
	case COL_4:
		uiFinalValue = uiHeaderPart1Size + uiHeaderPart2Size_COLVersion2 + 16;
		break;
	}
	uiFinalValue -= 8;
	return uiFinalValue;
}