#ifndef CRWFormat_H
#define CRWFormat_H

#include "Types.h"
#include "CFormat.h"
#include "CRWSectionContainer.h"
#include "Engine/RW/CRWVersion.h"
#include "Engine/RW/eRWSection.h"
#include "Game/ePlatformedGame.h"
#include <vector>
#include <unordered_map>

class CRWSection;
class C2dEffect;
class CTextureEntry;

class CRWFormat : public CFormat, public CRWSectionContainer
{
public:
	CRWFormat(void) :
		CFormat(true, LITTLE_ENDIAN),
		m_pRWVersion(nullptr)
	{};

	void												setRWVersion(CRWVersion* pRWVersion, eRWSection eRWSectionValue = (eRWSection)-1);	// Sets the RW version for all sections. Use -1 for eRWSectionValue to apply to all sections.
	CRWVersion*											getRWVersion(eRWSection eRWSectionValue = (eRWSection)-1);							// Sets the RW version for all sections. Use -1 for eRWSectionValue to apply to all sections.

	CRWSection*											getLatestClump(void) { return nullptr; } // todo

	std::vector<CTextureEntry*>&						getTextureEntries(void) { return m_vecTextureEntries; } // todo - inconsisteny in func name: Entry - remove word: Entry
	std::unordered_map<std::string, CTextureEntry*>&	getTextureEntryNamesUpper(void) { return m_umapTexturesByNameUpper; } // todo - inconsisteny in func name: Entry - remove word: Entry
	void												removeTextureEntry(CTextureEntry *pTextureEntry);

	CTextureEntry*										getTextureByDiffuseName(std::string strTextureDiffuseName);
	CTextureEntry*										getTextureByDiffuseOrAlphaName(std::string strTextureName);
	std::vector<std::string>							getTextureNames(void);

	void												fixAlphaTextureStates(void);
	bool												doesHaveTextureWithInvalidTXDRasterDataFormat(void);

	void												setGames(std::vector<ePlatformedGame> vecGames) { m_vecGames = vecGames; }
	std::vector<ePlatformedGame>&						getGames(void) { return m_vecGames; }

private:
	void												unserialize(void);
	void												serialize(void);

	void												loadTextureEntries(void);

private:
	CRWVersion*											m_pRWVersion;
	std::vector<CTextureEntry*>							m_vecTextureEntries; // todo - inconsisteny in func name: Entry - remove word: Entry
	std::vector<ePlatformedGame>						m_vecGames;
	std::unordered_map<std::string, CTextureEntry*>		m_umapTexturesByNameUpper;
};

#endif