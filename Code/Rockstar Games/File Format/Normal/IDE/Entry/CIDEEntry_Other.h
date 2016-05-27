#ifndef CIDEEntry_Other_H
#define CIDEEntry_Other_H

#include "IDE/CIDEEntry.h"
#include "Section Lines/Entry/CSectionLinesEntry_Other.h"

class CIDEFormat;

class CIDEEntry_Other : public CSectionLinesEntry_Other, public CIDEEntry
{
public:
	CIDEEntry_Other(CIDEFormat *pIDEFormat) :
		CIDEEntry(pIDEFormat)
	{};

	void							unserialize(void);
	void							serialize(void);
};

#endif