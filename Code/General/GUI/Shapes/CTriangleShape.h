#ifndef CTriangleShape_H
#define CTriangleShape_H

#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_3xPoints.h"

class CTriangleShape : public CGUIShapeGeometry_3xPoints
{
public:
	CTriangleShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CPoint2D& vecPoint);
	CPoint2D						getBoundingRectanglePosition(void);
	CSize2D							getBoundingRectangleSize(void);
};

#endif