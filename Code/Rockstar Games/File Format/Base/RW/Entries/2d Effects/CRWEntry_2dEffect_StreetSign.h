#ifndef CRWEntry_2dEffect_StreetSign_H
#define CRWEntry_2dEffect_StreetSign_H

#include "Types.h"
#include "RW/Entries/C2dEffect.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include <string>

class CRWEntry_2dEffect_StreetSign : public C2dEffect
{
public:
	CRWEntry_2dEffect_StreetSign(void);

	void							unserialize(void);
	void							serialize(void);

	void							setSize(CVector2D& vecSize) { m_vecSize = vecSize; }
	CVector2D&						getSize(void) { return m_vecSize; }

	void							setRotation(CVector3D& vecRotation) { m_vecRotation = vecRotation; }
	CVector3D&						getRotation(void) { return m_vecRotation; }

	void							setFlags(int16 uiFlags) { m_uiFlags = uiFlags; }
	int16							getFlags(void) { return m_uiFlags; }

	void							setText(uint8 uiStringIndex, std::string& strData) { m_strText[uiStringIndex] = strData; }
	std::string&					getText(uint8 uiStringIndex) { return m_strText[uiStringIndex]; }

private:
	CVector2D						m_vecSize;
	CVector3D						m_vecRotation;
	uint16							m_uiFlags;
	std::string						m_strText[4];
};

#endif