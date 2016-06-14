#include "CGUIShapeGeometry_2xPoints.h"

CGUIShapeGeometry_2xPoints::CGUIShapeGeometry_2xPoints(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_2_POINTS, eShapeType)
{
	m_vecPoint1.m_x = 0;
	m_vecPoint1.m_y = 0;
	m_vecPoint2.m_x = 0;
	m_vecPoint2.m_y = 0;
}

// position
void							CGUIShapeGeometry_2xPoints::moveItem(CVector2i32& vecPositionChange)
{
	m_vecPoint1 += vecPositionChange;
	m_vecPoint2 += vecPositionChange;
}