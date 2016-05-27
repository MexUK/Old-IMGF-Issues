#ifndef CIDEEntry_H
#define CIDEEntry_H

#include "Types.h"
#include "eIDESection.h"
#include "Section Lines/CSectionLinesEntry.h"
#include <string>
#include <vector>

class CIDEFormat;

class CIDEEntry : public CSectionLinesEntry<CIDEFormat, eIDESection>
{
public:
	CIDEEntry(CIDEFormat *pIDEFormat);
};

#endif