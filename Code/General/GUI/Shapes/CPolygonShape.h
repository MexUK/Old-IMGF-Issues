#ifndef CPolygonShape_H
#define CPolygonShape_H

#include "CVector2i32.h"
#include "CVector2ui32.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_NxPoint.h"

class CPolygonShape : public CGUIShapeGeometry_NxPoint
{
public:
	CPolygonShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CVector2i32& vecPoint);
	CVector2i32						getBoundingRectanglePosition(void);
	CVector2ui32					getBoundingRectangleSize(void);
};

#endif