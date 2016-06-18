#ifndef CDATChaseEntry_H
#define CDATChaseEntry_H

#include "Types.h"
#include "CVector3i8.h"
#include "CVector3i16.h"
#include "CVector3D.h"

class CDATChaseFormat;

class CDATChaseEntry
{
public:
	CDATChaseEntry(void);
	
	void					unload(void) {}

	void					serialize(void);
	
	void					setFormat(CDATChaseFormat* pFormat) { m_pFormat = pFormat; }
	CDATChaseFormat*		getFormat(void) { return m_pFormat; }
	
	void					setVelocity(CVector3i16& vecVelocity) { m_vecVelocity = vecVelocity; }
	CVector3i16&			getVelocity(void) { return m_vecVelocity; }

	void					setRight(CVector3i8& vecRight) { m_vecRight = vecRight; }
	CVector3i8&				getRight(void) { return m_vecRight; }

	void					setTop(CVector3i8& vecTop) { m_vecTop = vecTop; }
	CVector3i8&				getTop(void) { return m_vecTop; }

	void					setSteeringAngle(int8 iSteeringAngle) { m_iSteeringAngle = iSteeringAngle; }
	int8					getSteeringAngle(void) { return m_iSteeringAngle; }

	void					setGasPedalPower(int8 iGasPedalPower) { m_iGasPedalPower = iGasPedalPower; }
	int8					getGasPedalPower(void) { return m_iGasPedalPower; }

	void					setBrakePedalPower(int8 iBrakePedalPower) { m_iBrakePedalPower = iBrakePedalPower; }
	int8					getBrakePedalPower(void) { return m_iBrakePedalPower; }

	void					setHandbrakeUsed(int8 iHandbrakeUsed) { m_iHandbrakeUsed = iHandbrakeUsed; }
	int8					getHandbrakeUsed(void) { return m_iHandbrakeUsed; }

	void					setPosition(CVector3D& vecPosition) { m_vecPosition = vecPosition; }
	CVector3D&				getPosition(void) { return m_vecPosition; }
	
private:
	CDATChaseFormat*		m_pFormat;
	CVector3i16				m_vecVelocity;
	CVector3i8				m_vecRight;
	CVector3i8				m_vecTop;
	int8					m_iSteeringAngle;
	int8					m_iGasPedalPower;
	int8					m_iBrakePedalPower;
	int8					m_iHandbrakeUsed;
	CVector3D				m_vecPosition;
};

#endif