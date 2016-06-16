#include "CSize3D.h"

CSize3D::CSize3D(void)
{
}
CSize3D::CSize3D(uint32 uiWidth, uint32 uiHeight, uint32 uiDepth)
{
	m_x = uiWidth;
	m_y = uiHeight;
	m_z = uiDepth;
}