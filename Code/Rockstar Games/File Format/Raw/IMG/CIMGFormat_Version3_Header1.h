#ifndef CIMGFormat_Version3_Header1_H
#define CIMGFormat_Version3_Header1_H

#include "Types.h"
#include "Namespace.h"

#pragma pack(push, 1)
struct RG::CIMGFormat_Version3_Header1
{
	uint32			m_uiIdentifier;
	uint32			m_uiVersion;
	uint32			m_uiEntryCount;
	uint32			m_uiTableSize;
	uint16			m_usTableItemsSize;
	uint16			m_usUnknown;
};
#pragma pack(pop)

#endif