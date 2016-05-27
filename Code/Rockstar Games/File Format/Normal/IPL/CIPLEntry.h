#ifndef CIPLEntry_H
#define CIPLEntry_H

#include "Types.h"
#include "eIPLSection.h"
#include "Section Lines/CSectionLinesEntry.h"
#include <string>
#include <vector>

class CIPLFormat;

class CIPLEntry : public CSectionLinesEntry<CIPLFormat, eIPLSection>
{
public:
	CIPLEntry(CIPLFormat *pIPLFormat);
};

#endif