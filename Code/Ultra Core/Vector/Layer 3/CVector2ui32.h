#ifndef CVector2ui32_H
#define CVector2ui32_H

#include "Types.h"
#include "Vector/Layer 2/CVector2.h"
#include "Vector/Layer 3/CVector2i32.h"

class CVector2ui32 : public CVector2<uint32>
{
public:
	CVector2ui32(void)
	{
	}
	CVector2ui32(uint32 x, uint32 y) : CVector2(x, y)
	{
	}

	CVector2ui32&	operator+(const CVector2i32& vecVector2)
	{
		CVector2ui32 vecVector;
		vecVector.m_x = m_x + vecVector2.m_x;
		vecVector.m_y = m_y + vecVector2.m_y;
		return vecVector;
	}
};

#endif