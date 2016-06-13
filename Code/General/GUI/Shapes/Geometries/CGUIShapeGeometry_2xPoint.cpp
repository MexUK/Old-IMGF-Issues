#include "CGUIShapeGeometry_2xPoint.h"

CGUIShapeGeometry_2xPoint::CGUIShapeGeometry_2xPoint(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_2_POINTS, eShapeType)
{
	m_vecPoint1.m_x = 0;
	m_vecPoint1.m_y = 0;
	m_vecPoint2.m_x = 0;
	m_vecPoint2.m_y = 0;
}

// position
void							CGUIShapeGeometry_2xPoint::moveItem(CVector2i32& vecPositionChange)
{
	m_vecPoint1 += vecPositionChange;
	m_vecPoint2 += vecPositionChange;
}