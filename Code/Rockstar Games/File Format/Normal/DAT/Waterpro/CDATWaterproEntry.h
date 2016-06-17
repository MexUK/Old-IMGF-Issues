#ifndef CDATWaterproEntry_H
#define CDATWaterproEntry_H

#include "Types.h"
#include "CVector2D.h"

class CDATWaterproEntry
{
public:
	CDATWaterproEntry(void);
	
	void					unload(void) {}

	void					setWaterLevel(float32 fWaterLevel) { m_fWaterLevel = fWaterLevel; }
	float32					getWaterLevel(void) { return m_fWaterLevel; }
	
	void					setWaterZoneStart(CVector2D& vecWaterZoneStart) { m_vecWaterZoneStart = vecWaterZoneStart; }
	CVector2D&				getWaterZoneStart(void) { return m_vecWaterZoneStart; }
	
	void					setWaterZoneEnd(CVector2D& vecWaterZoneEnd) { m_vecWaterZoneEnd = vecWaterZoneEnd; }
	CVector2D&				setWaterZoneEnd(void) { return m_vecWaterZoneEnd; }
	
private:
	float32					m_fWaterLevel;
	CVector2D				m_vecWaterZoneStart;
	CVector2D				m_vecWaterZoneEnd;
};

#endif