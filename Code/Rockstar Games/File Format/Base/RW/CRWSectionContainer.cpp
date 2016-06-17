#include "CRWSectionContainer.h"
#include "CRWSection.h"
#include "Engine/RW/CRWManager.h"
#include "Sections/CRWSection_Geometry.h"
#include "Sections/CRWSection_Geometry.h"
#include "Sections/CRWSection_ExtraVertColour.h"
#include "Sections/CRWSection_Extension.h"
#include "Sections/CRWSection_2dEffect.h"
#include "String/CStringUtility.h"
#include "Data Stream/CDataReader.h"
#include "Data Stream/CDataWriter.h"

using namespace std;

CRWSectionContainer::CRWSectionContainer(void) :
	m_pParentNode(nullptr)
{
}

void				CRWSectionContainer::unload(void)
{
	for (CRWSection *pRWSection : getEntries())
	{
		pRWSection->unload();
		delete pRWSection;
	}
}

void				CRWSectionContainer::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::getInstance();

	CRWSectionContainer *pParentRWSectionContainer = this;
	uint32 uiSectionDepth = 0;
	vector<uint32>
		vecSectionSizes,
		vecByteCountsRead;
	while (!pDataReader->isEOF())
	{
		if (!pDataReader->canRead(12))
		{
			break;
		}

		// read section header
		uint32
			uiRWSectionId = pDataReader->readUint32(),
			uiRWSectionSize = pDataReader->readUint32(),
			uiRWSectionRWVersion = pDataReader->readUint32();
		eRWSection eRWSectionValue = (eRWSection)uiRWSectionId;

		// enter new section
		if (uiRWSectionId != 1)
		{
			vecSectionSizes.push_back(uiRWSectionSize);
			vecByteCountsRead.push_back(0);
			uiSectionDepth++;
		}

		// read struct header
		if (CRWSection::doesRWSectionContainStruct(eRWSectionValue))
		{
			uint32
				uiRWSectionId2 = pDataReader->readUint32(),
				uiRWSectionSize2 = pDataReader->readUint32(),
				uiRWSectionRWVersion2 = pDataReader->readUint32();
		}

		// create RW section
		CRWSection *pRWSection = CRWSection::createRWSection(eRWSectionValue);

		// unserialize RW section
		uint32 uiByteCountBefore = pDataReader->getSeek();
		pRWSection->unserialize();
		uint32 uiByteCountAfter = pDataReader->getSeek();
		vecByteCountsRead[uiSectionDepth - 1] += uiByteCountAfter - uiByteCountBefore;

		// store RW section
		pParentRWSectionContainer->addEntry(pRWSection);

		// reset section depth
		if (vecByteCountsRead[uiSectionDepth - 1] >= vecSectionSizes[uiSectionDepth - 1])
		{
			pParentRWSectionContainer = pParentRWSectionContainer->getParentNode();
			vecSectionSizes.pop_back();
			vecByteCountsRead[uiSectionDepth - 2] += vecByteCountsRead[uiSectionDepth - 1];
			vecByteCountsRead.pop_back();
			uiSectionDepth--;
		}
	}
}

void				CRWSectionContainer::serialize(void)
{
	for (CRWSection *pRWSection : getEntries())
	{
		CDataWriter *pDataWriter = nullptr; // todo CDataWriter::addInstance();
		pDataWriter->setStreamType(DATA_STREAM_MEMORY);

		pRWSection->CRWSection::serialize();	// base method,              RW section header
		pRWSection->serialize();				// derived method (virtual), RW section body
		pRWSection->fillPlaceholdersForSerialization();

		// todo CDataWriter::moveLatestInstanceDown();
	}
}

CRWSection*			CRWSectionContainer::addSection(eRWSection eRWSectionValue, eRWVersion eRWVersionValue)
{
	CRWSection *pRWSection = CRWSection::createRWSection(eRWSectionValue);
	if (pRWSection == nullptr)
	{
		return nullptr;
	}
	pRWSection->setParentNode(nullptr);
	pRWSection->setSectionId(eRWSectionValue);
	pRWSection->setSectionSize(0);
	pRWSection->setSectionRWVersion(eRWVersionValue);
	addEntry(pRWSection);
	return pRWSection;
}

void				CRWSectionContainer::removeSectionByIndex(uint32 uiSectionIndex)
{
	CRWSection *pRWSection = m_vecEntries[uiSectionIndex];
	auto it = std::find(m_vecEntries.begin(), m_vecEntries.end(), pRWSection);
	if (it != m_vecEntries.end())
	{
		m_vecEntries.erase(it);
	}
	pRWSection->unload();
	delete pRWSection;
}

vector<CRWSection*>	CRWSectionContainer::getSectionsByType(eRWSection eRWSectionValue, bool bCheckRecursiveSections)
{
	vector<CRWSection*>
		vecRWSectionsToSearch = getEntries(),
		vecRWSectionsToReturn;
	for (uint32 i = 0; i < vecRWSectionsToSearch.size(); i++)
	{
		CRWSection *pRWSection = vecRWSectionsToSearch[i];
		if (bCheckRecursiveSections)
		{
			for (CRWSection *pRWSection2 : pRWSection->getEntries())
			{
				vecRWSectionsToSearch.push_back(pRWSection2);
			}
		}

		if (pRWSection->getSectionId() == eRWSectionValue)
		{
			vecRWSectionsToReturn.push_back(pRWSection);
		}
	}
	return vecRWSectionsToReturn;
}

uint32				CRWSectionContainer::getSectionCountByType(eRWSection eRWSectionValue, bool bCheckRecursiveSections)
{
	vector<CRWSection*> vecRWSectionsToSearch = getEntries();
	uint32 uiSectionCount = 0;
	for (uint32 i = 0; i < vecRWSectionsToSearch.size(); i++)
	{
		CRWSection *pRWSection = vecRWSectionsToSearch[i];
		if (bCheckRecursiveSections)
		{
			for (CRWSection *pRWSection2 : pRWSection->getEntries())
			{
				vecRWSectionsToSearch.push_back(pRWSection2);
			}
		}

		if (pRWSection->getSectionId() == eRWSectionValue)
		{
			uiSectionCount++;
		}
	}
	return uiSectionCount;
}

void				CRWSectionContainer::removePrelightning(void)
{
	if (((CRWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		((CRWSection_Geometry*)this)->removePrelightning();
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->removePrelightning();
	}
}

void				CRWSectionContainer::setPrelightningColour(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha)
{
	CVector4ui8 vecColour; // todo - add CColour4uc : CVector4ui8
	vecColour.m_x = ssRed;
	vecColour.m_y = ssGreen;
	vecColour.m_z = ssBlue;
	vecColour.m_w = ssAlpha;

	vector<CVector4ui8> vecColours = { vecColour }; // todo - make setVertexColours take like COLOUR_MODE_COPY or COLOUR_MODE_ZERO - so that setVertexColours doesn't just apply new vertex colour to 1 vertex.

	if (((CRWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*)this;
		pRWSection_Geometry->setVertexColours(vecColours);
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->setPrelightningColour(ssRed, ssGreen, ssBlue, ssAlpha);
	}
}

void				CRWSectionContainer::applyPrelightningColourOffset(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha)
{
	if (((CRWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*)this;
		string strNewVertexColours;
		for (uint32 i = 0, j = pRWSection_Geometry->getVertexCount(); i < j; i++)
		{
			CVector4ui8 vecColour = pRWSection_Geometry->getVertexColours()[i];
			vecColour.m_x += ssRed;
			vecColour.m_y += ssGreen;
			vecColour.m_z += ssBlue;
			vecColour.m_w += ssAlpha;
			pRWSection_Geometry->getVertexColours()[i] = vecColour;
		}
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->applyPrelightningColourOffset(ssRed, ssGreen, ssBlue, ssAlpha);
	}
}

vector<CVector4ui8>		CRWSectionContainer::getDVCColours(void)
{
	vector<CVector4ui8> vecDVCColours;
	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*)pRWSection;
		vecDVCColours = pRWSection_Geometry->getVertexColours();
		break;
	}
	return vecDVCColours;
}
vector<CVector4ui8>		CRWSectionContainer::getNVCColours(void)
{
	vector<CVector4ui8> vecNVCColours;
	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_EXTRA_VERT_COLOUR))
	{
		CRWSection_ExtraVertColour *pRWSection_ExtraVertColour = (CRWSection_ExtraVertColour*)pRWSection;
		vecNVCColours = pRWSection_ExtraVertColour->getVertexColours();
		break;
	}
	return vecNVCColours;
}

void					CRWSectionContainer::setDVCColours(vector<CVector4ui8>& vecDVCColours)
{
	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*)pRWSection;
		pRWSection_Geometry->setVertexColours(vecDVCColours);
	}
}
void					CRWSectionContainer::setNVCColours(vector<CVector4ui8>& vecNVCColours)
{
	for (CRWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vector<CRWSection*> vecNVCSections = pRWSection1->getSectionsByType(RW_SECTION_EXTRA_VERT_COLOUR);
		if (vecNVCSections.size() == 0)
		{
			vector<CRWSection*> vecRWSections_Extension = pRWSection1->getSectionsByType(RW_SECTION_EXTENSION);
			CRWSection_Extension *pRWSection_Extension;
			if (vecRWSections_Extension.size() == 0)
			{
				pRWSection_Extension = (CRWSection_Extension*)pRWSection1->addSection(RW_SECTION_EXTENSION, RW_VERSION_3_4_0_3); // todo - don't use hardcoded RW version
			}
			else
			{
				pRWSection_Extension = (CRWSection_Extension*)vecRWSections_Extension[0];
			}
			CRWSection_ExtraVertColour *pRWSection_ExtraVertColour = (CRWSection_ExtraVertColour*)pRWSection_Extension->addSection(RW_SECTION_EXTRA_VERT_COLOUR, RW_VERSION_3_4_0_3); // todo - don't use hardcoded RW version
			pRWSection_ExtraVertColour->setVertexColours(vecNVCColours);
		}
		else
		{
			for (CRWSection *pRWSection2 : vecNVCSections)
			{
				CRWSection_ExtraVertColour *pRWSection_ExtraVertColour = (CRWSection_ExtraVertColour*)pRWSection2;
				pRWSection_ExtraVertColour->setVertexColours(vecNVCColours);
			}
		}
	}
}

void					CRWSectionContainer::set2dEffects(vector<C2dEffect*>& vec2dEffects)
{
	vector<vector<C2dEffect*>> vec2dEffects2;
	for (CRWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vec2dEffects2.push_back(vec2dEffects);
	}
	set2dEffects(vec2dEffects2);
}
void					CRWSectionContainer::set2dEffects(vector<vector<C2dEffect*>>& vec2dEffects)
{
	if (vec2dEffects.size() == 0)
	{
		return;
	}

	uint32 ui2dEffectsSectionOccurrenceIndex = 0;
	for (CRWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vector<CRWSection*> vecRWSections_2dEffects = pRWSection1->getSectionsByType(RW_SECTION_2D_EFFECT);
		if (vecRWSections_2dEffects.size() == 0)
		{
			vector<CRWSection*> vecRWSections_Extension = pRWSection1->getSectionsByType(RW_SECTION_EXTENSION);
			CRWSection_Extension *pRWSection_Extension;
			if (vecRWSections_Extension.size() == 0)
			{
				pRWSection_Extension = (CRWSection_Extension*)pRWSection1->addSection(RW_SECTION_EXTENSION, RW_VERSION_3_4_0_3); // todo - don't use hardcoded RW version
			}
			else
			{
				pRWSection_Extension = (CRWSection_Extension*)vecRWSections_Extension[0];
			}
			CRWSection_2dEffect *pRWSection_2dEffects = (CRWSection_2dEffect*)pRWSection_Extension->addSection(RW_SECTION_2D_EFFECT, RW_VERSION_3_4_0_3); // todo - don't use hardcoded RW version

			for (auto p2dEffect : (ui2dEffectsSectionOccurrenceIndex < vec2dEffects.size() ? vec2dEffects[ui2dEffectsSectionOccurrenceIndex] : vec2dEffects[0]))
			{
				pRWSection_2dEffects->get2dEffects()->addEntry(p2dEffect);
			}
			ui2dEffectsSectionOccurrenceIndex++;
		}
		else
		{
			for (CRWSection *pRWSection2 : vecRWSections_2dEffects)
			{
				CRWSection_2dEffect *pRWSection_2dEffects = (CRWSection_2dEffect*)pRWSection2;
				pRWSection_2dEffects->removeAllEntries();
				for (auto p2dEffect : (ui2dEffectsSectionOccurrenceIndex < vec2dEffects.size() ? vec2dEffects[ui2dEffectsSectionOccurrenceIndex] : vec2dEffects[0]))
				{
					pRWSection_2dEffects->get2dEffects()->addEntry(p2dEffect);
				}
				ui2dEffectsSectionOccurrenceIndex++;
			}
		}
	}
}

vector<vector<C2dEffect*>>	CRWSectionContainer::get2dEffects(void)
{
	vector<CRWSection_2dEffect*> vecRWSections_2dEffect = (vector<CRWSection_2dEffect*>&) getSectionsByType(RW_SECTION_2D_EFFECT);
	vector<vector<C2dEffect*>> vec2dEffects;
	for (CRWSection_2dEffect* pRWSection_2dEffect : vecRWSections_2dEffect)
	{
		vec2dEffects.push_back(pRWSection_2dEffect->get2dEffects()->getEntries());
	}
	return vec2dEffects;
}