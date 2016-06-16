#ifndef CSize2D_H
#define CSize2D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector2ui32.h"
#include "Vector/Layer 4/CPoint2D.h"
#include <math.h>

class CSize2D : public CVector2ui32
{
public:
	CSize2D(void);
	CSize2D(uint32 uiWidth, uint32 uiHeight);
	
	uint32					width(void) { return m_x; }
	uint32					height(void) { return m_y; }

	CSize2D(const CVector2& vecVector2)
	{
		m_x = vecVector2.m_x;
		m_y = vecVector2.m_y;
	}

	CSize2D	operator+(const CVector2& vecVector2)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x + vecVector2.m_x;
		vecVector1.m_y = m_y + vecVector2.m_y;
		return vecVector1;
	}
	CSize2D	operator+(const CPoint2D& vecVector2)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x + vecVector2.m_x;
		vecVector1.m_y = m_y + vecVector2.m_y;
		return vecVector1;
	}
	CSize2D	operator+(const uint32 uiValue)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x + uiValue;
		vecVector1.m_y = m_y + uiValue;
		return vecVector1;
	}

	CSize2D	operator-(const CVector2& vecVector2)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x - vecVector2.m_x;
		vecVector1.m_y = m_y - vecVector2.m_y;
		return vecVector1;
	}

	CSize2D	operator/(const uint32 uiValue)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x / uiValue;
		vecVector1.m_y = m_y / uiValue;
		return vecVector1;
	}

	CSize2D&	operator+=(const CVector2& vecVector2)
	{
		*this = *this + vecVector2;
		return *this;
	}
};

#endif