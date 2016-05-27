#ifndef CRWSection_2dEffect_H
#define CRWSection_2dEffect_H

#include "../CRWSection.h"
#include "Pool/CVectorPool.h"
#include "RW/Pools/C2dEffectsPool.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include <string>

class CRWSection_2dEffect : public CRWSection
{
public:
	CRWSection_2dEffect(void);
	~CRWSection_2dEffect(void);

	void							unserialize(void);
	void							serialize(void);

	C2dEffectsPool*					get2dEffects(void) { return m_p2dEffectsPool; }

private:
	C2dEffectsPool*					m_p2dEffectsPool;
};

#endif