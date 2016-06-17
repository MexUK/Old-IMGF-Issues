#ifndef CDATAnimgrpFormat_H
#define CDATAnimgrpFormat_H

#include "Types.h"
#include "CFormat.h"
#include "CDATAnimgrpSection.h"
#include <vector>

class CDATAnimgrpFormat : public CFormat, public CVectorPool<CDATAnimgrpSection*>
{
public:
	CDATAnimgrpFormat(void);
	
	void					unserialize(void);
	void					serialize(void);
};

#endif