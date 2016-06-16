#ifndef CSquareShape_H
#define CSquareShape_H

#include "CPoint2D.h"
#include "CSize2D.h"
#include "GUI/Shape/eGUIShape.h"
#include "GUI/Shapes/Geometries/CGUIShapeGeometry_1xPoint_1x1DSize.h"

class CSquareShape : public CGUIShapeGeometry_1xPoint_1x1DSize
{
public:
	CSquareShape(void);

	void							bindEvents(void);
	void							render(void);
	bool							isPointInItem(CPoint2D& vecPoint);
	CPoint2D						getBoundingRectanglePosition(void);
	CSize2D							getBoundingRectangleSize(void);
};

#endif