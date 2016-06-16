#ifndef CWDRBlock_VertexData_H
#define CWDRBlock_VertexData_H

#include "CVector2D.h"
#include "CVector3D.h"

class CWDRBlock_VertexData
{
public:
	CVector3D					m_vecPosition;
	CVector3D					m_vecNormals;
	uint32						m_uiColour; // RGBA
	CVector2D					m_vecTextureCoordinates;
	float32						m_fNormals2[4];
};

#endif