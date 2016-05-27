#ifndef CRWEntry_2dEffect_Escalator_H
#define CRWEntry_2dEffect_Escalator_H

#include "Types.h"
#include "RW/Entries/C2dEffect.h"
#include "CVector3D.h"
#include <string>

class CRWEntry_2dEffect_Escalator : public C2dEffect
{
public:
	CRWEntry_2dEffect_Escalator(void);

	void							unserialize(void);
	void							serialize(void);

	void							setEscalatorBottom(CVector3D& vecEscalatorBottom) { m_vecEscalatorBottom = vecEscalatorBottom; }
	CVector3D&						getEscalatorBottom(void) { return m_vecEscalatorBottom; }

	void							setEscalatorTop(CVector3D& vecEscalatorTop) { m_vecEscalatorTop = vecEscalatorTop; }
	CVector3D&						getEscalatorTop(void) { return m_vecEscalatorTop; }

	void							setEscalatorEnd(CVector3D& vecEscalatorEnd) { m_vecEscalatorEnd = vecEscalatorEnd; }
	CVector3D&						getEscalatorEnd(void) { return m_vecEscalatorEnd; }

	void							setDirection(uint32 uiDirection) { m_uiDirection = uiDirection; }
	uint32							getDirection(void) { return m_uiDirection; }

private:
	CVector3D						m_vecEscalatorBottom;
	CVector3D						m_vecEscalatorTop;
	CVector3D						m_vecEscalatorEnd; // (Z pos, matches top Z if escalator goes up, bottom Z if it goes down)
	uint32							m_uiDirection;
};

#endif