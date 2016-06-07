#include "CGUIShapeGeometry_1xPoint_1x2DSize.h"

CGUIShapeGeometry_1xPoint_1x2DSize::CGUIShapeGeometry_1xPoint_1x2DSize(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_1_POINT_1_X_2D_SIZE, eShapeType)
{
	m_vecPosition.m_x = 0;
	m_vecPosition.m_y = 0;
	m_vecSize.m_x = 0;
	m_vecSize.m_y = 0;
}