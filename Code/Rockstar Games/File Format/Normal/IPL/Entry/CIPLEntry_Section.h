#ifndef CIPLEntry_Section_H
#define CIPLEntry_Section_H

#include "IPL/CIPLEntry.h"

class CIPLFormat;

class CIPLEntry_Section : public CIPLEntry
{
public:
	CIPLEntry_Section(CIPLFormat *pIPLFormat) :
		CIPLEntry(pIPLFormat)
	{};

	void					unserialize(void);
	void					serialize(void);
};

#endif