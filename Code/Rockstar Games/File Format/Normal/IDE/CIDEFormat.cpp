#include "CIDEFormat.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"
#include "CIDEEntry.h"
#include "IDE/Entry/CIDEEntry_Section.h"
#include "IDE/Entry/CIDEEntry_Other.h"
#include "IDE/Entry/Data Entry/CIDEEntry_UnknownSection.h"
#include "IDE/Entry/Data Entry/CIDEEntry_OBJS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TOBJ.h"
#include "IDE/Entry/Data Entry/CIDEEntry_ANIM.h"
#include "IDE/Entry/Data Entry/CIDEEntry_PEDS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_WEAP.h"
#include "IDE/Entry/Data Entry/CIDEEntry_CARS.h"
#include "IDE/Entry/Data Entry/CIDEEntry_HIER.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TXDP.h"
#include "IDE/Entry/Data Entry/CIDEEntry_PATH.h"
#include "IDE/Entry/Data Entry/CIDEEntry_HAND.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TANM.h"
#include "IDE/Entry/Data Entry/CIDEEntry_TREE.h"
#include "IDE/Entry/Data Entry/CIDEEntry_VNOD.h"
#include "IDE/Entry/Data Entry/CIDEEntry_LINK.h"
#include "IDE/Entry/Data Entry/CIDEEntry_MLO.h"
#include "IDE/Entry/Data Entry/CIDEEntry_AMAT.h"
#include "IDE/Entry/Data Entry/CIDEEntry_LODM.h"
#include "IDE/Entry/Data Entry/CIDEEntry_AGRPS.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Light.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Particle.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Ped.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_SunReflection.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Unknown1.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Explosion.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_Ladder.h"
#include "IDE/Entry/Data Entry/2DFX/CIDEEntry_2DFX_SpawnPoint.h"
#include "IDE/Entry/Data Entry/PATH/CIDEEntry_PATH_Group.h"
#include "IDE/Entry/Data Entry/PATH/CIDEEntry_PATH_Node.h"
#include "Section Lines/eSectionLinesEntryType.h"
#include "CDataType.h"
#include "String/CStringUtility.h"
#include "eExceptionCode.h"
#include <unordered_map>

using namespace std;

CIDEFormat::CIDEFormat(void) :
	CSectionLinesFormat()
{
}

// unserialization & serialization
void				CIDEFormat::unserialize(void)
{
	unserializeText();
}

void				CIDEFormat::serialize(void)
{
	serializeText();
}

// unserialization
CIDEEntry_Data*		CIDEFormat::unserializeDataEntry(eIDESection eIDESectionValue)
{
	CIDEEntry_Data *pIDEEntry = nullptr;
	switch (eIDESectionValue)
	{
	case IDE_SECTION_2DFX:
	{
		pair<e2DFXType, uint32> pairData = detect2DFXTypeAndGame();
		pIDEEntry = createDataEntry(eIDESectionValue, pairData.first);
		pIDEEntry->setFormatGames(pairData.second);
		break;
	}
	case IDE_SECTION_PATH:
		pIDEEntry = createDataEntry(eIDESectionValue, detectPATHType());
		break;
	default:
		pIDEEntry = createDataEntry(eIDESectionValue);
		break;
	}
	if (!pIDEEntry)
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}

	return pIDEEntry;
}

pair<e2DFXType, uint32>			CIDEFormat::detect2DFXTypeAndGame(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();

	pDataReader->setActiveLineTokenIndex(4);
	uint32 uiToken5 = pDataReader->readTokenUint32();

	pDataReader->setActiveLineTokenIndex(8);
	uint32 uiToken9 = uiLineTokenCount >= 9 ? pDataReader->readTokenUint32() : -1;

	pDataReader->setActiveLineTokenIndex(0);

	// check for GTA 4 2DFX type - verifying 2dfx type id and token count
	switch (uiToken5)
	{
	case 0:
		if (uiLineTokenCount == 42)
		{
			return{ _2DFX_LIGHT, GAME_FLAG_GTA_IV };
		}
		break;
	case 1:
		if (uiLineTokenCount == 18)
		{
			return{ _2DFX_PARTICLE, GAME_FLAG_GTA_IV };
		}
		break;
	case 2:
		if (uiLineTokenCount == 13)
		{
			return{ _2DFX_EXPLOSION, GAME_FLAG_GTA_IV };
		}
		break;
	case 14:
		if (uiLineTokenCount == 19)
		{
			return{ _2DFX_LADDER, GAME_FLAG_GTA_IV };
		}
		break;
	case 17:
		if (uiLineTokenCount == 13)
		{
			return{ _2DFX_SPAWN_POINT, GAME_FLAG_GTA_IV };
		}
		break;
	}

	// check for GTA 3 era 2DFX type - verifying 2dfx type id and token count
	switch (uiToken9)
	{
	case 0:
		if (uiLineTokenCount == 20)
		{
			return{ _2DFX_LIGHT, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 1:
		if (uiLineTokenCount == 14)
		{
			return{ _2DFX_PARTICLE, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 2:
		if (uiLineTokenCount == 14)
		{
			return{ _2DFX_UNKNOWN_1, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 3:
		if (uiLineTokenCount == 16)
		{
			return{ _2DFX_PED, GAME_FLAG_GTA_VC };
		}
		break;
	case 4:
		if (uiLineTokenCount == 9)
		{
			return{ _2DFX_SUN_REFLECTION, GAME_FLAG_GTA_VC };
		}
		break;
	}

	// check for GTA 4 2DFX type - verifying just the 2dfx type id, because the token count is not documented
	switch (uiToken5)
	{
	case 10: return{ _2DFX_ESCALATOR, GAME_FLAG_GTA_IV };
	case 12: return{ _2DFX_PROC_OBJECT, GAME_FLAG_GTA_IV };
	case 13: return{ _2DFX_WORLD_POINT, GAME_FLAG_GTA_IV };
	case 15: return{ _2DFX_AUDIO, GAME_FLAG_GTA_IV };
	case 18: return{ _2DFX_LIGHT_SHAFT, GAME_FLAG_GTA_IV };
	case 19: return{ _2DFX_SCROLL_BAR, GAME_FLAG_GTA_IV };
	case 21: return{ _2DFX_SWAYABLE_ATTR, GAME_FLAG_GTA_IV };
	case 22: return{ _2DFX_BOUYANCY_ATTR, GAME_FLAG_GTA_IV };
	case 23: return{ _2DFX_WALK_DONT_WALK_ATTR, GAME_FLAG_GTA_IV };
	}

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

eIDEPathType			CIDEFormat::detectPATHType(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	switch (pDataReader->getLineTokens().size())
	{
	case 3:		return IDE_PATH_GROUP;
	case 9:		return IDE_PATH_NODE;
	default:	return IDE_PATH_UNKNOWN;
	}
}

// general
eIDESection					CIDEFormat::getSectionFromText(string strIDESectionText)
{
	strIDESectionText = CStringUtility::zeroPad(strIDESectionText, 4);

	const uint8 *pszData = (const uint8*) CStringUtility::toUpperCase(strIDESectionText.substr(0, 5)).c_str();
	switch ((uint32)*pszData)
	{
	case 'OBJS':	return IDE_SECTION_OBJS;
	case 'TOBJ':	return IDE_SECTION_TOBJ;
	case 'HIER':	return IDE_SECTION_HIER;
	case 'CARS':	return IDE_SECTION_CARS;
	case 'PEDS':	return IDE_SECTION_PEDS;
	case 'PATH':	return IDE_SECTION_PATH;
	case '2DFX':	return IDE_SECTION_2DFX;
	case 'WEAP':	return IDE_SECTION_WEAP;
	case 'ANIM':	return IDE_SECTION_ANIM;
	case 'TXDP':	return IDE_SECTION_TXDP;
	case 'HAND':	return IDE_SECTION_HAND;
	case 'TANM':	return IDE_SECTION_TANM;
	case 'TREE':	return IDE_SECTION_TREE;
	case 'VNOD':	return IDE_SECTION_VNOD;
	case 'LINK':	return IDE_SECTION_LINK;
	case 'MLO\0':	return IDE_SECTION_MLO;
	case 'AMAT':	return IDE_SECTION_AMAT;
	case 'LODM':	return IDE_SECTION_LODM;
	case 'AGRP':
		if (strIDESectionText.length() >= 5 && *(pszData + 4) == 'S')
		{
			return IDE_SECTION_AGRPS;
		}
		break;
	}
	return IDE_SECTION_UNKNOWN;
}

string						CIDEFormat::getSectionText(eIDESection eIDESectionValue)
{
	switch (eIDESectionValue)
	{
	case IDE_SECTION_OBJS:	return "OBJS";
	case IDE_SECTION_TOBJ:	return "TOBJ";
	case IDE_SECTION_HIER:	return "HIER";
	case IDE_SECTION_CARS:	return "CARS";
	case IDE_SECTION_PEDS:	return "PEDS";
	case IDE_SECTION_PATH:	return "PATH";
	case IDE_SECTION_2DFX:	return "2DFX";
	case IDE_SECTION_WEAP:	return "WEAP";
	case IDE_SECTION_ANIM:	return "ANIM";
	case IDE_SECTION_TXDP:	return "TXDP";
	case IDE_SECTION_HAND:	return "HAND";
	case IDE_SECTION_TANM:	return "TANM";
	case IDE_SECTION_TREE:	return "TREE";
	case IDE_SECTION_VNOD:	return "VNOD";
	case IDE_SECTION_LINK:	return "LINK";
	case IDE_SECTION_MLO:	return "MLO";
	case IDE_SECTION_AMAT:	return "AMAT";
	case IDE_SECTION_LODM:	return "LODM";
	case IDE_SECTION_AGRPS:	return "AGRPS";
	default:				return "UNKNOWN";
	}
}

CIDEEntry_Data*				CIDEFormat::createDataEntry(eIDESection eIDESectionValue, uint32 uiSectionSpecificType)
{
	switch (eIDESectionValue)
	{
	case IDE_SECTION_OBJS:	return new CIDEEntry_OBJS(this);
	case IDE_SECTION_TOBJ:	return new CIDEEntry_TOBJ(this);
	case IDE_SECTION_HIER:	return new CIDEEntry_HIER(this);
	case IDE_SECTION_CARS:	return new CIDEEntry_CARS(this);
	case IDE_SECTION_PEDS:	return new CIDEEntry_PEDS(this);
	case IDE_SECTION_PATH:
	{
		eIDEPathType eIDEPathTypeValue = (eIDEPathType)uiSectionSpecificType;
		switch (eIDEPathTypeValue)
		{
		case IDE_PATH_GROUP:	return new CIDEEntry_PATH_Group(this);
		case IDE_PATH_NODE:		return new CIDEEntry_PATH_Node(this);
		default:				return new CIDEEntry_UnknownSection(this);
		}
		break;
	}
	case IDE_SECTION_2DFX:
	{
		e2DFXType e2DFXTypeValue = (e2DFXType)uiSectionSpecificType;
		switch (e2DFXTypeValue)
		{
		case _2DFX_LIGHT:			return new CIDEEntry_2DFX_Light(this);
		case _2DFX_PARTICLE:			return new CIDEEntry_2DFX_Particle(this);
		case _2DFX_PED:				return new CIDEEntry_2DFX_Ped(this);
		case _2DFX_SUN_REFLECTION:	return new CIDEEntry_2DFX_SunReflection(this);
		case _2DFX_UNKNOWN_1:		return new CIDEEntry_2DFX_Unknown1(this);
		default:						return new CIDEEntry_UnknownSection(this);
		}
		break;
	}
	case IDE_SECTION_WEAP:	return new CIDEEntry_WEAP(this);
	case IDE_SECTION_ANIM:	return new CIDEEntry_ANIM(this);
	case IDE_SECTION_TXDP:	return new CIDEEntry_TXDP(this);
	case IDE_SECTION_HAND:	return new CIDEEntry_HAND(this);
	case IDE_SECTION_TANM:	return new CIDEEntry_TANM(this);
	case IDE_SECTION_TREE:	return new CIDEEntry_TREE(this);
	case IDE_SECTION_VNOD:	return new CIDEEntry_VNOD(this);
	case IDE_SECTION_LINK:	return new CIDEEntry_LINK(this);
	case IDE_SECTION_MLO:	return new CIDEEntry_MLO(this);
	case IDE_SECTION_AMAT:	return new CIDEEntry_AMAT(this);
	case IDE_SECTION_LODM:	return new CIDEEntry_LODM(this);
	case IDE_SECTION_AGRPS:	return new CIDEEntry_AGRPS(this);
	default:				return new CIDEEntry_UnknownSection(this);
	}
	return nullptr;
}

uint32						CIDEFormat::detectSectionSpecificType(eIDESection eIDESectionValue)
{
	switch (eIDESectionValue)
	{
	case IDE_SECTION_2DFX:	return detect2DFXTypeAndGame().first;
	case IDE_SECTION_PATH:	return detectPATHType();
	}
	return 0;
}