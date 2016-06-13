#include "CGUIShapeGeometry_3xPoint.h"

CGUIShapeGeometry_3xPoint::CGUIShapeGeometry_3xPoint(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_3_POINTS, eShapeType)
{
	m_vecPoint1.m_x = 0;
	m_vecPoint1.m_y = 0;
	m_vecPoint2.m_x = 0;
	m_vecPoint2.m_y = 0;
	m_vecPoint3.m_x = 0;
	m_vecPoint3.m_y = 0;
}

// position
void							CGUIShapeGeometry_3xPoint::moveItem(CVector2i32& vecPositionChange)
{
	m_vecPoint1 += vecPositionChange;
	m_vecPoint2 += vecPositionChange;
	m_vecPoint3 += vecPositionChange;
}