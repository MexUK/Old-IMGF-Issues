#include "CGUIShapeGeometry_NxPoint.h"

CGUIShapeGeometry_NxPoint::CGUIShapeGeometry_NxPoint(eGUIShape eShapeType) :
	CGUIShape(GUI_SHAPE_GEOMETRY_N_POINTS, eShapeType)
{
}

// position
void							CGUIShapeGeometry_NxPoint::moveItem(CVector2i32& vecPositionChange)
{
	for (CVector2i32& vecPoint : m_vecPoints)
	{
		vecPoint += vecPositionChange;
	}
}