#ifndef CIntermediateVertexData_H
#define CIntermediateVertexData_H

#include "Types.h"
#include "CVector2D.h"
#include "CVector3D.h"
#include "CVector4D.h"

class CIntermediateVertexData
{
public:
	CIntermediateVertexData(void) :
		m_uiColour(0)
	{
		m_vecPosition.m_x = 0.0f;
		m_vecPosition.m_y = 0.0f;
		m_vecPosition.m_z = 0.0f;
		m_vecNormal.m_x = 0.0f;
		m_vecNormal.m_y = 0.0f;
		m_vecNormal.m_z = 0.0f;
		m_vecTextureCoordinates.m_x = 0.0f;
		m_vecTextureCoordinates.m_y = 0.0f;
		m_vecNormal2.m_x = 0.0f;
		m_vecNormal2.m_y = 0.0f;
		m_vecNormal2.m_z = 0.0f;
		m_vecNormal2.m_w = 0.0f;
	};

	void								setPosition(CVector3D& vecPosition) { m_vecPosition = vecPosition; }
	CVector3D&							getPosition(void) { return m_vecPosition; }

	void								setNormal(CVector3D& vecNormal) { m_vecNormal = vecNormal; }
	CVector3D&							getNormal(void) { return m_vecNormal; }

	void								setColour(uint32 uiColour) { m_uiColour = uiColour; }
	uint32								getColour(void) { return m_uiColour; }

	void								setTextureCoordinates(CVector2D& vecTextureCoordinates) { m_vecTextureCoordinates = vecTextureCoordinates; }
	CVector2D&							getTextureCoordinates(void) { return m_vecTextureCoordinates; }

	void								setNormal2(CVector4D& vecNormal2) { m_vecNormal2 = vecNormal2; }
	CVector4D&							getNormal2(void) { return m_vecNormal2; }

private:
	CVector3D							m_vecPosition;
	CVector3D							m_vecNormal;
	uint32								m_uiColour; // RGBA
	CVector2D							m_vecTextureCoordinates;
	CVector4D							m_vecNormal2;
};

#endif