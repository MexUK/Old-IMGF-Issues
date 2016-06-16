#ifndef CPolygonShape_H
#define CPolygonShape_H

#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_NxPoints.h"

class CPolygonShape : public CGUIShapeGeometry_NxPoints
{
public:
	CPolygonShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CPoint2D& vecPoint);
	CPoint2D						getBoundingRectanglePosition(void);
	CSize2D							getBoundingRectangleSize(void);
};

#endif