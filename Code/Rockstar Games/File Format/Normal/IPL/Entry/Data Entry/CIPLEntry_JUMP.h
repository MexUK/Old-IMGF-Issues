#ifndef CIPLEntry_JUMP_H
#define CIPLEntry_JUMP_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"
#include "CVector3D.h"

class CIPLEntry_JUMP : public CIPLEntry_Data
{
public:
	CIPLEntry_JUMP(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_JUMP),
		m_vecStartLowerPosition{ 0.0f, 0.0f, 0.0f },
		m_vecStartUpperPosition{ 0.0f, 0.0f, 0.0f },
		m_vecTargetLowerPosition{ 0.0f, 0.0f, 0.0f },
		m_vecTargetUpperPosition{ 0.0f, 0.0f, 0.0f },
		m_vecCameraPosition{ 0.0f, 0.0f, 0.0f },
		m_uiReward(0)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setStartLowerPosition(CVector3D& vecStartLowerPosition) { m_vecStartLowerPosition = vecStartLowerPosition; }
	CVector3D&					getStartLowerPosition(void) { return m_vecStartLowerPosition; }

	void						setStartUpperPosition(CVector3D& vecStartUpperPosition) { m_vecStartUpperPosition = vecStartUpperPosition; }
	CVector3D&					getStartUpperPosition(void) { return m_vecStartUpperPosition; }

	void						setTargetLowerPosition(CVector3D& vecTargetLowerPosition) { m_vecTargetLowerPosition = vecTargetLowerPosition; }
	CVector3D&					getTargetLowerPosition(void) { return m_vecTargetLowerPosition; }

	void						setTargetUpperPosition(CVector3D& vecTargetUpperPosition) { m_vecTargetUpperPosition = vecTargetUpperPosition; }
	CVector3D&					getTargetUpperPosition(void) { return m_vecTargetUpperPosition; }

	void						setCameraPosition(CVector3D& vecCameraPosition) { m_vecCameraPosition = vecCameraPosition; }
	CVector3D&					getCameraPosition(void) { return m_vecCameraPosition; }

	void						setReward(uint32 uiReward) { m_uiReward = uiReward; }
	uint32						getReward(void) { return m_uiReward; }

private:
	// GTA SA only
	CVector3D					m_vecStartLowerPosition;
	CVector3D					m_vecStartUpperPosition;
	CVector3D					m_vecTargetLowerPosition;
	CVector3D					m_vecTargetUpperPosition;
	CVector3D					m_vecCameraPosition;
	uint32						m_uiReward;
};

#endif