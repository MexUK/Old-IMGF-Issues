#ifndef CSize3D_H
#define CSize3D_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector3ui32.h"
#include <math.h>

class CSize3D : public CVector3ui32
{
public:
	CSize3D(void);
	CSize3D(uint32 uiWidth, uint32 uiHeight, uint32 uiDepth);
	
	uint32					width(void) { return m_x; }
	uint32					height(void) { return m_y; }
	uint32					depth(void) { return m_z; }
};

#endif