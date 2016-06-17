#ifndef CDATChaseEntry_Raw_H
#define CDATChaseEntry_Raw_H

#include "Types.h"
#include "CVector3i8.h"
#include "CVector3i16.h"
#include "CVector3D.h"

#pragma pack(push, 1)
struct CDATChaseEntry_Raw
{
	CVector3i16				m_vecVelocity;
	CVector3i8				m_vecRight;
	CVector3i8				m_vecTop;
	int8					m_iSteeringAngle;
	int8					m_iGasPedalPower;
	int8					m_iBrakePedalPower;
	int8					m_iHandbrakeUsed;
	CVector3D				m_vecPosition;
};
#pragma pack(pop)

#endif