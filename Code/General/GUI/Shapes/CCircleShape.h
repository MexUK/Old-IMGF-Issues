#ifndef CCircleShape_H
#define CCircleShape_H

#include "CVector2i32.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x1DSize.h"

class CCircleShape : public CGUIShapeGeometry_1xPoint_1x1DSize
{
public:
	CCircleShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CPoint2D& vecPoint);
	CPoint2D						getBoundingRectanglePosition(void);
	CSize2D							getBoundingRectangleSize(void);
};

#endif