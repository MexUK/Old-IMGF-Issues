#ifndef CIDEEntry_2DFX_Ladder_H
#define CIDEEntry_2DFX_Ladder_H

#include "IDE/Entry/Data Entry/CIDEEntry_2DFX.h"
#include "CVector3D.h"
#include <string>

class CIDEEntry_2DFX_Ladder : public CIDEEntry_2DFX
{
public:
	CIDEEntry_2DFX_Ladder(CIDEFormat *pIDEFormat) :
		CIDEEntry_2DFX(pIDEFormat, _2DFX_LADDER),
		m_vecLadderStartPosition{ 0.0f, 0.0f, 0.0f },
		m_vecLadderEndPosition{ 0.0f, 0.0f, 0.0f },
		m_vecClimbDirectionPosition{ 0.0f, 0.0f, 0.0f },
		m_uiFlags(0)
	{
	};

	void						unserialize(void);
	void						serialize(void);

	void						setLadderStartPosition(CVector3D& vecLadderStartPosition) { m_vecLadderStartPosition = vecLadderStartPosition; }
	CVector3D&					getLadderStartPosition(void) { return m_vecLadderStartPosition; }

	void						setLadderEndPosition(CVector3D& vecLadderEndPosition) { m_vecLadderEndPosition = vecLadderEndPosition; }
	CVector3D&					getLadderEndPosition(void) { return m_vecLadderEndPosition; }

	void						setClimbDirectionPosition(CVector3D& vecClimbDirectionPosition) { m_vecClimbDirectionPosition = vecClimbDirectionPosition; }
	CVector3D&					getClimbDirectionPosition(void) { return m_vecClimbDirectionPosition; }

	void						setFlags(uint32 uiFlags) { m_uiFlags = uiFlags; }
	uint32						getFlags(void) { return m_uiFlags; }

private:
	// GTA IV only
	CVector3D					m_vecLadderStartPosition;
	CVector3D					m_vecLadderEndPosition;
	CVector3D					m_vecClimbDirectionPosition;
	uint32						m_uiFlags;
};

#endif