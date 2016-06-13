#ifndef CVector2i32_H
#define CVector2i32_H

#include "Types.h"
#include "Vector/Layer 2/CVector2.h"

class CVector2i32 : public CVector2<int32>
{
public:
	CVector2i32(void)
	{
	}
	CVector2i32(float32 fX, float32 fY) : CVector2(fX, fY)
	{
	}

	CVector2i32	operator+(const CVector2i32& vecVector2)
	{
		CVector2i32 vecVector1;
		vecVector1.m_x = m_x + vecVector2.m_x;
		vecVector1.m_y = m_y + vecVector2.m_y;
		return vecVector1;
	}

	CVector2i32	operator-(const CVector2i32& vecVector2)
	{
		CVector2i32 vecVector1;
		vecVector1.m_x = m_x - vecVector2.m_x;
		vecVector1.m_y = m_y - vecVector2.m_y;
		return vecVector1;
	}

	CVector2i32	operator/(const CVector2i32& vecVector2)
	{
		CVector2i32 vecVector1;
		vecVector1.m_x = m_x / vecVector2.m_x;
		vecVector1.m_y = m_y / vecVector2.m_y;
		return vecVector1;
	}

	CVector2i32	operator/(const uint32& uiValue2)
	{
		CVector2i32 vecVector1;
		vecVector1.m_x = m_x / uiValue2;
		vecVector1.m_y = m_y / uiValue2;
		return vecVector1;
	}

	CVector2i32	operator+=(const CVector2i32& vecVector2)
	{
		m_x += vecVector2.m_x;
		m_y += vecVector2.m_y;
		return *this;
	}
};

#endif