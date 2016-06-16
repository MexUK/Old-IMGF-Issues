#ifndef CPoint3D_H
#define CPoint3D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector3i32.h"
#include <math.h>

class CPoint3D : public CVector3i32
{
public:
	CPoint3D(void);
	CPoint3D(int32 x, int32 y, int32 z);
	
	int32					x(void) { return m_x; }
	int32					y(void) { return m_y; }
	int32					z(void) { return m_z; }
};

#endif