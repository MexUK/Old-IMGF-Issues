#ifndef CRWSection_Extension_H
#define CRWSection_Extension_H

#include "../CRWSection.h"
#include <string>

class CRWSection_Extension : public CRWSection
{
public:
	CRWSection_Extension(void) { setSectionId(RW_SECTION_EXTENSION); }

	void							unserialize(void);
	void							serialize(void);
};

#endif