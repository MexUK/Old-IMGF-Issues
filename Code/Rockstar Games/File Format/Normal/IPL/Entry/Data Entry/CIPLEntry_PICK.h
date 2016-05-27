#ifndef CIPLEntry_PICK_H
#define CIPLEntry_PICK_H

#include "Types.h"
#include "CVector3D.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_PICK : public CIPLEntry_Data
{
public:
	CIPLEntry_PICK(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_PICK)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setWeaponId(uint32 uiWeaponId) { m_uiWeaponId = uiWeaponId; }
	uint32						getWeaponId(void) { return m_uiWeaponId; }

	void						setPosition(CVector3D& vecPosition) { m_vecPosition = vecPosition; }
	CVector3D&					getPosition(void) { return m_vecPosition; }

private:
	// GTA SA only
	uint32						m_uiWeaponId;
	CVector3D					m_vecPosition;
};

#endif