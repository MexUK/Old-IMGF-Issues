#ifndef CIPLEntry_CULL_H
#define CIPLEntry_CULL_H

#include "Types.h"
#include "CVector3D.h"
#include "CVector4D.h"
#include "IPL/Entry/CIPLEntry_Data.h"

class CIPLEntry_CULL : public CIPLEntry_Data
{
public:
	CIPLEntry_CULL(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_CULL),
		m_vecCenterPosition{ 0.0f, 0.0f, 0.0f },
		m_vecLowerLeftPosition{ 0.0f, 0.0f, 0.0f },
		m_vecUpperRightPosition{ 0.0f, 0.0f, 0.0f },
		m_uiFlags(0),
		m_uiUnknown1(0),
		m_uiUnknown2(0),
		m_uiUnknown3(0),
		m_uiLength(0),
		m_uiWidth(0),
		m_uiTop(0),
		m_uiBottom(0),
		m_vecMirrorParameters{ 0.0f, 0.0f, 0.0f, 0.0f }
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setFlags(uint32 uiFlags) { m_uiFlags = uiFlags; }
	uint32						getFlags(void) { return m_uiFlags; }

	void						setUnknown1(uint32 uiUnknown1) { m_uiUnknown1 = uiUnknown1; }
	uint32						getUnknown1(void) { return m_uiUnknown1; }

	void						setCenterPosition(CVector3D& vecCenterPosition) { m_vecCenterPosition = vecCenterPosition; }
	CVector3D&					getCenterPosition(void) { return m_vecCenterPosition; }

	void						setLowerLeftPosition(CVector3D& vecLowerLeftPosition) { m_vecLowerLeftPosition = vecLowerLeftPosition; }
	CVector3D&					getLowerLeftPosition(void) { return m_vecLowerLeftPosition; }

	void						setUpperRightPosition(CVector3D& vecUpperRightPosition) { m_vecUpperRightPosition = vecUpperRightPosition; }
	CVector3D&					getUpperRightPosition(void) { return m_vecUpperRightPosition; }

	void						setUnknown2(uint32 uiUnknown2) { m_uiUnknown2 = uiUnknown2; }
	uint32						getUnknown2(void) { return m_uiUnknown2; }

	void						setUnknown3(uint32 uiUnknown3) { m_uiUnknown3 = uiUnknown3; }
	uint32						getUnknown3(void) { return m_uiUnknown3; }

	void						setLength(uint32 uiLength) { m_uiLength = uiLength; }
	uint32						getLength(void) { return m_uiLength; }

	void						setWidth(uint32 uiWidth) { m_uiWidth = uiWidth; }
	uint32						getWidth(void) { return m_uiWidth; }

	void						setTop(uint32 uiTop) { m_uiTop = uiTop; }
	uint32						getTop(void) { return m_uiTop; }

	void						setBottom(uint32 uiBottom) { m_uiBottom = uiBottom; }
	uint32						getBottom(void) { return m_uiBottom; }

	void						setMirrorParameters(CVector4D& vecMirrorParameters) { m_vecMirrorParameters = vecMirrorParameters; }
	CVector4D&					getMirrorParameters(void) { return m_vecMirrorParameters; }

private:
	// GTA III era & GTA IV
	uint32						m_uiFlags;
	uint32						m_uiUnknown1;

	// GTA III era
	CVector3D					m_vecCenterPosition;
	CVector3D					m_vecLowerLeftPosition;
	CVector3D					m_vecUpperRightPosition;

	// GTA SA & GTA IV
	uint32						m_uiUnknown2;
	uint32						m_uiUnknown3;
	uint32						m_uiLength;
	uint32						m_uiWidth;
	uint32						m_uiTop;
	uint32						m_uiBottom;
	CVector4D					m_vecMirrorParameters;
};

#endif