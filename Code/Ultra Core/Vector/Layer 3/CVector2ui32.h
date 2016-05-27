#ifndef CVector2ui32_H
#define CVector2ui32_H

#include "Types.h"
#include "Vector/Layer 2/CVector2.h"

class CVector2ui32 : public CVector2<uint32>
{
public:
	CVector2ui32(void) : CVector2() {};
	CVector2ui32(uint32 x, uint32 y) : CVector2(x, y) {};

	CVector2ui32	operator+(const CVector2& vecVector2)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x + vecVector2.m_x;
		vecVector1.m_y = m_y + vecVector2.m_y;
		return vecVector1;
	}
	CVector2ui32	operator+(const uint32 uiValue)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x + uiValue;
		vecVector1.m_y = m_y + uiValue;
		return vecVector1;
	}

	CVector2ui32	operator-(const CVector2& vecVector2)
	{
		CVector2ui32 vecVector1;
		vecVector1.m_x = m_x - vecVector2.m_x;
		vecVector1.m_y = m_y - vecVector2.m_y;
		return vecVector1;
	}

	CVector2ui32&	operator+=(const CVector2& vecVector2)
	{
		*this = *this + vecVector2;
		return *this;
	}
};

#endif