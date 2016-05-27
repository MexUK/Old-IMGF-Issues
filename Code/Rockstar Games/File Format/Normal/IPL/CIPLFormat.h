#ifndef CIPLFormat_H
#define CIPLFormat_H

#include "CFormat.h"
#include "Section Lines/CSectionLinesFormat.h"
#include "eDataType.h" // todo - no idea why this include is needed - it makes eIPLSection underlined when not here
#include "eIPLSection.h"
#include "eIPLPathType.h"
#include "Entry/CIPLEntry_Other.h"
#include "Entry/CIPLEntry_Section.h"
#include "Entry/CIPLEntry_Data.h"
#include <string>
#include <vector>

class CIPLFormat;
class CIPLEntry;

class CIPLFormat : public CSectionLinesFormat<CIPLFormat, CIPLEntry, eIPLSection, CIPLEntry_Other, CIPLEntry_Section, CIPLEntry_Data>
{
public:
	CIPLFormat(void);

	void												setIsBinary(bool bState) { m_bIsBinary = bState; }
	bool												isBinary(void) { return m_bIsBinary; }

	CIPLEntry_Data*										createDataEntry(eIPLSection eIDESectionValue, uint32 uiSectionSpecificType = 0);

	eIPLSection											getSectionFromText(std::string strIPLSectionText);
	std::string											getSectionText(eIPLSection eIPLSectionValue);

	uint32												detectSectionSpecificType(eIPLSection eIPLSectionValue);

private:
	void												unserialize(void);
	void												serialize(void);

	void												unserializeBinary(void);
	void												serializeBinary(void);

	CIPLEntry_Data*										unserializeDataEntry(eIPLSection eIPLSectionValue);

	eIPLPathType										detectPATHType(void);

private:
	uint8												m_bIsBinary : 1;
};

#endif