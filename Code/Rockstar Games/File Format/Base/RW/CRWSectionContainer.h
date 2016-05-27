#ifndef CRWSectionContainer_H
#define CRWSectionContainer_H

#include "Pool/CVectorPool.h"
// #include "CRWSection.h" // todo
#include "Engine/RW/eRWSection.h"
#include "Engine/RW/eRWVersion.h"
#include "CVector4ui8.h"
#include <vector>

class CRWSection;
class C2dEffect;
class CRWFormat;

class CRWSectionContainer : public CVectorPool<CRWSection*>
{
public:
	CRWSectionContainer(void);

	void									unload(void);

	virtual void							unserialize(void) = 0;
	virtual void							serialize(void) = 0;

	CRWSection*								addSection(eRWSection eRWSectionValue, eRWVersion eRWVersionValue);
	void									removeSectionByIndex(uint32 uiSectionIndex);
	std::vector<CRWSection*>				getSectionsByType(eRWSection eRWSectionValue, bool bCheckRecursiveSections = true);
	uint32									getSectionCountByType(eRWSection eRWSectionValue, bool bCheckRecursiveSections = true);

	void									removePrelightning(void);
	void									setPrelightningColour(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha);
	void									applyPrelightningColourOffset(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha);

	CRWFormat*								getRWFormat(void) { return nullptr; } // todo

	void									setParentNode(CRWSection* pParentNode) { m_pParentNode = pParentNode; }
	CRWSection*								getParentNode(void) { return m_pParentNode; }

	void									setDVCColours(std::vector<CVector4ui8>& vecDVCColours);
	std::vector<CVector4ui8>				getDVCColours(void);

	void									setNVCColours(std::vector<CVector4ui8>& vecNVCColours);
	std::vector<CVector4ui8>				getNVCColours(void);

	void									set2dEffects(std::vector<C2dEffect*>& vec2dEffects);				// the array of 2d effects are all applied to all 2d effects sections
	void									set2dEffects(std::vector<std::vector<C2dEffect*>>& vec2dEffects);	// index into outer vector represents 2d effects section index
	std::vector<std::vector<C2dEffect*>>	get2dEffects(void);													// index into outer vector represents 2d effects section index

private:
	CRWSection*								m_pParentNode;
};

#endif