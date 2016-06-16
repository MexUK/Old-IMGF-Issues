#include "CRWSection.h"
#include "Engine/RW/CRWManager.h"
#include "Data Stream/CDataWriter.h"
#include "Sections/CRWSection_2dEffect.h"
#include "Sections/CRWSection_Atomic.h"
#include "Sections/CRWSection_BinMeshPLG.h"
#include "Sections/CRWSection_Clump.h"
#include "Sections/CRWSection_Extension.h"
#include "Sections/CRWSection_ExtraVertColour.h"
#include "Sections/CRWSection_Frame.h"
#include "Sections/CRWSection_FrameList.h"
#include "Sections/CRWSection_Geometry.h"
#include "Sections/CRWSection_GeometryList.h"
#include "Sections/CRWSection_Light.h"
#include "Sections/CRWSection_Material.h"
#include "Sections/CRWSection_MaterialList.h"
#include "Sections/CRWSection_String.h"
#include "Sections/CRWSection_Texture.h"
#include "Sections/CRWSection_TextureDictionary.h"
#include "Sections/CRWSection_TextureNative.h"
#include "Sections/CRWSection_UnknownSection.h"
#include "String/CStringUtility.h"

using namespace std;

unordered_map<eRWSection, bool>		CRWSection::m_umapRWSectionsContainingStruct;

CRWSection::CRWSection(void) :
	m_eSectionId(RW_SECTION_UNKNOWN),
	m_uiSectionSize(0),
	m_uiSectionRWVersion(0),
	m_bSectionHeaderSkipped(false)
{
}

void				CRWSection::initStatic(void)
{
	initRWSections();
}

void				CRWSection::initRWSections(void)
{
	vector<eRWSection> vecRWSectionIds = getRWSections();
	for (eRWSection eRWSectionValue : vecRWSectionIds)
	{
		m_umapRWSectionsContainingStruct[eRWSectionValue] = doesRWSectionContainStruct_BeforeInit(eRWSectionValue);
	}
}

void				CRWSection::serialize(void)
{
	if (m_eSectionId == RW_SECTION_UNKNOWN)
	{
		return;
	}
	
	CDataWriter *pDataWriter = CDataWriter::getInstance();
	uint32 uiVersionCC = CRWManager::getInstance()->getRWVersionCCForSerialization();
	
	if (!isSectionHeaderSkipped())
	{
		pDataWriter->writeUint32(m_eSectionId);
		pDataWriter->writeUint32(0); // placeholder
		pDataWriter->writeUint32(uiVersionCC);
	}
	
	if (doesRWSectionContainStruct(m_eSectionId))
	{
		pDataWriter->writeUint32(RW_SECTION_STRUCT);
		pDataWriter->writeUint32(0); // placeholder
		pDataWriter->writeUint32(uiVersionCC);
	}
}

CRWSection*			CRWSection::addSection(eRWSection eRWSectionValue, eRWVersion eRWVersionValue)
{
	CRWSection *pRWSection = CRWSectionContainer::addSection(eRWSectionValue, eRWVersionValue);
	pRWSection->setParentNode(this);
	return pRWSection;
}

void				CRWSection::removeSection(void)
{
	uint32 uiSectionIndex = getSectionIndex();
	if (uiSectionIndex == -1)
	{
		return;
	}
	getParentNode()->removeSectionByIndex(uiSectionIndex);
}

uint32				CRWSection::getSectionIndex(void)
{
	uint32 i = 0;
	for (CRWSection *pRWSection : getParentNode()->getEntries())
	{
		if (pRWSection == this)
		{
			return i;
		}
		i++;
	}
	return -1;
}

void				CRWSection::fillPlaceholdersForSerialization(void)
{
	CDataWriter *pDataWriter = CDataWriter::getInstance();

	string
		&strDataIn = pDataWriter->getData(),
		strDataOut = "";

	if (!isSectionHeaderSkipped())
	{
		uint32 uiSectionLength = strDataIn.length() - 12;
		strDataOut = strDataIn.substr(0, 4) + CStringUtility::packUint32(uiSectionLength, false) + strDataIn.substr(8);
	}

	if (doesRWSectionContainStruct(m_eSectionId))
	{
		uint32 uiBodyDataLength = strDataIn.length() - 24;
		strDataOut = strDataIn.substr(0, 16) + CStringUtility::packUint32(uiBodyDataLength, false) + strDataIn.substr(20);
	}

	pDataWriter->setData(strDataOut);
}

CRWSection*			CRWSection::createRWSection(eRWSection eRWSectionValue)
{
	switch (eRWSectionValue)
	{
	case RW_SECTION_2D_EFFECT:			return new CRWSection_2dEffect;
	case RW_SECTION_ATOMIC:				return new CRWSection_Atomic;
	case RW_SECTION_BIN_MESH_PLG:		return new CRWSection_BinMeshPLG;
	case RW_SECTION_CLUMP:				return new CRWSection_Clump;
	case RW_SECTION_EXTENSION:			return new CRWSection_Extension;
	case RW_SECTION_EXTRA_VERT_COLOUR:	return new CRWSection_ExtraVertColour;
	case RW_SECTION_FRAME:				return new CRWSection_Frame;
	case RW_SECTION_FRAME_LIST:			return new CRWSection_FrameList;
	case RW_SECTION_GEOMETRY:			return new CRWSection_Geometry;
	case RW_SECTION_GEOMETRY_LIST:		return new CRWSection_GeometryList;
	case RW_SECTION_LIGHT:				return new CRWSection_Light;
	case RW_SECTION_MATERIAL:			return new CRWSection_Material;
	case RW_SECTION_MATERIAL_LIST:		return new CRWSection_MaterialList;
	case RW_SECTION_STRING:				return new CRWSection_String;
	case RW_SECTION_TEXTURE:			return new CRWSection_Texture;
	case RW_SECTION_TEXTURE_DICTIONARY:	return new CRWSection_TextureDictionary;
	case RW_SECTION_TEXTURE_NATIVE:		return new CRWSection_TextureNative;
	case RW_SECTION_UNKNOWN:			return new CRWSection_UnknownSection;
	}
	return nullptr;
}

bool				CRWSection::doesRWSectionContainStruct_BeforeInit(eRWSection eRWSectionValue)
{
	return eRWSectionValue == RW_SECTION_ATOMIC
		|| eRWSectionValue == RW_SECTION_CLUMP
		|| eRWSectionValue == RW_SECTION_GEOMETRY
		|| eRWSectionValue == RW_SECTION_GEOMETRY_LIST
		|| eRWSectionValue == RW_SECTION_FRAME_LIST
		|| eRWSectionValue == RW_SECTION_MATERIAL
		|| eRWSectionValue == RW_SECTION_MATERIAL_LIST
		|| eRWSectionValue == RW_SECTION_TEXTURE
		|| eRWSectionValue == RW_SECTION_TEXTURE_DICTIONARY
		|| eRWSectionValue == RW_SECTION_TEXTURE_NATIVE;
}
bool				CRWSection::doesRWSectionContainStruct(eRWSection eRWSectionValue)
{
	return m_umapRWSectionsContainingStruct.count(eRWSectionValue) == 1 && m_umapRWSectionsContainingStruct[eRWSectionValue] == true;
}

vector<eRWSection>	CRWSection::getRWSections(void)
{
	return {
		RW_SECTION_STRUCT,
		RW_SECTION_STRING,
		RW_SECTION_EXTENSION,
		RW_SECTION_TEXTURE,
		RW_SECTION_MATERIAL,
		RW_SECTION_MATERIAL_LIST,
		RW_SECTION_FRAME_LIST,
		RW_SECTION_GEOMETRY,
		RW_SECTION_CLUMP,
		RW_SECTION_LIGHT,
		RW_SECTION_ATOMIC,
		RW_SECTION_TEXTURE_NATIVE,
		RW_SECTION_TEXTURE_DICTIONARY,
		RW_SECTION_GEOMETRY_LIST,
		RW_SECTION_BIN_MESH_PLG,
		RW_SECTION_2D_EFFECT,
		RW_SECTION_EXTRA_VERT_COLOUR,
		RW_SECTION_FRAME
	};
}