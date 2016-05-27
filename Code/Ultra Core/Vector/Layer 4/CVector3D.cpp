#include "CVector3D.h"

CVector3D::CVector3D(void)
{
}
CVector3D::CVector3D(float32 fX, float32 fY, float32 fZ)
{
	m_x = fX;
	m_y = fY;
	m_z = fZ;
}

// operators
CVector3D&		CVector3D::operator=(const CVector3D &vecVector3D)
{
	m_x = vecVector3D.m_x;
	m_y = vecVector3D.m_y;
	m_z = vecVector3D.m_z;
	return *this;
}
CVector3D		CVector3D::operator+(const CVector3D &vecVector3D)
{
	return CVector3D(
		m_x + vecVector3D.m_x,
		m_y + vecVector3D.m_y,
		m_z + vecVector3D.m_z
	);
}
CVector3D		CVector3D::operator-(const CVector3D &vecVector3D)
{
	return CVector3D(
		m_x - vecVector3D.m_x,
		m_y - vecVector3D.m_y,
		m_z - vecVector3D.m_z
	);
}
CVector3D		CVector3D::operator*(const CVector3D &vecVector3D)
{
	return CVector3D(
		m_x * vecVector3D.m_x,
		m_y * vecVector3D.m_y,
		m_z * vecVector3D.m_z
	);
}
CVector3D		CVector3D::operator/(const CVector3D &vecVector3D)
{
	return CVector3D(
		m_x / vecVector3D.m_x,
		m_y / vecVector3D.m_y,
		m_z / vecVector3D.m_z
	);
}
CVector3D		CVector3D::operator%(const CVector3D &vecVector3D)
{
	return CVector3D(
		fmod(m_x, vecVector3D.m_x),
		fmod(m_y, vecVector3D.m_y),
		fmod(m_z, vecVector3D.m_z)
	);
}
bool			CVector3D::operator!=(float32 fValue)
{
	return m_x != fValue
		|| m_y != fValue
		|| m_z != fValue;
}
bool			CVector3D::operator!=(const CVector3D &vecVector3D)
{
	return m_x != vecVector3D.m_x
		|| m_y != vecVector3D.m_y
		|| m_z != vecVector3D.m_z;
}
CVector3D		CVector3D::operator-(void)
{
	return CVector3D(
		-m_x,
		-m_y,
		-m_z
	);
}

// angle unit conversion
void			CVector3D::toRadians(void)
{
	m_x *= 0.0174532925f;
	m_y *= 0.0174532925f;
	m_z *= 0.0174532925f;
}
void			CVector3D::toDegrees(void)
{
	m_x *= 57.2957795f;
	m_y *= 57.2957795f;
	m_z *= 57.2957795f;
}

// angle capping
void			CVector3D::capAngleRadians(void)
{
	float32 fDoublePI = static_cast<float>(M_PI) * 2.0f;
	m_x = fmod(m_x, fDoublePI);
	m_y = fmod(m_y, fDoublePI);
	m_z = fmod(m_z, fDoublePI);
}
void			CVector3D::capAngleDegrees(void)
{
	m_x = fmod(m_x, 360.0f);
	m_y = fmod(m_y, 360.0f);
	m_z = fmod(m_z, 360.0f);
}

// cross product
CVector3D		CVector3D::getCrossProduct(CVector3D &vecVector3D)
{
	return CVector3D(
		m_y * vecVector3D.m_z - m_z * vecVector3D.m_y,
		m_z * vecVector3D.m_x - m_x * vecVector3D.m_z,
		m_x * vecVector3D.m_y - m_y * vecVector3D.m_x
	);
}

// length
float32			CVector3D::getLength(void)
{
	return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}