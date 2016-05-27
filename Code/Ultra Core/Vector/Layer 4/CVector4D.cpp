#include "CVector4D.h"

CVector4D::CVector4D(void)
{
}
CVector4D::CVector4D(float32 fX, float32 fY, float32 fZ, float32 fW)
{
	m_x = fX;
	m_y = fY;
	m_z = fZ;
	m_w = fW;
}