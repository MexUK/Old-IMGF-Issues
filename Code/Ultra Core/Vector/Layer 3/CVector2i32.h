#ifndef CVector2i32_H
#define CVector2i32_H

#include "Types.h"
#include "Vector/Layer 2/CVector2.h"

class CVector2i32 : public CVector2<int32>
{
public:
	CVector2i32(void) {};
	CVector2i32(float32 fX, float32 fY)
	{
		m_x = fX;
		m_y = fY;
	}
};

#endif