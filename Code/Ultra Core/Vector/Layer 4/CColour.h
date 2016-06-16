#ifndef CColour_H
#define CColour_H

#define _USE_MATH_DEFINES

#include "Vector/Layer 3/CVector4ui8.h"
#include <math.h>

class CColour : public CVector4ui8
{
public:
	CColour(void);
	CColour(uint8 uiRed, uint8 uiGreen, uint8 uiBlue, uint8 uiAlpha = 0xFF);

	uint8					red(void) { return m_x; }
	uint8					green(void) { return m_y; }
	uint8					blue(void) { return m_z; }
	uint8					alpha(void) { return m_w; }
};

#endif