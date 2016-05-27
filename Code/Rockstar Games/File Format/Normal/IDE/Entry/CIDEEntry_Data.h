#ifndef CIDEEntry_Data_H
#define CIDEEntry_Data_H

#include "IDE/CIDEEntry.h"
#include "Section Lines/Entry/CSectionLinesEntry_Data.h"

class CIDEEntry_Data : public CSectionLinesEntry_Data, public CIDEEntry
{
public:
	CIDEEntry_Data(CIDEFormat *pIDEFormat, eIDESection eIDESectionValue) :
		CIDEEntry(pIDEFormat)
	{
		setSectionType(eIDESectionValue);
	};
};

#endif