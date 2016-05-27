#ifndef CIDEEntry_2DFX_SpawnPoint_H
#define CIDEEntry_2DFX_SpawnPoint_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include "CVector4ui32.h"
#include <string>

class CIDEEntry_2DFX_SpawnPoint: public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_SpawnPoint(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_SPAWN_POINT)
	{
		m_vecFlags.m_x = 0;
		m_vecFlags.m_y = 0;
		m_vecFlags.m_z = 0;
		m_vecFlags.m_w = 0;
	};

	void						unserialize(void);
	void						serialize(void);

	void						setFlags(CVector4ui32& vecFlags) { m_vecFlags = vecFlags; }
	CVector4ui32&				getFlags(void) { return m_vecFlags; }

private:
	// GTA IV only
	CVector4ui32				m_vecFlags;
};

#endif