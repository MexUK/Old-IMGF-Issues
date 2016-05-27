#ifndef CIPLEntry_AUZO_H
#define CIPLEntry_AUZO_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"
#include "CVector3D.h"
#include <string>

class CIPLEntry_AUZO : public CIPLEntry_Data
{
public:
	CIPLEntry_AUZO(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_AUZO),
		m_uiSoundId(0),
		m_uiSwitch(0),
		m_vecLowerLeftPosition{ 0.0f, 0.0f, 0.0f },
		m_vecUpperRightPosition{ 0.0f, 0.0f, 0.0f },
		m_vecCenterPosition{ 0.0f, 0.0f, 0.0f },
		m_fRadius(0.0f)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setZoneName(std::string& strZoneName) { m_strZoneName = strZoneName; }
	std::string&				getZoneName(void) { return m_strZoneName; }

	void						setSoundId(uint32 uiSoundID) { m_uiSoundId = uiSoundID; }
	uint32						getSoundId(void) { return m_uiSoundId; }

	void						setLowerLeftPosition(CVector3D& vecLowerLeftPosition) { m_vecLowerLeftPosition = vecLowerLeftPosition; }
	CVector3D&					getLowerLeftPosition(void) { return m_vecLowerLeftPosition; }

	void						setUpperRightPosition(CVector3D& vecUpperRightPosition) { m_vecUpperRightPosition = vecUpperRightPosition; }
	CVector3D&					getUpperRightPosition(void) { return m_vecUpperRightPosition; }

	void						setCenterPosition(CVector3D& vecCenterPosition) { m_vecCenterPosition = vecCenterPosition; }
	CVector3D&					getCenterPosition(void) { return m_vecCenterPosition; }

	void						setRadius(float32 fRadius) { m_fRadius = fRadius; }
	float32						getRadius(void) { return m_fRadius; }

private:
	// GTA SA only
	std::string					m_strZoneName;
	uint32						m_uiSoundId;
	uint32						m_uiSwitch;
	CVector3D					m_vecLowerLeftPosition;
	CVector3D					m_vecUpperRightPosition;
	CVector3D					m_vecCenterPosition;
	float32						m_fRadius;
};

#endif