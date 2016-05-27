#ifndef CIPLEntry_GRGE_H
#define CIPLEntry_GRGE_H

#include "Types.h"
#include "IPL/Entry/CIPLEntry_Data.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include <string>

class CIPLEntry_GRGE : public CIPLEntry_Data
{
public:
	CIPLEntry_GRGE(CIPLFormat *pIPLFormat) :
		CIPLEntry_Data(pIPLFormat, IPL_SECTION_GRGE),
		m_vecPosition{ 0.0f, 0.0f, 0.0f },
		m_vecLine{ 0.0f, 0.0f },
		m_vecCubePosition{ 0.0f, 0.0f, 0.0f },
		m_uiGarageFlags(0),
		m_uiGarageType(0)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setPosition(CVector3D& vecPosition) { m_vecPosition = vecPosition; }
	CVector3D&					getPosition(void) { return m_vecPosition; }

	void						setLine(CVector2D& vecLine) { m_vecLine = vecLine; }
	CVector2D&					getLine(void) { return m_vecLine; }

	void						setCubePosition(CVector3D& vecCubePosition) { m_vecCubePosition = vecCubePosition; }
	CVector3D&					getCubePosition(void) { return m_vecCubePosition; }

	void						setGarageFlags(uint32 uiGarageFlags) { m_uiGarageFlags = uiGarageFlags; }
	uint32						getGarageFlags(void) { return m_uiGarageFlags; }

	void						setGarageType(uint32 uiGarageType) { m_uiGarageType = uiGarageType; }
	uint32						getGarageType(void) { return m_uiGarageType; }

	void						setGarageName(std::string& strGarageName) { m_strGarageName = strGarageName; }
	std::string&				getGarageName(void) { return m_strGarageName; }

private:
	// GTA SA only
	CVector3D					m_vecPosition;
	CVector2D					m_vecLine;
	CVector3D					m_vecCubePosition;
	uint32						m_uiGarageFlags;
	uint32						m_uiGarageType;
	std::string					m_strGarageName;
};

#endif