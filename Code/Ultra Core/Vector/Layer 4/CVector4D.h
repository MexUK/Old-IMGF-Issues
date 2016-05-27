#ifndef CVector4D_H
#define CVector4D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector4f32.h"
#include <math.h>

class CVector4D : public CVector4f32
{
public:
	CVector4D(void);
	CVector4D(float32 fX, float32 fY, float32 fZ, float32 fW);
};

#endif