#include "CGUIShapeGeometry_NxPoints.h"

CGUIShapeGeometry_NxPoints::CGUIShapeGeometry_NxPoints(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_N_POINTS, eShapeType)
{
}

// position
void							CGUIShapeGeometry_NxPoints::moveItem(CVector2i32& vecPositionChange)
{
	for (CPoint2D& vecPoint : m_vecPoints)
	{
		vecPoint += vecPositionChange;
	}
}