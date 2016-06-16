#ifndef CPoint2D_H
#define CPoint2D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector2i32.h"
#include <math.h>

class CPoint2D : public CVector2i32
{
public:
	CPoint2D(void);
	CPoint2D(int32 x, int32 y);
	
	int32					x(void) { return m_x; }
	int32					y(void) { return m_y; }

	CPoint2D	operator+(const CPoint2D& vecVector2)
	{
		CPoint2D vecVector1;
		vecVector1.m_x = m_x + vecVector2.m_x;
		vecVector1.m_y = m_y + vecVector2.m_y;
		return vecVector1;
	}

	CPoint2D	operator-(const CPoint2D& vecVector2)
	{
		CPoint2D vecVector1;
		vecVector1.m_x = m_x - vecVector2.m_x;
		vecVector1.m_y = m_y - vecVector2.m_y;
		return vecVector1;
	}

	CPoint2D	operator/(const CPoint2D& vecVector2)
	{
		CPoint2D vecVector1;
		vecVector1.m_x = m_x / vecVector2.m_x;
		vecVector1.m_y = m_y / vecVector2.m_y;
		return vecVector1;
	}

	CPoint2D	operator/(const uint32& uiValue2)
	{
		CPoint2D vecVector1;
		vecVector1.m_x = m_x / uiValue2;
		vecVector1.m_y = m_y / uiValue2;
		return vecVector1;
	}

	CPoint2D	operator+=(const CPoint2D& vecVector2)
	{
		m_x += vecVector2.m_x;
		m_y += vecVector2.m_y;
		return *this;
	}

	CPoint2D	operator+=(const CVector2i32& vecVector2)
	{
		m_x += vecVector2.m_x;
		m_y += vecVector2.m_y;
		return *this;
	}

	CPoint2D&	operator+(const CVector2i32& vecVector2)
	{
		CPoint2D vecVector;
		vecVector.m_x = m_x + vecVector2.m_x;
		vecVector.m_y = m_y + vecVector2.m_y;
		return vecVector;
	}
};

#endif