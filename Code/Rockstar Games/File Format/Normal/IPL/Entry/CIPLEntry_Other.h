#ifndef CIPLEntry_Other_H
#define CIPLEntry_Other_H

#include "IPL/CIPLEntry.h"
#include "Section Lines/Entry/CSectionLinesEntry_Other.h"

class CIPLFormat;

class CIPLEntry_Other : public CSectionLinesEntry_Other, public CIPLEntry
{
public:
	CIPLEntry_Other(CIPLFormat *pIPLFormat) :
		CIPLEntry(pIPLFormat)
	{};

	void							unserialize(void);
	void							serialize(void);
};

#endif