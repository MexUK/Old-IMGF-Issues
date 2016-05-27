#ifndef CVector3D_H
#define CVector3D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector3f32.h"
#include <math.h>

class CVector3D : public CVector3f32
{
public:
	CVector3D(void);
	CVector3D(float32 fX, float32 fY, float32 fZ);

	/*
	CVector3D&	operator=	(VC::CVector& vecVectorVC)
	//operator VC::CVector&()
	{
		//return CVector3D::operator=(vecVectorVC);
		//m_X = vecVectorVC.m_X;
		//m_Y = vecVectorVC.m_Y;
		//m_Z = vecVectorVC.m_Z;

		//return *this;

		CVector3D vec(vecVectorVC.m_X, vecVectorVC.m_Y, vecVectorVC.m_Z);
		return vec;
	}
	*/

	/*
	CVector3D&	operator=(VC::CVector& vecVectorVC)
	{
		this->m_X = 1.0f;
		this->m_Y = 1.0f;
		this->m_Z = 15.0f;
	}
	*/

	CVector3D&		operator=(const CVector3D &vecVector3D);
	CVector3D		operator+(const CVector3D &vecVector3D);
	CVector3D		operator-(const CVector3D &vecVector3D);
	CVector3D		operator*(const CVector3D &vecVector3D);
	CVector3D		operator/(const CVector3D &vecVector3D);
	CVector3D		operator%(const CVector3D &vecVector3D);
	bool			operator!=(float32 fValue);
	bool			operator!=(const CVector3D &vecVector3D);
	CVector3D		operator-(void);
	template <class T>	CVector3D		operator+(const T value);
	template <class T>	CVector3D		operator-(const T value);
	template <class T>	CVector3D		operator*(const T value);
	template <class T>	CVector3D		operator/(const T value);
	template <class T>	CVector3D		operator%(const T value);

	void			toRadians(void);
	void			toDegrees(void);

	void			capAngleRadians(void);
	void			capAngleDegrees(void);

	CVector3D		getCrossProduct(CVector3D &vecVector3D);
	float32			getLength(void);
};

template <class T>
CVector3D		CVector3D::operator+(const T value)
{
	return CVector3D(
		m_x + value,
		m_y + value,
		m_z + value
		);
}
template <class T>
CVector3D		CVector3D::operator-(const T value)
{
	return CVector3D(
		m_x - value,
		m_y - value,
		m_z - value
		);
}
template <class T>
CVector3D		CVector3D::operator*(const T value)
{
	return CVector3D(
		m_x * value,
		m_y * value,
		m_z * value
		);
}
template <class T>
CVector3D		CVector3D::operator/(const T value)
{
	return CVector3D(
		m_x / value,
		m_y / value,
		m_z / value
		);
}
template <class T>
CVector3D		CVector3D::operator%(const T value)
{
	return CVector3D(
		fmod(m_x, value),
		fmod(m_y, value),
		fmod(m_z, value)
		);
}

#endif