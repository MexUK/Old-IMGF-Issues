#ifndef CIPLEntry_Data_H
#define CIPLEntry_Data_H

#include "IPL/CIPLEntry.h"
#include "Section Lines/Entry/CSectionLinesEntry_Data.h"

class CIPLEntry_Data : public CSectionLinesEntry_Data, public CIPLEntry
{
public:
	CIPLEntry_Data(CIPLFormat *pIPLFormat, eIPLSection eIPLSectionValue) :
		CIPLEntry(pIPLFormat)
	{
		setSectionType(eIPLSectionValue);
	};
};

#endif