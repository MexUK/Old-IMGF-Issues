#ifndef CRWEntry_2dEffect_CoverPoint_H
#define CRWEntry_2dEffect_CoverPoint_H

#include "Types.h"
#include "RW/Entries/C2dEffect.h"
#include "CVector2D.h"
#include <string>

class CRWEntry_2dEffect_CoverPoint : public C2dEffect
{
public:
	CRWEntry_2dEffect_CoverPoint(void);

	void							unserialize(void);
	void							serialize(void);

	void							setDirection(CVector2D& vecDirection) { m_vecDirection = vecDirection; }
	CVector2D&						getDirection(void) { return m_vecDirection; }

	void							setCoverType(uint32 uiCoverType) { m_uiCoverType = uiCoverType; }
	uint32							getCoverType(void) { return m_uiCoverType; }

private:
	CVector2D						m_vecDirection;
	uint32							m_uiCoverType;
};

#endif