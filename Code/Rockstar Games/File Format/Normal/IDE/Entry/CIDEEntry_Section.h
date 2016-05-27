#ifndef CIDEEntry_Section_H
#define CIDEEntry_Section_H

#include "IDE/CIDEEntry.h"

class CIDEFormat;

class CIDEEntry_Section : public CIDEEntry
{
public:
	CIDEEntry_Section(CIDEFormat *pIDEFormat) :
		CIDEEntry(pIDEFormat)
	{};

	void					unserialize(void);
	void					serialize(void);
};

#endif