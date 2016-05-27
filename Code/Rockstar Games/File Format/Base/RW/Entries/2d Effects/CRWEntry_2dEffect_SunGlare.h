#ifndef CRWEntry_2dEffect_SunGlare_H
#define CRWEntry_2dEffect_SunGlare_H

#include "Types.h"
#include "RW/Entries/C2dEffect.h"
#include <string>

class CRWEntry_2dEffect_SunGlare : public C2dEffect
{
public:
	CRWEntry_2dEffect_SunGlare(void);

	void							unserialize(void);
	void							serialize(void);
};

#endif