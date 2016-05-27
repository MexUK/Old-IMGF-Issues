#ifndef CRWSection_H
#define CRWSection_H

#include <d2d1.h>
#include "CRWSectionContainer.h"
#include "Engine/RW/eRWSection.h"
#include "Engine/RW/eRWVersion.h"
#include <vector>
#include <unordered_map>

class CRWSection;

class CRWSection : public CRWSectionContainer
{
public:
	CRWSection(void);

	static void						initStatic(void);

	void							serialize(void);

	CRWSection*						addSection(eRWSection eRWSectionValue, eRWVersion eRWVersionValue);
	void							removeSection(void);
	uint32							getSectionIndex(void);

	void							fillPlaceholdersForSerialization(void);

	static CRWSection*				createRWSection(eRWSection eRWSectionValue);
	static bool						doesRWSectionContainStruct(eRWSection eRWSectionValue);
	static bool						doesRWSectionContainStruct_BeforeInit(eRWSection eRWSectionValue);

	static std::vector<eRWSection>					getRWSections(void);
	static std::unordered_map<eRWSection, bool>&	getRWSectionsContainingStruct(void) { return m_umapRWSectionsContainingStruct; }

	void							setSectionId(eRWSection eSectionId) { m_eSectionId = eSectionId; }
	eRWSection						getSectionId(void) { return m_eSectionId; }

	void							setSectionSize(uint32 uiSectionSize) { m_uiSectionSize = uiSectionSize; }
	uint32							getSectionSize(void) { return m_uiSectionSize; }

	void							setSectionRWVersion(uint32 uiSectionRWVersion) { m_uiSectionRWVersion = uiSectionRWVersion; }
	uint32							getSectionRWVersion(void) { return m_uiSectionRWVersion; }

	void							setSectionHeaderSkipped(bool bSectionHeaderSkipped) { m_bSectionHeaderSkipped = bSectionHeaderSkipped; }
	bool							isSectionHeaderSkipped(void) { return m_bSectionHeaderSkipped; }

private:
	static void						initRWSections(void);

protected:
	eRWSection						m_eSectionId;
	uint32							m_uiSectionSize;
	uint32							m_uiSectionRWVersion;

private:
	uint8											m_bSectionHeaderSkipped		: 1;
	static std::unordered_map<eRWSection, bool>		m_umapRWSectionsContainingStruct;
};

#endif