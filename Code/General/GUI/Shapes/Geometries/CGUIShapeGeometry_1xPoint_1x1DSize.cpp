#include "CGUIShapeGeometry_1xPoint_1x1DSize.h"

CGUIShapeGeometry_1xPoint_1x1DSize::CGUIShapeGeometry_1xPoint_1x1DSize(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_1_POINT_1_X_1D_SIZE, eShapeType)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_fSize = 0.0f;
}