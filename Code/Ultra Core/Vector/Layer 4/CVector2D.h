#ifndef CVector2D_H
#define CVector2D_H

#include "Vector/Layer 3/CVector2f32.h"

class CVector2D : public CVector2f32
{
public:
	CVector2D(void);
	CVector2D(float32 fX, float32 fY);
};

#endif