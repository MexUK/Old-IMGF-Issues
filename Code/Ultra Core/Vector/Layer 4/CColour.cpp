#include "CColour.h"

CColour::CColour(void)
{
}
CColour::CColour(uint8 uiRed, uint8 uiGreen, uint8 uiBlue, uint8 uiAlpha)
{
	m_x = uiRed;
	m_y = uiGreen;
	m_z = uiBlue;
	m_w = uiAlpha;
}