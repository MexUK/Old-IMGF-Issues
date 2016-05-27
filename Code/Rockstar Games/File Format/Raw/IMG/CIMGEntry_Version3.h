#ifndef CIMGEntry_Version3_H
#define CIMGEntry_Version3_H

#include "Types.h"
#include "Namespace.h"

#pragma pack(push, 1)
struct RG::CIMGEntry_Version3
{
	uint32			m_uiItemSize;
	uint32			m_uiRageResourceTypeIdentifier;
	uint32			m_uiOffsetInSectors;
	uint32			m_uiSizeInSectors;
	uint32			m_uiFlags;
};
#pragma pack(pop)

#endif